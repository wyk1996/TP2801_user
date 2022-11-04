/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart2.h"

#if UART0_EN
uint8_t uart0_buf[UART0_RX_MAX];
uart_t  uart0; 
#endif

#if UART1_EN
uint8_t uart1_buf[UART1_RX_MAX];
uart_t  uart1; 
#endif

#if UART3_EN
uint8_t uart3_buf[UART3_RX_MAX];
uart_t  uart3; 
#endif
#if UART4_EN
uint8_t uart4_buf[UART4_RX_MAX];
uart_t  uart4; 
#endif
#if UART5_EN
uint8_t uart5_buf[UART5_RX_MAX];
uart_t  uart5; 
#endif


/* 保存 TIM定时中断到后执行的回调函数指针 */
//static void (*s_TIM_CallBack1)(void);

#if 0
static void uart_mb_sw(uint8_t states)
{
  if(states)
    LL_GPIO_SetOutputPin( rs485de_gpio,rs485de_gpio_id );
  else
    LL_GPIO_ResetOutputPin(rs485de_gpio,rs485de_gpio_id );
}
#endif


void uart_var_init(void)
{
#if UART0_EN
	uart0.uart    = UART0;
	uart0.rx_size = UART0_RX_MAX;
	uart0.p_rxbuf = uart0_buf;

	uart0.write_pos = 0;
	uart0.read_pos  = 0;
	uart0.rx_count  = 0;
	uart0.rx_end    = 0;

#endif
	
#if UART1_EN
	uart1.uart = UART1;
	uart1.rx_size = UART1_RX_MAX;
	uart1.p_rxbuf = uart1_buf;	
	
	uart1.write_pos = 0;
	uart1.read_pos  = 0;
	uart1.rx_count  = 0;
	uart1.rx_end    = 0;

#endif

#if UART3_EN
	uart3.uart = UART3;
	uart3.rx_size = UART3_RX_MAX;
	uart3.p_rxbuf = uart3_buf;	
	
	uart3.write_pos = 0;
	uart3.read_pos  = 0;
	uart3.rx_count  = 0;
	uart3.rx_end    = 0;

#endif
	
#if UART4_EN
	uart4.uart    = UART4;
	uart4.rx_size = UART4_RX_MAX;
	uart4.p_rxbuf = uart4_buf;

	uart4.write_pos = 0;
	uart4.read_pos  = 0;
	uart4.rx_count  = 0;
	uart4.rx_end    = 0;
  uart4.sw_mod    = uart_mb_sw;
#endif

#if UART5_EN
	uart5.uart    = UART5;
	uart5.rx_size = UART5_RX_MAX;
	uart5.p_rxbuf = uart5_buf;

	uart5.write_pos = 0;
	uart5.read_pos  = 0;
	uart5.rx_count  = 0;
	uart5.rx_end    = 0;
#endif
}


void uart_transmit_buf(uart_t *uart, uint8_t *tx_buf, uint16_t tx_size)
{
	uint16_t tx_count = 0;
	if(uart->uart == UART4)
	{
		uart->sw_mod(1);
	 delay_vtask(5);
	}			
	while(tx_count < tx_size)
	{
		LL_UART_TransmitData(uart->uart, tx_buf[tx_count++]);
		while(SET != LL_UART_IsActiveFlag_TXSE(uart->uart));		//等待发送完成
	}
	if(uart->uart == UART4 )
	{ 
		uart->sw_mod(0); 		
		delay_vtask(5);
	}
		
}

void uart_transmit_str(uart_t *uart, uint8_t *tx_buf )
{
	  if(uart->sw_mod != 0  & uart->uart == UART4)
		{ uart->sw_mod(1);
		
		}
	while(*tx_buf != '\0')
	{
		LL_UART_TransmitData(uart->uart, *tx_buf++);
		while(SET != LL_UART_IsActiveFlag_TXSE(uart->uart));		//等待发送完成
	}
	  if(uart->sw_mod != 0  & uart->uart == UART4 )
		{ uart->sw_mod(0); 
		}
		
}

