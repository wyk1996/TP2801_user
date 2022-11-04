#ifndef __Uart_H__
#define __Uart_H__

#include "main.h"

#define NB_UART          0x00
#define USB_UART         0x01
#define RS485_UART         0x02

#define wireless_pin_port     GPIOC
#define wireless_rx_pin       LL_GPIO_PIN_2     //ģ�鴮��
#define wireless_tx_pin       LL_GPIO_PIN_3

#define usb_rx_pin            LL_GPIO_PIN_2     //USB����
#define usb_tx_pin            LL_GPIO_PIN_3
#define usb_pin_port          GPIOA

#define rs485_rx_pin            LL_GPIO_PIN_0     //RS485����
#define rs485_tx_pin            LL_GPIO_PIN_1
#define rs485_pin_port          GPIOA

void Uartx_Init(UART_Type* UARTx,uint32_t baud);
void DebugInit(void);

void uart_exti_sleep_mode(uint8_t uart_id);      //���ڴ�����ģʽ�˳�
void uart_enter_sleep_mode(uint8_t uart_id);     //���ڽ�������ģʽ

//void SysTick_Handler(void);

//#ifdef DEBUG_PRINT
//    #define PRINT   printf
//#else
//    #define PRINT(...)
//#endif    

#endif
