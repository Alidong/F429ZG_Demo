/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dht11_driver.h"
#include "string.h"
#include "lvgl.h"
#include "lv_examples.h"
#include "lv_box.h"
#include "ILI9488.h"
#include "gt5xx.h"
#include "dma2d.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for GUITask */
osThreadId_t GUITaskHandle;
const osThreadAttr_t GUITask_attributes = {
    .name = "GUITask",
    .priority = (osPriority_t)osPriorityLow,
    .stack_size = 1024 * 4};
/* Definitions for DHT11Task */
osThreadId_t DHT11TaskHandle;
const osThreadAttr_t DHT11Task_attributes = {
    .name = "DHT11Task",
    .priority = (osPriority_t)osPriorityNormal,
    .stack_size = 128 * 4};
/* Definitions for Screen_DMA */
osSemaphoreId_t Screen_DMAHandle;
const osSemaphoreAttr_t Screen_DMA_attributes = {
    .name = "Screen_DMA"};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Lvgl_Task(void *argument);
void DHT11_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of Screen_DMA */
  Screen_DMAHandle = osSemaphoreNew(1, 1, &Screen_DMA_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of GUITask */
  GUITaskHandle = osThreadNew(Lvgl_Task, NULL, &GUITask_attributes);

  /* creation of DHT11Task */
  DHT11TaskHandle = osThreadNew(DHT11_Task, NULL, &DHT11Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */
}

/* USER CODE BEGIN Header_Lvgl_Task */
/**
  * @brief  Function implementing the GUITask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_Lvgl_Task */
void Lvgl_Task(void *argument)
{
  /* USER CODE BEGIN Lvgl_Task */
  //lv_demo_widgets();
  lv_init();
  lv_port_fs_init();
  lv_port_disp_init();
  lv_port_indev_init();
  //lv_demo_widgets();
  //lv_demo_benchmark();
  lv_box_demo();
  /* Infinite loop */
  for (;;)
  {
    lv_task_handler();
    osDelay(10);
  }
  /* USER CODE END Lvgl_Task */
}

/* USER CODE BEGIN Header_DHT11_Task */
/**
* @brief Function implementing the DHT11Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DHT11_Task */
void DHT11_Task(void *argument)
{
  /* USER CODE BEGIN DHT11_Task */
  DHT11_Data_TypeDef DHT11_Data;
  /* Infinite loop */
  for (;;)
  {
    osDelay(1000);
  }
  /* USER CODE END DHT11_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
