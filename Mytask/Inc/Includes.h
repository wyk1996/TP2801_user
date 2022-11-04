#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

/*
*********************************************************************************************************
*                                         标准库
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>	
#include  <stdlib.h>
#include  <math.h>
#include  <string.h>

/*
*********************************************************************************************************
*                                           FreeRTOS
*********************************************************************************************************
*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "semphr.h"
#include "event_groups.h"
#include "user_init.h"
#include "cmsis_os.h"
/*
*********************************************************************************************************
*                                           宏定义
*********************************************************************************************************
*/

#define DISABLE_INT()    taskENTER_CRITICAL()
#define ENABLE_INT()     taskEXIT_CRITICAL()


/*
*********************************************************************************************************
*                                           APP / BSP
*********************************************************************************************************
*/
#include "gpio.h"
#include "spi.h"
#include "usart.h"
#include "iwdg.h"
#include "adc.h"
#include "main.h"

#include "usart2.h"
#include "device.h"
#include "nb.h"
#include "JHM1200_IIC.h"
#include "hist.h"
#include "w25qxx.h"
#include "fm33lg0xx_flash.h"
//#include "spi2.h"
#include "pwr.h"
#include "tm1729.h"
#include "myiic.h"
#include "SD3078.h"
#include "modbus.h"
#include "EC20_4g.h"
#include "lora.h"
#include "AT.h"
#include "timestamp.h"
#include "MPU6050.h"
#include "ads1115.h"

extern device_t device_info;
extern uart_t uart0;
extern uart_t uart1;
extern uart_t uart4;
extern nb_ack nb;
extern MY_TIME sMyTime;
extern MPA MPAbuf;
extern LORA_databuf lora_buf;
#define GPRS   uart1
#define COM0   uart0  
#define MODBUS485   uart4


#define GET_TIME  1


//#define Trace_log(x)     {if(device_info.debug_en) uart_transmit_str(&COM0, (uint8_t *)x);}
#define Trace_log(x)     {uart_transmit_str(&COM0, (uint8_t *)x);}
#define delay_vtask(x)   vTaskDelay(x)


#endif



