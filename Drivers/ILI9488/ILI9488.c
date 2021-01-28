/*
 * ILI9488.c
 *
 *  Created on: 2018/07/05
 *      Author: Haru
 */
#include "ILI9488.h"
#include "stdio.h"

static uint16_t width, height;
uint16_t POINT_COLOR = BLACK; //榛樿?绾㈣壊
uint16_t BACK_COLOR = WHITE;  //鑳屾櫙棰滆壊.榛樿?涓虹櫧鑹?
void ILI9488_writeCmd(uint16_t cmd)
{
	LCD_CMD = cmd;
}

void ILI9488_writeData(uint16_t data)
{
	LCD_DATA = data;
}

uint16_t ILI9488_readData(void)
{
	uint16_t tmp = LCD_DATA;
	return tmp;
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue)
{
	LCD_CMD = LCD_Reg;		 //鍐欏叆瑕佸啓鐨勫瘎瀛樺櫒搴忓彿
	LCD_DATA = LCD_RegValue; //鍐欏叆鏁版嵁
}

/*****************************************************************************
 * @name       :uint16_t LCD_ReadReg(uint16_t LCD_Reg)
 * @date       :2018-11-13 
 * @function   :read value from specially registers
 * @parameters :LCD_Reg:Register address
 * @retvalue   :read value
******************************************************************************/
void LCD_ReadReg(uint16_t LCD_Reg, uint8_t *Rval, int n)
{
	ILI9488_writeCmd(LCD_Reg);
	while (n--)
	{
		*(Rval++) = ILI9488_readData();
	}
}

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
	ILI9488_writeCmd(wramcmd);
}
/*****************************************************************************
 * @name       :void LCD_ReadRAM_Prepare(void)
 * @date       :2018-11-13 
 * @function   :Read GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void LCD_ReadRAM_Prepare(void)
{
	ILI9488_writeCmd(rramcmd);
}

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(uint16_t Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/
void Lcd_WriteData_16Bit(uint16_t Data)
{
#if LCD_USE8BIT_MODEL
	LCD->LCD_RAM = Data >> 8;
	LCD->LCD_RAM = Data; //鍐欏崄鍏?綅棰滆壊鍊?
#else
	LCD_DATA = Data; //鍐欏崄鍏?綅棰滆壊鍊?
#endif
}

uint16_t Color_To_565(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

/*****************************************************************************
 * @name       :uint16_t Lcd_ReadData_16Bit(void)
 * @date       :2018-11-13 
 * @function   :Read an 16-bit value from the LCD screen
 * @parameters :None
 * @retvalue   :read value
******************************************************************************/
uint16_t Lcd_ReadData_16Bit(void)
{
	uint16_t r, g, b;
	//dummy data
	r = ILI9488_readData();
	//delay_us(1);//寤舵椂1us
	//8bit:red data
	//16bit:red and green data
	r = ILI9488_readData();
	//delay_us(1);//寤舵椂1us
	//8bit:green data
	//16bit:blue data
	g = ILI9488_readData();
#if LCD_USE8BIT_MODEL
	delay_us(1); //寤舵椂1us
	//8bit:blue data
	b = ILI9488_readData();
#else
	b = g >> 8;
	g = r & 0xFF;
	r = r >> 8;
#endif
	return Color_To_565(r, g, b);
}
/*****************************************************************************
 * @name       :void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd)
{
	ILI9488_writeCmd(setxcmd);
	ILI9488_writeData(xStar >> 8);
	ILI9488_writeData(0x00FF & xStar);
	ILI9488_writeData(xEnd >> 8);
	ILI9488_writeData(0x00FF & xEnd);

	ILI9488_writeCmd(setycmd);
	ILI9488_writeData(yStar >> 8);
	ILI9488_writeData(0x00FF & yStar);
	ILI9488_writeData(yEnd >> 8);
	ILI9488_writeData(0x00FF & yEnd);

	LCD_WriteRAM_Prepare(); //寮€濮嬪啓鍏?RAM
}

/*****************************************************************************
 * @name       :void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	LCD_SetWindows(Xpos, Ypos, Xpos, Ypos);
}
/*****************************************************************************
 * @name       :void LCD_DrawPoint(uint16_t x,uint16_t y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
	Lcd_WriteData_16Bit(color);
	LCD_SetCursor(x, y); //璁剧疆鍏夋爣浣嶇疆
}

/*****************************************************************************
 * @name       :uint16_t LCD_ReadPoint(uint16_t x,uint16_t y)
 * @date       :2018-11-13 
 * @function   :Read a pixel color value at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :the read color value
******************************************************************************/
uint16_t LCD_ReadPoint(uint16_t x, uint16_t y)
{
	uint16_t color;
	if (x >= width || y >= height)
	{
		return 0; //瓒呰繃浜嗚寖鍥?,鐩存帴杩斿洖
	}
	LCD_SetCursor(x, y); //璁剧疆鍏夋爣浣嶇疆
	LCD_ReadRAM_Prepare();
	color = Lcd_ReadData_16Bit();
	return color;
}

