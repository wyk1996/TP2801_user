#ifndef __Uart_H__
#define __Uart_H__

#include "main.h"

#define NB_UART          0x00
#define USB_UART         0x01
#define RS485_UART         0x02

#define wireless_pin_port     GPIOC
#define wireless_rx_pin       LL_GPIO_PIN_2     //模块串口
#define wireless_tx_pin       LL_GPIO_PIN_3

#define usb_rx_pin            LL_GPIO_PIN_2     //USB串口
#define usb_tx_pin            LL_GPIO_PIN_3
#define usb_pin_port          GPIOA

#define rs485_rx_pin            LL_GPIO_PIN_0     //RS485串口
#define rs485_tx_pin            LL_GPIO_PIN_1
#define rs485_pin_port          GPIOA

void Uartx_Init(UART_Type* UARTx,uint32_t baud);
void DebugInit(void);

void uart_exti_sleep_mode(uint8_t uart_id);      //串口从休眠模式退出
void uart_enter_sleep_mode(uint8_t uart_id);     //串口进入休眠模式

//void SysTick_Handler(void);

//#ifdef DEBUG_PRINT
//    #define PRINT   printf
//#else
//    #define PRINT(...)
//#endif    

#endif
