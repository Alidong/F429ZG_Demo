/**
  ******************************************************************************
  * @file    gt5xx.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   i2c电容屏驱动函数gt9157芯片
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "stdio.h"
#include "stdlib.h"
#include "gt5xx.h"
#include "bsp_i2c_touch.h"

// 4.5寸屏GT5688驱动配置
const uint8_t CTP_CFG_GT5688[] = {
    0x96, 0xE0, 0x01, 0x56, 0x03, 0x05, 0x35, 0x00, 0x01, 0x00,
    0x00, 0x05, 0x50, 0x3C, 0x53, 0x11, 0x00, 0x00, 0x22, 0x22,
    0x14, 0x18, 0x1A, 0x1D, 0x0A, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x53, 0x00, 0x14, 0x00, 0x00, 0x84, 0x00, 0x00,
    0x3C, 0x19, 0x19, 0x64, 0x1E, 0x28, 0x88, 0x29, 0x0A, 0x2D,
    0x2F, 0x29, 0x0C, 0x20, 0x33, 0x60, 0x13, 0x02, 0x24, 0x00,
    0x00, 0x20, 0x3C, 0xC0, 0x14, 0x02, 0x00, 0x00, 0x54, 0xAC,
    0x24, 0x9C, 0x29, 0x8C, 0x2D, 0x80, 0x32, 0x77, 0x37, 0x6E,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x50, 0x3C,
    0xFF, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x02, 0x14, 0x14, 0x03,
    0x04, 0x00, 0x21, 0x64, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x32, 0x20, 0x50, 0x3C, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0D, 0x06, 0x0C, 0x05, 0x0B, 0x04, 0x0A, 0x03, 0x09, 0x02,
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x05, 0x1E, 0x00, 0x02,
    0x2A, 0x1E, 0x19, 0x14, 0x02, 0x00, 0x03, 0x0A, 0x05, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0x86,
    0x22, 0x03, 0x00, 0x00, 0x33, 0x00, 0x0F, 0x00, 0x00, 0x00,
    0x50, 0x3C, 0x50, 0x00, 0x00, 0x00, 0x1A, 0x64, 0x01

};

//uint8_t config[GTP_CONFIG_MAX_LENGTH + GTP_ADDR_LENGTH]
//                = {GTP_REG_CONFIG_DATA >> 8, GTP_REG_CONFIG_DATA & 0xff};

TOUCH_IC touchIC;

static int8_t GTP_I2C_Test(void);

static void Delay(__IO uint32_t nCount) //简单的延时函数
{
    for (; nCount != 0; nCount--)
        ;
}

/**
  * @brief   使用IIC进行数据传输
  * @param
  *		@arg i2c_msg:数据传输结构体
  *		@arg num:数据传输结构体的个数
  * @retval  正常完成的传输结构个数，若不正常，返回0xff
  */
static int I2C_Transfer(struct i2c_msg *msgs, int num)
{
    int im = 0;
    int ret = 0;

    GTP_DEBUG_FUNC();

    for (im = 0; ret == 0 && im != num; im++)
    {
        if ((msgs[im].flags & I2C_M_RD)) //根据flag判断是读数据还是写数据
        {
            ret = I2C_ReadBytes(msgs[im].addr, msgs[im].buf, msgs[im].len); //IIC读取数据
        }
        else
        {
            ret = I2C_WriteBytes(msgs[im].addr, msgs[im].buf, msgs[im].len); //IIC写入数据
        }
    }

    if (ret)
        return ret;

    return im; //正常完成的传输结构个数
}

/**
  * @brief   从IIC设备中读取数据
  * @param
  *		@arg client_addr:设备地址
  *		@arg  buf[0~1]: 读取数据寄存器的起始地址
  *		@arg buf[2~len-1]: 存储读出来数据的缓冲buffer
  *		@arg len:    GTP_ADDR_LENGTH + read bytes count（寄存器地址长度+读取的数据字节数）
  * @retval  i2c_msgs传输结构体的个数，2为成功，其它为失败
  */