/*****************************************************************************
 * @name       :void LCD_Clear(uint16_t Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/
void LCD_Clear(uint16_t Color)
{
	unsigned int i, times; //,m;
	uint32_t total_point = width * height;
	LCD_SetWindows(0, 0, width - 1, height - 1);
	for (i = 0; i < total_point; i++)
	{
#if LCD_USE8BIT_MODEL
		LCD->LCD_RAM = Color >> 8;
		LCD->LCD_RAM = Color;
#else
		LCD_DATA = Color;
#endif
	}
}

/*****************************************************************************
 * @name       :void LCD_direction(uint8_t direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/
void LCD_direction(uint8_t direction)
{
	switch (direction)
	{
	case 0:
		width = LCD_W;
		height = LCD_H;
		LCD_WriteReg(0x36, (1 << 3));
		break;
	case 1:
		width = LCD_H;
		height = LCD_W;
		LCD_WriteReg(0x36, (1 << 3) | (1 << 6) | (1 << 5));
		break;
	case 2:
		width = LCD_W;
		height = LCD_H;
		LCD_WriteReg(0x36, (1 << 3) | (1 << 6) | (1 << 7));
		break;
	case 3:
		width = LCD_H;
		height = LCD_W;
		LCD_WriteReg(0x36, (1 << 3) | (1 << 5) | (1 << 7));
		break;
	default:
		break;
	}
}

/*****************************************************************************
 * @name       :uint16_t LCD_Read_ID(void)
 * @date       :2018-11-13 
 * @function   :Read ID
 * @parameters :None
 * @retvalue   :ID value
******************************************************************************/
uint16_t LCD_Read_ID(void)
{
	uint8_t val[4] = {0};
	LCD_ReadReg(0xD3, val, 4);
	return (val[2] << 8) | val[3];
}

void ILI9488_init(void)
{
	uint16_t ID = LCD_Read_ID();
	printf("LCD_ID=0x%X\r\n", ID);
	//************* ILI9488鍒濆?鍖?**********//
	ILI9488_writeCmd(0XF7);
	ILI9488_writeData(0xA9);
	ILI9488_writeData(0x51);
	ILI9488_writeData(0x2C);
	ILI9488_writeData(0x82);

	ILI9488_writeCmd(0xC0);
	ILI9488_writeData(0x11);
	ILI9488_writeData(0x09);

	ILI9488_writeCmd(0xC1);
	ILI9488_writeData(0x41);

	ILI9488_writeCmd(0XC5);
	ILI9488_writeData(0x00);
	ILI9488_writeData(0x0A);
	ILI9488_writeData(0x80);

	ILI9488_writeCmd(0xB1);
	ILI9488_writeData(0xB0);
	ILI9488_writeData(0x11);

	ILI9488_writeCmd(0xB4);
	ILI9488_writeData(0x02);

	ILI9488_writeCmd(0xB6);
	ILI9488_writeData(0x02);
	ILI9488_writeData(0x22);

	ILI9488_writeCmd(0xB7);
	ILI9488_writeData(0xc6);

	ILI9488_writeCmd(0xBE);
	ILI9488_writeData(0x00);
	ILI9488_writeData(0x04);

	ILI9488_writeCmd(0xE9);
	ILI9488_writeData(0x00);

	ILI9488_writeCmd(0x36);
	ILI9488_writeData(0x08);

	ILI9488_writeCmd(0x3A);
	ILI9488_writeData(0x55);

	ILI9488_writeCmd(0xE0);
	ILI9488_writeData(0x00);
	ILI9488_writeData(0x07);
	ILI9488_writeData(0x10);
	ILI9488_writeData(0x09);
	ILI9488_writeData(0x17);
	ILI9488_writeData(0x0B);
	ILI9488_writeData(0x41);
	ILI9488_writeData(0x89);
	ILI9488_writeData(0x4B);
	ILI9488_writeData(0x0A);
	ILI9488_writeData(0x0C);
	ILI9488_writeData(0x0E);
	ILI9488_writeData(0x18);
	ILI9488_writeData(0x1B);
	ILI9488_writeData(0x0F);

	ILI9488_writeCmd(0XE1);
	ILI9488_writeData(0x00);
	ILI9488_writeData(0x17);
	ILI9488_writeData(0x1A);
	ILI9488_writeData(0x04);
	ILI9488_writeData(0x0E);
	ILI9488_writeData(0x06);
	ILI9488_writeData(0x2F);
	ILI9488_writeData(0x45);
	ILI9488_writeData(0x43);
	ILI9488_writeData(0x02);
	ILI9488_writeData(0x0A);
	ILI9488_writeData(0x09);
	ILI9488_writeData(0x32);
	ILI9488_writeData(0x36);
	ILI9488_writeData(0x0F);

	ILI9488_writeCmd(0x11);
	HAL_Delay(100);
	ILI9488_writeCmd(0x29);
	LCD_direction(USE_HORIZONTAL); //璁剧疆LCD鏄剧ず鏂瑰悜
	LCD_ON;
	LCD_Clear(BLUE); //娓呭叏灞忕櫧鑹?
					 //ILI9488_drawRect(0, 0, 480, 320, BLUE);
}
