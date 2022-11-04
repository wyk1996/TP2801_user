/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 FMSH.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by FM under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  <math.h>

#include "mf_config.h"    
    
#include "fm33lc0xx_ll_rcc.h"
#include "fm33lc0xx_ll_lptim.h"
#include "fm33lc0xx_ll_lpuart.h"
#include "fm33lc0xx_ll_gpio.h"
#include "fm33lc0xx_ll_uart.h"
#include "fm33lc0xx_ll_vref.h"
#include "fm33lc0xx_ll_iwdt.h"
#include "fm33lc0xx_ll_wwdt.h"
#include "fm33lc0xx_ll_pmu.h"
#include "fm33lc0xx_ll_flash.h"
#include "fm33lc0xx_ll_svd.h"
#include "fm33lc0xx_ll_aes.h"
#include "fm33lc0xx_ll_rmu.h"
#include "fm33lc0xx_ll_rng.h"
#include "fm33lc0xx_ll_opa.h"
#include "fm33lc0xx_ll_comp.h"
#include "fm33lc0xx_ll_hdiv.h"
#include "fm33lc0xx_ll_i2c.h"
#include "fm33lc0xx_ll_spi.h"
#include "fm33lc0xx_ll_u7816.h"
#include "fm33lc0xx_ll_bstim.h"
#include "fm33lc0xx_ll_gptim.h"
#include "fm33lc0xx_ll_atim.h"
#include "fm33lc0xx_ll_crc.h"
#include "fm33lc0xx_ll_dma.h"
#include "fm33lc0xx_ll_rtc.h"
#include "fm33lc0xx_ll_lcd.h"



#include "user_init.h"
#include "taskSystem.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
//#include "semphr.h"
#include "adc.h"
//#include "bstim32.h"
//#include "lptim32.h"
#include "uart.h"
#include "gpio.h"
#include "usart2.h"
#include "device.h"
#include "SD3078.h"
#include "spi.h"
#include "hist.h"
#include "w25qxx.h"
#include "freetask.h"
#include "LCDdisplay.h"
#include "nb.h"
#include "lora.h"
#include "modbus.h"
#include "EC20_4g.h"
#include "AT.h"
#include "timestamp.h"
//#include "MPU6050.h"
#include "JHM1200_IIC.h"
//#include "myiic.h"
//#include "ads1115.h"
#include "pwr.h"
#include "flash.h"
//#include "rtc.h"
#include "lorawan_yxd.h"
//#include "QMA7981.h"
#include "DS18B20.h"
#include "CS5560.h"

#if defined(USE_FULL_ASSERT)
#include "fm33_assert.h"
#endif /* USE_FULL_ASSERT */


#define TRUE 0
#define FALSE 1
  
#define gpio_input 0
#define gpio_output 1

extern uart_t uart0;
extern uart_t uart1;
//extern uart_t uart4;
#define GPRS   uart1
#define COM0   uart0  
//#define MODBUS485   uart4

#define Trace_log(x)     {if(device_info.debug_en) uart_transmit_str(&COM0, (uint8_t *)x);}
//#define Trace_log(x)     {uart_transmit_str(&COM0, (uint8_t *)x);}
#define delay_vtask(x)   vTaskDelay(x)
//#define DISABLE_INT()    taskENTER_CRITICAL()
//#define ENABLE_INT()     taskEXIT_CRITICAL()

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);



#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT FMSH *****END OF FILE****/
