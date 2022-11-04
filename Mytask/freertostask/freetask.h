#ifndef _FREETASK_H_
#define _FREETASK_H_

#include "main.h"

extern	xTaskHandle device_usbHandle;   //usb   256     6
extern	xTaskHandle device_flashHandle; //W_FLASH   1024        4
//extern	xTaskHandle rtc_displayHandle;  //RTC &  display  1024   3
extern	xTaskHandle sensorHandle;  //������  512     2
extern	xTaskHandle sleepHandle;   //����    256     7  // 1
extern	xTaskHandle sever_nbHandle;//NB      512     5
//extern	xTaskHandle mykeyHandle;   //����    256     1  // 7
//extern	xTaskHandle modbusHandle;   //����    256     1  // 7

//extern SemaphoreHandle_t flash_semap;


void device_usbTask(void *pvParameters);
void flashTask(void *pvParameters);
//void rtc_displayTask(void *pvParameters);
void sensorTask(void *pvParameters);
void sleepTask(void *pvParameters);
void severTask(void *pvParameters);
void modbustask(void *pvParameters);


void MX_FREERTOS_Init(void);
//void key_Callback(void);

#endif 
