/*
 * ILI9488.h
 *
 *  Created on: 2018/07/05
 *      Author: Haru
 */

#ifndef ILI9488_H_
#define ILI9488_H_

#include <stdint.h>
#include "stm32f4xx.h"
#include "string.h"
#include "stdbool.h"
#include "stdlib.h"

#define FSMC_Ax 6
#define FSMC_NEx 4
#define LCD_BL_PORT GPIOC
#define LCD_BL_PIN GPIO_PIN_2
#define FSMC_ADDRESS (0x60000000 + ((FSMC_NEx - 1) << 26))
#define LCD_CMD_ADDR (FSMC_ADDRESS)

#define LCD_DATA_ADDR (FSMC_ADDRESS | 1 << (FSMC_Ax + 1))
#define LCD_CMD (*((volatile uint16_t *)LCD_CMD_ADDR))
#define LCD_DATA (*((volatile uint16_t *)LCD_DATA_ADDR))

#define LCD_ON LCD_BL_PORT->BSRR = (uint32_t)LCD_BL_PIN
#define LCD_OFF LCD_BL_PORT->BRR = (uint32_t)LCD_BL_PIN

//LCD閲嶈?鍙傛暟闆?
/////////////////////////////////////鐢ㄦ埛閰嶇疆鍖?///////////////////////////////////
#define wramcmd 0x2C        //寮€濮嬪啓gram鎸囦护
#define rramcmd 0x2E        //寮€濮嬭?gram鎸囦护
#define setxcmd 0x2A        //璁剧疆x鍧愭爣鎸囦护
#define setycmd 0x2B        //璁剧疆y鍧愭爣鎸囦护
#define USE_HORIZONTAL 3    //瀹氫箟娑叉櫠灞忛『鏃堕拡鏃嬭浆鏂瑰悜 	0-0搴︽棆杞?紝1-90搴︽棆杞?紝2-180搴︽棆杞?紝3-270搴︽棆杞?
#define LCD_USE8BIT_MODEL 0 //瀹氫箟鏁版嵁鎬荤嚎鏄?惁浣跨敤8浣嶆ā寮? 0,浣跨敤16浣嶆ā寮?.1,浣跨敤8浣嶆ā寮?
#define LCD_W 320           //LCD 瀹藉害
#define LCD_H 480           //LCD 楂樺害
#define id 0x9488           //LCD ID
#define dir 0               /*direction:0-0 degree \
                                      1-90 degree  \
                                      2-180 degree \
                                      3-270 degree \
                             */

//鐢荤瑪棰滆壊
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40      //妫曡壊
#define BRRED 0XFC07      //妫曠孩鑹?
#define GRAY 0X8430       //鐏拌壊
#define DARKBLUE 0X01CF   //娣辫摑鑹?
#define LIGHTBLUE 0X7D7C  //娴呰摑鑹?
#define GRAYBLUE 0X5458   //鐏拌摑鑹?
#define LIGHTGREEN 0X841F //娴呯豢鑹?
#define LIGHTGRAY 0XEF5B  //娴呯伆鑹?(PANNEL)
#define LGRAY 0XC618      //娴呯伆鑹?(PANNEL),绐椾綋鑳屾櫙鑹?
#define LGRAYBLUE 0XA651  //娴呯伆钃濊壊(涓?棿灞傞?鑹?)
#define LBBLUE 0X2B12     //娴呮?钃濊壊(閫夋嫨鏉＄洰鐨勫弽鑹?)

static const uint16_t LCD_X = 480;
static const uint16_t LCD_Y = 320;

#define YF(a) (LCD_Y - a)

void ILI9488_init(void);
uint16_t LCD_read(void);
void LCD_Clear(uint16_t Color);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color); //鐢荤偣
uint16_t LCD_ReadPoint(uint16_t x, uint16_t y);             //璇荤偣
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd);
uint16_t LCD_RD_DATA(void); //璇诲彇LCD鏁版嵁
void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
void ILI9488_writeCmd(uint16_t data);
void ILI9488_writeData(uint16_t data);
void LCD_ReadReg(uint16_t LCD_Reg, uint8_t *Rval, int n);
void LCD_WriteRAM_Prepare(void);
void LCD_ReadRAM_Prepare(void);
void Lcd_WriteData_16Bit(uint16_t Data);
uint16_t Lcd_ReadData_16Bit(void);
void LCD_direction(uint8_t direction);
uint16_t LCD_Read_ID(void);

#endif /* ILI9488_H_ */
