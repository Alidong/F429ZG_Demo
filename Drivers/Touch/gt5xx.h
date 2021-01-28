/* drivers/input/touchscreen/gt9xx.h
 * 
 * 2010 - 2013 Goodix Technology.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be a reference 
 * to you, when you are integrating the GOODiX's CTP IC into your system, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details.
 * 
 */

#ifndef _GOODIX_GT9XX_H
#define _GOODIX_GT9XX_H

#include "stm32f4xx.h"
#include "stdbool.h"
#ifndef NULL
#define NULL 0
#endif

/*flags锟侥匡拷取值锟斤拷注锟酵碉拷锟斤拷锟节憋拷锟斤拷锟斤拷没锟斤拷锟矫碉拷*/
//#define I2C_M_TEN		0x0010	/* 锟斤拷示锟斤拷锟角革拷10位锟斤拷址 */
//#define I2C_M_NOSTART		0x4000	/* if I2C_FUNC_PROTOCOL_MANGLING */
//#define I2C_M_REV_DIR_ADDR	0x2000	/* if I2C_FUNC_PROTOCOL_MANGLING */
//#define I2C_M_IGNORE_NAK	0x1000	/* if I2C_FUNC_PROTOCOL_MANGLING */
//#define I2C_M_NO_RD_ACK		0x0800	/* if I2C_FUNC_PROTOCOL_MANGLING */
//#define I2C_M_RECV_LEN		0x0400	/* length will be first received byte */

/* 锟斤拷示锟斤拷锟斤拷锟斤拷 */
#define I2C_M_RD 0x0001
/*
 * 锟芥储I2C通讯锟斤拷锟斤拷息
 * @addr锟斤拷  锟斤拷锟借备锟斤拷I2C锟借备锟斤拷址	
 * @flags: 锟斤拷锟狡憋拷志
 * @len锟斤拷  锟斤拷写锟斤拷锟捷的筹拷锟斤拷
 * @buf锟斤拷  锟芥储锟斤拷写锟斤拷锟捷碉拷指锟斤拷
 **/
struct i2c_msg
{
  uint8_t addr;   /*锟斤拷锟借备锟斤拷I2C锟借备锟斤拷址 */
  uint16_t flags; /*锟斤拷锟狡憋拷志*/
  uint16_t len;   /*锟斤拷写锟斤拷锟捷的筹拷锟斤拷			*/
  uint8_t *buf;   /*锟芥储锟斤拷写锟斤拷锟捷碉拷指锟斤拷	*/
};

typedef enum
{
  GT9157 = 0,
  GT911 = 1,
  GT5688 = 2,
} TOUCH_IC;

// STEP_3(optional): Specify your special config info if needed
#define GTP_MAX_HEIGHT 320
#define GTP_MAX_WIDTH 480
#define GTP_INT_TRIGGER 0
#define GTP_MAX_TOUCH 5

#define LCD_X_LENGTH GTP_MAX_WIDTH
#define LCD_Y_LENGTH GTP_MAX_HEIGHT
#define LCD_SCAN_MODE 1

//***************************PART3:OTHER define*********************************
#define GTP_DRIVER_VERSION "V2.2<2014/01/14>"
#define GTP_I2C_NAME "Goodix-TS"
#define GT91XX_CONFIG_PROC_FILE "gt9xx_config"
#define GTP_POLL_TIME 10
#define GTP_ADDR_LENGTH 2
#define GTP_CONFIG_MIN_LENGTH 186
#define GTP_CONFIG_MAX_LENGTH 256
#define FAIL 0
#define SUCCESS 1
#define SWITCH_OFF 0
#define SWITCH_ON 1

//******************** For GT9XXF Start **********************//
#define GTP_REG_BAK_REF 0x99EC
#define GTP_REG_MAIN_CLK 0x8020
#define GTP_REG_CHIP_TYPE 0x8000
#define GTP_REG_HAVE_KEY 0x8057
#define GTP_REG_MATRIX_DRVNUM 0x8069
#define GTP_REG_MATRIX_SENNUM 0x806A
#define GTP_REG_COMMAND 0x8040