static int32_t GTP_I2C_Read(uint8_t client_addr, uint8_t *buf, int32_t len)
{
    struct i2c_msg msgs[2];
    int32_t ret = -1;
    int32_t retries = 0;

    GTP_DEBUG_FUNC();
    /*一个读数据的过程可以分为两个传输过程:
     * 1. IIC  写入 要读取的寄存器地址
     * 2. IIC  读取  数据
     * */

    msgs[0].flags = !I2C_M_RD;     //写入
    msgs[0].addr = client_addr;    //IIC设备地址
    msgs[0].len = GTP_ADDR_LENGTH; //寄存器地址为2字节(即写入两字节的数据)
    msgs[0].buf = &buf[0];         //buf[0~1]存储的是要读取的寄存器地址

    msgs[1].flags = I2C_M_RD;            //读取
    msgs[1].addr = client_addr;          //IIC设备地址
    msgs[1].len = len - GTP_ADDR_LENGTH; //要读取的数据长度
    msgs[1].buf = &buf[GTP_ADDR_LENGTH]; //buf[GTP_ADDR_LENGTH]之后的缓冲区存储读出的数据

    while (retries < 5)
    {
        ret = I2C_Transfer(msgs, 2); //调用IIC数据传输过程函数，有2个传输过程
        if (ret == 2)
            break;
        retries++;
    }
    if ((retries >= 5))
    {
        GTP_ERROR("I2C Read: 0x%04X, %d bytes failed, errcode: %d! Process reset.", (((uint16_t)(buf[0] << 8)) | buf[1]), len - 2, ret);
    }
    return ret;
}

/**
  * @brief   向IIC设备写入数据
  * @param
  *		@arg client_addr:设备地址
  *		@arg  buf[0~1]: 要写入的数据寄存器的起始地址
  *		@arg buf[2~len-1]: 要写入的数据
  *		@arg len:    GTP_ADDR_LENGTH + write bytes count（寄存器地址长度+写入的数据字节数）
  * @retval  i2c_msgs传输结构体的个数，1为成功，其它为失败
  */
static int32_t GTP_I2C_Write(uint8_t client_addr, uint8_t *buf, int32_t len)
{
    struct i2c_msg msg;
    int32_t ret = -1;
    int32_t retries = 0;

    GTP_DEBUG_FUNC();
    /*一个写数据的过程只需要一个传输过程:
     * 1. IIC连续 写入 数据寄存器地址及数据
     * */
    msg.flags = !I2C_M_RD;  //写入
    msg.addr = client_addr; //从设备地址
    msg.len = len;          //长度直接等于(寄存器地址长度+写入的数据字节数)
    msg.buf = buf;          //直接连续写入缓冲区中的数据(包括了寄存器地址)

    while (retries < 5)
    {
        ret = I2C_Transfer(&msg, 1); //调用IIC数据传输过程函数，1个传输过程
        if (ret == 1)
            break;
        retries++;
    }
    if ((retries >= 5))
    {

        GTP_ERROR("I2C Write: 0x%04X, %d bytes failed, errcode: %d! Process reset.", (((uint16_t)(buf[0] << 8)) | buf[1]), len - 2, ret);
    }
    return ret;
}

/**
  * @brief   使用IIC读取再次数据，检验是否正常
  * @param
  *		@arg client:设备地址
  *		@arg  addr: 寄存器地址
  *		@arg rxbuf: 存储读出的数据
  *		@arg len:    读取的字节数
  * @retval
  * 	@arg FAIL
  * 	@arg SUCCESS
  */