#if 0
int fputc(int ch, FILE *f)
{
	LL_UART_TransmitData(UART0, (uint8_t) ch);
    while(SET != LL_UART_IsActiveFlag_TXSE(UART0));
    return ch;
}
#endif



uint16_t uart_get_rx_count(uart_t *uart)
{
    uint16_t rx_cnt = 0;
    if(uart == NULL)
        return 0; 
    if(uart->rx_end == 1)
    {
        taskENTER_CRITICAL();
        uart->rx_end = 0;
        rx_cnt = uart->rx_count;
        taskEXIT_CRITICAL();
    }
    return rx_cnt;
}

uint16_t uart_recv_data(uart_t *uart, uint8_t *recv_buf ,uint16_t recv_len, uint16_t wait_tick)
{
	uint16_t rx_count = 0,save_count = 0;
	
	while(wait_tick--)   /*等待串口数据接受完毕*/
	{
		if((rx_count = uart_get_rx_count(uart)) != 0)
			break;
		delay_vtask(1);
	}
	if(rx_count > 0)
	{
		memset(recv_buf, 0, recv_len);
		if(rx_count >= recv_len)
		rx_count = recv_len;
		
		for(; save_count < rx_count; save_count++)
		{
			recv_buf[save_count] = uart->p_rxbuf[uart->read_pos];
			if(++uart->read_pos >= uart->rx_size)
			{
				uart->read_pos = 0;
			}
		}
		taskENTER_CRITICAL();
		uart->rx_count -= rx_count;
		taskEXIT_CRITICAL();
		recv_buf[save_count] = '\0';
	}
	
	return rx_count;
	
}

uint8_t uart_strstr(uart_t *uart, uint8_t *recv_buf ,const char *str, uint16_t rd_size ,uint16_t wait_tick)
{
	uint16_t rx_size = 0;
	uint8_t ret = 1;
			
	while(wait_tick--)
	{
		rx_size = uart_recv_data(uart, recv_buf ,rd_size, 1 );
		if(rx_size)
		{
			Trace_log(recv_buf); ////////***打印串口1数据******///
			if(strstr((const char *)recv_buf, str) != NULL)
			{
				ret = 0;
				break;
			}
		}
	}
	return ret;
}


/*****************************
函数会把接收的在串口1打印
参数1发送串口
参数2.发送数组
参数3.接收数组
参数4.接收匹配数据
参数5。接收长度
参数6等等循环接收
******************************/
uint8_t uart_send_ack(uart_t *uart, void *send_buf, uint8_t *recv_buf ,const char *str, uint16_t rd_size ,uint16_t wait_tick)
{
	uart_transmit_str(uart, send_buf);
	
	return uart_strstr(uart, recv_buf, str, rd_size, wait_tick);
}

uint8_t uart_send_buff_ack(uart_t *uart , void *send_buf , uint16_t send_size, uint8_t *recv_buf ,uint16_t  rd_size ,const char *str ,uint16_t wait_tick)
{
 
	if(send_size !=0)
	{
	 uart_transmit_buf(uart , send_buf ,send_size);
	}
	else 
	{
	uart_transmit_str(uart, send_buf);
	}
	
	return uart_strstr(uart, recv_buf, str, rd_size, wait_tick);
}




void uart_clear_recv(uart_t *uart)
{
	uart->read_pos  = 0;
	uart->write_pos = 0;
	uart->rx_end    = 0;
	uart->rx_count  = 0;
}

void uart_isr_recv(uart_t *uart)
{
	uint8_t rd;
	rd = LL_UART_ReceiveData(uart->uart);
	if(uart->rx_count < uart->rx_size)
	{
		uart->rx_end = 0;
		uart->rx_count++;
		uart->p_rxbuf[uart->write_pos] = rd;
		
		if(++uart->write_pos >= uart->rx_size)
			uart->write_pos = 0;
	}
}
void uart_isr_rx_timeout(uart_t *uart)
{
	uart->rx_end = 1;
}