#define GTP_COMMAND_READSTATUS 0
#define GTP_COMMAND_DIFFERENCE 1
#define GTP_COMMAND_SOFTRESET 2
#define GTP_COMMAND_UPDATE 3
#define GTP_COMMAND_CALCULATE 4
#define GTP_COMMAND_TURNOFF 5

#define GTP_FL_FW_BURN 0x00
#define GTP_FL_ESD_RECOVERY 0x01
#define GTP_FL_READ_REPAIR 0x02

#define GTP_BAK_REF_SEND 0
#define GTP_BAK_REF_STORE 1
#define CFG_LOC_DRVA_NUM 29
#define CFG_LOC_DRVB_NUM 30
#define CFG_LOC_SENS_NUM 31

#define GTP_CHK_FW_MAX 40
#define GTP_CHK_FS_MNT_MAX 300
#define GTP_BAK_REF_PATH "/data/gtp_ref.bin"
#define GTP_MAIN_CLK_PATH "/data/gtp_clk.bin"
#define GTP_RQST_CONFIG 0x01
#define GTP_RQST_BAK_REF 0x02
#define GTP_RQST_RESET 0x03
#define GTP_RQST_MAIN_CLOCK 0x04
#define GTP_RQST_RESPONDED 0x00
#define GTP_RQST_IDLE 0xFF

//******************** For GT9XXF End **********************//
// Registers define
#define GTP_READ_COOR_ADDR 0x814E
#define GTP_REG_SLEEP 0x8040
#define GTP_REG_SENSOR_ID 0x814A
#define GTP_REG_CONFIG_DATA 0x8050
#define GTP_REG_VERSION 0x8140

#define RESOLUTION_LOC 3
#define TRIGGER_LOC 8
#define X2Y_LOC (1 << 3)

#define CFG_GROUP_LEN(p_cfg_grp) (sizeof(p_cfg_grp) / sizeof(p_cfg_grp[0]))

//***************************PART1:ON/OFF define*******************************

#define GTP_DEBUG_ON 0
#define GTP_DEBUG_ARRAY_ON 0
#define GTP_DEBUG_FUNC_ON 0
// Log define
#define GTP_INFO(fmt, arg...) printf("<<-GTP-INFO->> " fmt "\r\n", ##arg)
#define GTP_ERROR(fmt, arg...) printf("<<-GTP-ERROR->> " fmt "\r\n", ##arg)
#define GTP_DEBUG(fmt, arg...)                                    \
  do                                                              \
  {                                                               \
    if (GTP_DEBUG_ON)                                             \
      printf("<<-GTP-DEBUG->> [%d]" fmt "\r\n", __LINE__, ##arg); \
  } while (0)

#define GTP_DEBUG_ARRAY(array, num)        \
  do                                       \
  {                                        \
    int32_t i;                             \
    uint8_t *a = array;                    \
    if (GTP_DEBUG_ARRAY_ON)                \
    {                                      \
      printf("<<-GTP-DEBUG-ARRAY->>\r\n"); \
      for (i = 0; i < (num); i++)          \
      {                                    \
        printf("%02x   ", (a)[i]);         \
        if ((i + 1) % 10 == 0)             \
        {                                  \
          printf("\n");                    \
        }                                  \
      }                                    \
      printf("\n");                        \
    }                                      \
  } while (0)

#define GTP_DEBUG_FUNC()                                                \
  do                                                                    \
  {                                                                     \
    if (GTP_DEBUG_FUNC_ON)                                              \
      printf("<<-GTP-FUNC->> Func:%s@Line:%d\r\n", __func__, __LINE__); \
  } while (0)

#define GTP_SWAP(x, y) \
  do                   \
  {                    \
    typeof(x) z = x;   \
    x = y;             \
    y = z;             \
  } while (0)

//*****************************End of Part III********************************
int8_t GTP_Reset_Guitar(void);
int32_t GTP_Read_Version(void);
void GTP_IRQ_Disable(void);
void GTP_IRQ_Enable(void);
int32_t GTP_Init_Panel(void);
int8_t GTP_Send_Command(uint8_t command);
void GTP_TouchProcess(void);
void Get_Touch_XY(int16_t *x, int16_t *y);
bool Check_finger(void);

#endif /* _GOODIX_GT9XX_H_ */
