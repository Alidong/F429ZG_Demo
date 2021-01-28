/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
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

#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */
static uint8_t buffer[_MAX_SS];   /* a work buffer for the f_mkfs() */
FRESULT res;                      /* FatFs function common result code */
uint32_t byteswritten, bytesread; /* File write/read counts */
uint8_t wtext[] = "Hello world";  /* File write buffer */
uint8_t rtext[100];
FIL MyFile;
/* USER CODE END Variables */    

void MX_FATFS_Init(void) 
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC 
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */  
}

/* USER CODE BEGIN Application */
void fatfs_test(void)
{
  /* additional user code for init */
  /*##-2- Register the file system object to the FatFs module ##############*/
  if (f_mount(&SDFatFS, (TCHAR const *)SDPath, 0) != FR_OK)
  {
    /* FatFs Initialization Error */
    printf("mount err ! \r\n", f_mount(&SDFatFS, (TCHAR const *)SDPath, 0));
    Error_Handler();
  }
  else
  {
    /*##-3- Create a FAT file system (format) on the logical drive #########*/
    if (f_mkfs((TCHAR const *)SDPath, FM_ANY, 0, buffer, sizeof(buffer)) != FR_OK)
    {
      printf("mkdir err=%d\r\n",f_mkfs((TCHAR const *)SDPath, FM_ANY, 0, buffer, sizeof(buffer)));
      Error_Handler();
    }
    else
    {
      /*##-4- Create and Open a new text file object with write access #####*/
      if (f_open(&MyFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
      {
        /* 'STM32.TXT' file Open for write Error */
        printf("Open err \r\n");
        Error_Handler();
      }
      else
      {
        /*##-5- Write data to the text file ################################*/
        res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);

        if ((byteswritten == 0) || (res != FR_OK))
        {
          /* 'STM32.TXT' file Write or EOF Error */
          Error_Handler();
        }
        else
        {
          /*##-6- Close the open text file #################################*/
          f_close(&MyFile);

          /*##-7- Open the text file object with read access ###############*/
          if (f_open(&MyFile, "STM32.TXT", FA_READ) != FR_OK)
          {
            /* 'STM32.TXT' file Open for read Error */
            Error_Handler();
          }
          else
          {
            /*##-8- Read data from the text file ###########################*/
            res = f_read(&MyFile, rtext, sizeof(rtext), (UINT *)&bytesread);

            if ((bytesread == 0) || (res != FR_OK)) /* EOF or Error */
            {
              /* 'STM32.TXT' file Read or EOF Error */
              Error_Handler();
            }
            else
            {
              /*##-9- Close the open text file #############################*/
              f_close(&MyFile);

              /*##-10- Compare read data with the expected data ############*/
              if ((bytesread != byteswritten))
              {
                /* Read data is different from the expected data */
                Error_Handler();
              }
              else
              {
                /* Success of the demo: no error occurrence */
              }
            }
          }
        }
      }
    }
  }
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