int32_t GTP_I2C_Read_dbl_check(uint8_t client_addr, uint16_t addr, uint8_t *rxbuf, int len)
{
    uint8_t buf[16] = {0};
    uint8_t confirm_buf[16] = {0};
    uint8_t retry = 0;

    GTP_DEBUG_FUNC();

    while (retry++ < 3)
    {
        memset(buf, 0xAA, 16);
        buf[0] = (uint8_t)(addr >> 8);
        buf[1] = (uint8_t)(addr & 0xFF);
        GTP_I2C_Read(client_addr, buf, len + 2);

        memset(confirm_buf, 0xAB, 16);
        confirm_buf[0] = (uint8_t)(addr >> 8);
        confirm_buf[1] = (uint8_t)(addr & 0xFF);
        GTP_I2C_Read(client_addr, confirm_buf, len + 2);

        if (!memcmp(buf, confirm_buf, len + 2))
        {
            memcpy(rxbuf, confirm_buf + 2, len);
            return SUCCESS;
        }
    }
    GTP_ERROR("I2C read 0x%04X, %d bytes, double check failed!", addr, len);
    return FAIL;
}

/**
  * @brief   关闭GT91xx中断
  * @param 无
  * @retval 无
  */
void GTP_IRQ_Disable(void)
{

    GTP_DEBUG_FUNC();
}

/**
  * @brief   使能GT91xx中断
  * @param 无
  * @retval 无
  */
void GTP_IRQ_Enable(void)
{
    GTP_DEBUG_FUNC();
}

/**
  * @brief   用于处理或报告触屏检测到按下
  * @param
  *    @arg     ID: 触摸顺序trackID
  *    @arg     x:  触摸的 x 坐标
  *    @arg     y:  触摸的 y 坐标
  *    @arg     w:  触摸的 大小
  * @retval 无
  */
/*用于记录连续触摸时(长按)的上一次触摸位置，负数值表示上一次无触摸按下*/
static int16_t pre_x[GTP_MAX_TOUCH] = {-1, -1, -1, -1, -1};
static int16_t pre_y[GTP_MAX_TOUCH] = {-1, -1, -1, -1, -1};

void GTP_Touch_Down(int32_t ID, int32_t x, int32_t y, int32_t w)
{

    GTP_DEBUG_FUNC();

    /*取x、y初始值大于屏幕像素值*/
    GTP_DEBUG("ID:%d, X:%d, Y:%d, W:%d", ID, x, y, w);

    /************************************/
    /*在此处添加自己的触摸点按下时处理过程即可*/
    /* (x,y) 即为最新的触摸点 *************/
    /************************************/

    /*prex,prey数组存储上一次触摸的位置，ID为轨迹编号(多点触控时有多轨迹)*/
    pre_x[ID] = x;
    pre_y[ID] = y;
}

/**
  * @brief   用于处理或报告触屏释放
  * @param 释放点的ID号
  * @retval 无
  */
static void GTP_Touch_Up(int32_t ID)
{

    /*****************************************/
    /*在此处添加自己的触摸点释放时的处理过程即可*/
    /* pre_x[ID],pre_y[ID] 即为最新的释放点 ****/
    /*******************************************/
    /***ID为轨迹编号(多点触控时有多轨迹)********/

    /*触笔释放，把pre xy 重置为负*/
    pre_x[ID] = -1;
    pre_y[ID] = -1;
    GTP_DEBUG("Touch ID[%2d] release!", ID);
}

/**
  * @brief   触屏处理函数，轮询或者在触摸中断调用
  * @param 无
  * @retval 无
  */
static void Goodix_TS_Work_Func(void)
{
    uint8_t end_cmd[3] = {GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF, 0};
    uint8_t point_data[2 + 1 + 8 * GTP_MAX_TOUCH + 1] = {GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF};
    uint8_t touch_num = 0;
    uint8_t finger = 0;
    static uint16_t pre_touch = 0;
    static uint8_t pre_id[GTP_MAX_TOUCH] = {0};

    uint8_t client_addr = GTP_ADDRESS;
    uint8_t *coor_data = NULL;
    int32_t input_x = 0;
    int32_t input_y = 0;
    int32_t input_w = 0;
    uint8_t ID = 0;

    int32_t i = 0;
    int32_t ret = -1;

    GTP_DEBUG_FUNC();

    ret = GTP_I2C_Read(client_addr, point_data, 12); //10字节寄存器加2字节地址
    if (ret < 0)
    {
        GTP_ERROR("I2C transfer error. errno:%d\n ", ret);

        return;
    }

    finger = point_data[GTP_ADDR_LENGTH]; //状态寄存器数据

    if (finger == 0x00) //没有数据，退出
    {

        return;
    }

    if ((finger & 0x80) == 0) //判断buffer status位
    {
        goto exit_work_func; //坐标未就绪，数据无效
    }

    touch_num = finger & 0x0f; //坐标点数
    if (touch_num > GTP_MAX_TOUCH)
    {
        goto exit_work_func; //大于最大支持点数，错误退出
    }

    if (touch_num > 1) //不止一个点
    {
        uint8_t buf[8 * GTP_MAX_TOUCH] = {(GTP_READ_COOR_ADDR + 10) >> 8, (GTP_READ_COOR_ADDR + 10) & 0xff};

        ret = GTP_I2C_Read(client_addr, buf, 2 + 8 * (touch_num - 1));
        memcpy(&point_data[12], &buf[2], 8 * (touch_num - 1)); //复制其余点数的数据到point_data
    }

    if (pre_touch > touch_num) //pre_touch>touch_num,表示有的点释放了
    {
        for (i = 0; i < pre_touch; i++) //一个点一个点处理
        {
            uint8_t j;
            for (j = 0; j < touch_num; j++)
            {
                coor_data = &point_data[j * 8 + 3];
                ID = coor_data[0] & 0x0F; //track ID
                if (pre_id[i] == ID)
                    break;

                if (j >= touch_num - 1) //遍历当前所有ID都找不到pre_id[i]，表示已释放
                {
                    GTP_Touch_Up(pre_id[i]);
                }
            }
        }
    }

    if (touch_num)
    {
        for (i = 0; i < touch_num; i++) //一个点一个点处理
        {
            coor_data = &point_data[i * 8 + 3];

            ID = coor_data[0] & 0x0F; //track ID
            pre_id[i] = ID;

            input_y = coor_data[1] | (coor_data[2] << 8); //x坐标
            input_x = coor_data[3] | (coor_data[4] << 8); //y坐标
            input_w = coor_data[5] | (coor_data[6] << 8); //size

            {

                /*根据扫描模式更正X/Y起始方向*/
                switch (LCD_SCAN_MODE)
                {
                case 0:
                case 7:
                    input_y = LCD_Y_LENGTH - input_y;
                    break;

                case 2:
                case 3:
                    input_x = LCD_X_LENGTH - input_x;
                    input_y = LCD_Y_LENGTH - input_y;
                    break;

                case 1:
                case 6:
                    input_x = LCD_X_LENGTH - input_x;
                    break;

                default:
                    break;
                }
                GTP_Touch_Down(ID, input_x, input_y, input_w); //数据处理
            }
        }
    }
    else if (pre_touch) //touch_ num=0 且pre_touch！=0
    {
        for (i = 0; i < pre_touch; i++)
        {
            GTP_Touch_Up(pre_id[i]);
        }
    }

    pre_touch = touch_num;

exit_work_func:
{
    ret = GTP_I2C_Write(client_addr, end_cmd, 3);
    if (ret < 0)
    {
        GTP_INFO("I2C write end_cmd error!");
    }
}
}

/**
  * @brief   给触屏芯片重新复位
  * @param 无
  * @retval 无
  */
int8_t GTP_Reset_Guitar(void)
{
    GTP_DEBUG_FUNC();
#if 1
    I2C_ResetChip();
    return 0;
#else //软件复位
    int8_t ret = -1;
    int8_t retry = 0;
    uint8_t reset_command[3] = {(uint8_t)GTP_REG_COMMAND >> 8, (uint8_t)GTP_REG_COMMAND & 0xFF, 2};

    //写入复位命令
    while (retry++ < 5)
    {
        ret = GTP_I2C_Write(GTP_ADDRESS, reset_command, 3);
        if (ret > 0)
        {
            GTP_INFO("GTP enter sleep!");

            return ret;
        }
    }
    GTP_ERROR("GTP send sleep cmd failed.");
    return ret;
#endif
}

/**
   * @brief   进入睡眠模式
   * @param 无
   * @retval 1为成功，其它为失败
   */
