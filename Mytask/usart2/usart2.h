/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart2_H
#define __usart2_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

//extern void _Error_Handler(char *, int);
/*
UART0     主串口，程序烧录口，调试接口
UART1     通讯模块接口
UART3     未使用
UART4     RS485接口
USRT5     外部TTL接口，与IIC2传感器读取接口复用，默认使用IIC2功能
*/
#define UART0_EN  1
#define UART0_RX_MAX   256

#define UART1_EN  1
#define UART1_RX_MAX   400

#define UART3_EN  0
#define UART3_RX_MAX   256

#define UART4_EN  0
#define UART4_RX_MAX   128

#define UART5_EN  0
#define UART5_RX_MAX   256

typedef struct
{
	UART_Type *uart;
  uint8_t *p_rxbuf;
	 void (*sw_mod)(uint8_t);
	uint16_t rx_size;
	
	volatile uint16_t read_pos;
	volatile uint16_t write_pos;
	volatile uint16_t rx_count;
	volatile uint16_t rx_end;
}uart_t;





void uart_hard_init(void);

void hard_deinit(UART_Type * usart);

void uart_var_init(void);

void uart_init( void );

void uart_deinit( void );

void uart_transmit_buf(uart_t *uart, uint8_t *tx_buf, uint16_t tx_size);

void uart_transmit_str(uart_t *uart, uint8_t *tx_buf );

uint16_t uart_get_rx_count(uart_t *uart);

uint16_t uart_recv_data(uart_t *uart, uint8_t *recv_buf ,uint16_t recv_len, uint16_t wait_tick);

uint8_t uart_strstr(uart_t *uart, uint8_t *recv_buf ,const char *str, uint16_t rd_size ,uint16_t wait_tick);

uint8_t uart_send_ack(uart_t *uart, void *send_buf, uint8_t *recv_buf ,const char *str, uint16_t rd_size ,uint16_t wait_tick);
uint8_t uart_send_buff_ack(uart_t *uart , void *send_buf , uint16_t send_size, uint8_t *recv_buf ,uint16_t  rd_size ,const char *str ,uint16_t wait_tick);

void uart_clear_recv(uart_t *uart);

void uart_isr_recv(uart_t *uart);

void uart_isr_rx_timeout(uart_t *uart);

void uart_it_callback(void);


#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */
