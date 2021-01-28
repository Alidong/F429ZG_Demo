#ifndef __DHT11_H
#define __DHT11_H
#include "stm32f4xx.h"
typedef enum
{
  DHT11_OK = 0x00U,
  DHT11_ERROR = 0x01U,
} DHT11_Status;
typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
  float   humi;
  float   temp;
		                 
} DHT11_Data_TypeDef;

//IO设置
#define DHT11_PORT GPIOG
#define DHT11_PIN GPIO_PIN_9

#define DHT11_Dout_1 (DHT11_PORT->BSRR = DHT11_PIN)                    //数据端口	PG9
#define DHT11_Dout_0 (DHT11_PORT->BSRR = (uint32_t)DHT11_PIN << 16U) //数据端口	PG9
#define DHT11_DQ_READ_PIN() HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)          //(DHT11_PORT->IDR & DHT11_PIN)  //数据端口	PG9

void DHT11_Init(void);                                 //初始化DHT11
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data);

#endif