//int8_t GTP_Enter_Sleep(void)
//{
//    int8_t ret = -1;
//    int8_t retry = 0;
//    uint8_t reset_comment[3] = {(uint8_t)(GTP_REG_COMMENT >> 8), (uint8_t)GTP_REG_COMMENT&0xFF, 5};//5
//
//    GTP_DEBUG_FUNC();
//
//    while(retry++ < 5)
//    {
//        ret = GTP_I2C_Write(GTP_ADDRESS, reset_comment, 3);
//        if (ret > 0)
//        {
//            GTP_INFO("GTP enter sleep!");
//
//            return ret;
//        }
//
//    }
//    GTP_ERROR("GTP send sleep cmd failed.");
//    return ret;
//}

int8_t GTP_Send_Command(uint8_t command)
{
    int8_t ret = -1;
    int8_t retry = 0;
    uint8_t command_buf[3] = {(uint8_t)(GTP_REG_COMMAND >> 8), (uint8_t)GTP_REG_COMMAND & 0xFF, GTP_COMMAND_READSTATUS};

    GTP_DEBUG_FUNC();

    while (retry++ < 5)
    {
        ret = GTP_I2C_Write(GTP_ADDRESS, command_buf, 3);
        if (ret > 0)
        {
            GTP_INFO("send command success!");

            return ret;
        }
    }
    GTP_ERROR("send command fail!");
    return ret;
}

/**
  * @brief   唤醒触摸屏
  * @param 无
  * @retval 0为成功，其它为失败
  */
int8_t GTP_WakeUp_Sleep(void)
{
    uint8_t retry = 0;
    int8_t ret = -1;

    GTP_DEBUG_FUNC();

    while (retry++ < 10)
    {
        ret = GTP_I2C_Test();
        if (ret > 0)
        {
            GTP_INFO("GTP wakeup sleep.");
            return ret;
        }
        GTP_Reset_Guitar();
    }

    GTP_ERROR("GTP wakeup sleep failed.");
    return ret;
}

static int32_t GTP_Get_Info(void)
{
    uint8_t opr_buf[10] = {0};
    int32_t ret = 0;

    uint16_t abs_x_max = GTP_MAX_WIDTH;
    uint16_t abs_y_max = GTP_MAX_HEIGHT;
    uint8_t int_trigger_type = GTP_INT_TRIGGER;

    opr_buf[0] = (uint8_t)((GTP_REG_CONFIG_DATA + 1) >> 8);
    opr_buf[1] = (uint8_t)((GTP_REG_CONFIG_DATA + 1) & 0xFF);

    ret = GTP_I2C_Read(GTP_ADDRESS, opr_buf, 10);
    if (ret < 0)
    {
        return FAIL;
    }

    abs_x_max = (opr_buf[3] << 8) + opr_buf[2];
    abs_y_max = (opr_buf[5] << 8) + opr_buf[4];
    GTP_DEBUG("RES");
    GTP_DEBUG_ARRAY(&opr_buf[0], 10);

    opr_buf[0] = (uint8_t)((GTP_REG_CONFIG_DATA + 6) >> 8);
    opr_buf[1] = (uint8_t)((GTP_REG_CONFIG_DATA + 6) & 0xFF);
    ret = GTP_I2C_Read(GTP_ADDRESS, opr_buf, 3);
    if (ret < 0)
    {
        return FAIL;
    }
    int_trigger_type = opr_buf[2] & 0x03;

    GTP_INFO("X_MAX = %d, Y_MAX = %d, TRIGGER = 0x%02x",
             abs_x_max, abs_y_max, int_trigger_type);

    return SUCCESS;
}

