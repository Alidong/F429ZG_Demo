#ifndef __I2C_TOUCH_H
#define __I2C_TOUCH_H

#include "stm32f4xx.h"

/*设定使用的电容屏IIC设备地址*/
#define GTP_ADDRESS 0xBA

#define I2CT_FLAG_TIMEOUT ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

/*I2C引脚*/
#define GTP_I2C_SCL_PIN GPIO_PIN_0
#define GTP_I2C_SCL_GPIO_PORT GPIOB

#define GTP_I2C_SDA_PIN GPIO_PIN_11
#define GTP_I2C_SDA_GPIO_PORT GPIOF

/*复位引脚*/
#define GTP_RST_GPIO_PORT GPIOC
#define GTP_RST_GPIO_PIN GPIO_PIN_13
/*中断引脚*/
#define GTP_INT_GPIO_PORT GPIOB
#define GTP_INT_GPIO_PIN GPIO_PIN_1

/*中断服务函数*/
#define GTP_IRQHandler EXTI9_5_IRQHandler

//软件IIC使用的宏
#define I2C_SCL_1() GTP_I2C_SCL_GPIO_PORT->BSRR = GTP_I2C_SCL_PIN                  /* SCL = 1 */
#define I2C_SCL_0() GTP_I2C_SCL_GPIO_PORT->BSRR = (uint32_t)GTP_I2C_SCL_PIN << 16U /* SCL = 0 */

#define I2C_SDA_1() GTP_I2C_SDA_GPIO_PORT->BSRR = GTP_I2C_SDA_PIN                  /* SDA = 1 */
#define I2C_SDA_0() GTP_I2C_SDA_GPIO_PORT->BSRR = (uint32_t)GTP_I2C_SDA_PIN << 16U /* SDA = 0 */

#define I2C_SDA_READ() HAL_GPIO_ReadPin(GTP_I2C_SDA_GPIO_PORT, GTP_I2C_SDA_PIN)
#define SDA_IN()                                      \
    {                                                 \
        GTP_INT_GPIO_PORT->MODER &= ~(3 << (11 * 2)); \
        GPIOB->MODER |= 0 << 11 * 2;                  \
    } //PB9输入模式
#define SDA_OUT()                                     \
    {                                                 \
        GTP_INT_GPIO_PORT->MODER &= ~(3 << (11 * 2)); \
        GPIOB->MODER |= 1 << 11 * 2;                  \
    } //PB9输出模式
//函数接口
void I2C_Touch_Init(void);
uint32_t I2C_WriteBytes(uint8_t ClientAddr, uint8_t *pBuffer, uint8_t NumByteToWrite);
uint32_t I2C_ReadBytes(uint8_t ClientAddr, uint8_t *pBuffer, uint16_t NumByteToRead);
void I2C_ResetChip(void);

#endif /* __I2C_TOUCH_H */