/*******************************************************
Function:
    Initialize gtp.
Input:
    ts: goodix private data
Output:
    Executive outcomes.
        0: succeed, otherwise: failed
*******************************************************/
int32_t GTP_Init_Panel(void)
{
    int32_t ret = -1;

    int32_t i = 0;
    uint16_t check_sum = 0;
    int32_t retry = 0;

    GTP_DEBUG_FUNC();

    I2C_ResetChip();

    ret = GTP_I2C_Test();
    if (ret < 0)
    {
        GTP_ERROR("I2C communication ERROR!");
        return ret;
    }

    //获取触摸IC的型号
    GTP_Read_Version();

#if 0 //读出写入的数据，检查是否正常写入
    //检验读出的数据与写入的是否相同
	{
    	    uint16_t i;
    	    uint8_t buf[300];
    	     buf[0] = config[0];
    	     buf[1] =config[1];    //寄存器地址

    	    GTP_DEBUG_FUNC();

    	    ret = GTP_I2C_Read(GTP_ADDRESS, buf, sizeof(buf));
			   
					GTP_DEBUG("read ");

					GTP_DEBUG_ARRAY(buf,cfg_num);
		
			    GTP_DEBUG("write ");

					GTP_DEBUG_ARRAY(config,cfg_num);

					//不对比版本号
    	    for(i=1;i<cfg_num+GTP_ADDR_LENGTH-3;i++)
    	    {

    	    	if(config[i] != buf[i])
    	    	{
    	    		GTP_ERROR("Config fail ! i = %d ",i);
							free(config);
    	    		return -1;
    	    	}
    	    }
    	    if(i==cfg_num+GTP_ADDR_LENGTH-3)
	    		GTP_DEBUG("Config success ! i = %d ",i);
	}
#endif

    /*使能中断，这样才能检测触摸数据*/

    GTP_Get_Info();

    return 0;
}

/*******************************************************
Function:
    Read chip version.
Input:
    client:  i2c device
    version: buffer to keep ic firmware version
Output:
    read operation return.
        2: succeed, otherwise: failed
*******************************************************/
int32_t GTP_Read_Version(void)
{
    int32_t ret = -1;
    uint8_t buf[8] = {GTP_REG_VERSION >> 8, GTP_REG_VERSION & 0xff}; //寄存器地址

    GTP_DEBUG_FUNC();

    ret = GTP_I2C_Read(GTP_ADDRESS, buf, sizeof(buf));
    if (ret < 0)
    {
        GTP_ERROR("GTP read version failed");
        return ret;
    }
    if (buf[2] == '5')
    {
        GTP_INFO("IC1 Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

        //GT5688芯片
        if (buf[2] == '5' && buf[3] == '6' && buf[4] == '8' && buf[5] == '8')
            touchIC = GT5688;
    }
    else if (buf[5] == 0x00)
    {
        GTP_INFO("IC2 Version: %c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[7], buf[6]);

        //GT911芯片
        if (buf[2] == '9' && buf[3] == '1' && buf[4] == '1')
            touchIC = GT911;
    }
    else
    {
        GTP_INFO("IC3 Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

        //GT9157芯片
        if (buf[2] == '9' && buf[3] == '1' && buf[4] == '5' && buf[5] == '7')
            touchIC = GT9157;
    }
    return ret;
}

/*******************************************************
Function:
    I2c test Function.
Input:
    client:i2c client.
Output:
    Executive outcomes.
        2: succeed, otherwise failed.
*******************************************************/
static int8_t GTP_I2C_Test(void)
{
    uint8_t test[3] = {GTP_REG_CONFIG_DATA >> 8, GTP_REG_CONFIG_DATA & 0xff};
    uint8_t retry = 0;
    int8_t ret = -1;

    GTP_DEBUG_FUNC();

    while (retry++ < 5)
    {
        ret = GTP_I2C_Read(GTP_ADDRESS, test, 3);
        if (ret > 0)
        {
            return ret;
        }
        GTP_ERROR("GTP i2c test failed time %d.", retry);
    }
    return ret;
}
bool Check_finger(void)
{
    if (pre_x[0] == -1 || pre_y[0] == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}
void Get_Touch_XY(int16_t *x, int16_t *y)
{
    *x = pre_x[0];
    *y = pre_y[0];
}
//检测到触摸中断时调用，
void GTP_TouchProcess(void)
{
    GTP_DEBUG_FUNC();
    Goodix_TS_Work_Func();
}
