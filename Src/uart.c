#include "main.h"
#include "user_init.h"
#include "uart.h"
#include "taskSystem.h"


/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */
typedef struct
{
	volatile uint8_t Mode;		/* 计数器模式，1次性 */
	volatile uint8_t Flag;		/* 定时到达标志  */
	volatile uint32_t Count;	/* 计数器 */
	volatile uint32_t PreLoad;	/* 计数器预装值 */
	volatile uint32_t backcall;	/* 回调 */
	
}SOFT_TMR;

static SOFT_TMR s_tTmr;

#if 0
#pragma import(__use_no_semihosting)                          
struct __FILE 
{ 
    int handle; 
};
FILE __stdout;       

void _sys_exit(int x) 
{ 
    x = x;
} 

int fputc(int ch, FILE *f)
{
    UART0->TXBUF = ch;
    while (!(UART0->ISR & (0x1 << 0)));
    UART0->ISR |= (0x1 << 0);
    
    return ch;
}

#endif

#if 0
void Inittimer()
{
	/* 清零所有的软件定时器 */
	s_tTmr.Count = 0;
	s_tTmr.PreLoad = 0;
	s_tTmr.Flag = 0;
	s_tTmr.backcall=0;
	
//		s_tTmr.Mode = TMR_ONCE_MODE;	/* 缺省是1次性工作模式 */ 

}

void bsp_StartTimer(  uint32_t _period,SOFT_TMR *_tmr)
{

	taskENTER_CRITICAL();  			/* 关中断 */

	_tmr->Count = _period;		/* 实时计数器初值 */
	_tmr->PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
	_tmr->Flag = 0;				/* 定时时间到标志 */
	//s_tTmr.Mode = TMR_ONCE_MODE;	/* 1次性工作模式 */

	taskEXIT_CRITICAL();  				/* 开中断 */
}
/*********************************************************************************************************
*	函 数 名: bsp_SoftTimerDec
*	功能说明: 每隔1ms对所有定时器变量减1。必须被SysTick_ISR周期性调用。
*	形    参:  _tmr : 定时器变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_SoftTimerDec(void)
{
	if (s_tTmr.Count> 0)
	{
		/* 如果定时器变量减到1则设置定时器到达标志 */
		if (--s_tTmr.Count == 0)
		{
			s_tTmr.Flag = 1;
			uart_isr_rx_timeout(&MODBUS485);
      s_tTmr.backcall=1;
			/* 如果是自动模式，则自动重装计数器 */
		}
	}
}


void SysTick_ISR(void)
{	/* 每隔1ms，对软件定时器的计数器进行减一操作 */
  bsp_SoftTimerDec();
}


#endif

//void SysTick_Handler(void)
//{	
//xPortSysTickHandler();
//	SysTick_ISR();
//}

void DebugInit(void)
{ 
	Uartx_Init(UART0,115200);//初始化uart配置
	#if 0
  	LL_UART_EnableDirectionTx(UART0);
	LL_UART_EnableDirectionRx(UART0);
	#endif
}


#if UART0_EN 
void UART0_IRQHandler(void)
{
	
	//接收中断处理
	if((ENABLE == LL_UART_IsEnbaledIT_ReceiveBuffFull(UART0))
		&&(SET == LL_UART_IsActiveFlag_RXBF(UART0)))
	{
		uart_isr_recv(&uart0);
	}
	
  if(ENABLE == LL_UART_IsActiveFlag_RXTO(UART0))
	{
		LL_UART_ClearFlag_RXTO(UART0);
		uart_isr_rx_timeout(&COM0);		
	}
}
#endif

#if UART1_EN 
void UART1_IRQHandler(void)
{	
	//接收中断处理
	if((ENABLE == LL_UART_IsEnbaledIT_ReceiveBuffFull(UART1))
		&&(SET == LL_UART_IsActiveFlag_RXBF(UART1)))
	{
		uart_isr_recv(&uart1);
	}
	
	if(ENABLE == LL_UART_IsActiveFlag_RXTO(UART1))
	{
		LL_UART_ClearFlag_RXTO(UART1);
		uart_isr_rx_timeout(&GPRS);		
	}
}
#endif


#if UART4_EN
void UART4_IRQHandler(void)
{
	uint8_t tmp08;
	
	if((ENABLE == LL_UART_IsEnbaledIT_ReceiveBuffFull(UART4))
		&&(SET == LL_UART_IsActiveFlag_RXBF(UART4)))
	{
		//中断转发接收到的数据
		uart_isr_recv(&uart4);
		bsp_StartTimer(50,&s_tTmr);
	}
	if((ENABLE == LL_UART_IsActiveFlag_RXTO(UART4)))
	{
//		uart_isr_rx_timeout(&RS232);
		LL_UART_ClearFlag_RXTO(UART4);
	}
}
#endif

#if UART5_EN
void UART5_IRQHandler(void)
{
	uint8_t tmp08;
	
	//接收中断处理
	if((ENABLE == LL_UART_IsEnabledIT_RXBuffFull(UART5))
		&&(SET == LL_UART_IsActiveFlag_RXBuffFull(UART5)))
	{
		//中断转发接收到的数据
		tmp08 = LL_UART_ReadRXBuff(UART5);//接收中断标志可通过读取rxreg寄存器清除
		LL_UART_WriteTXBuff(UART5, tmp08);
	}
	
}
#endif

void Uartx_Init(UART_Type* UARTx,uint32_t baud)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	LL_UART_InitTypeDef UART_InitStruct = {0};   

	UART_InitStruct.BaudRate            = baud;							        //波特率
	UART_InitStruct.DataWidth           = LL_UART_DATAWIDTH_8B;			      	//数据位数
	UART_InitStruct.StopBits            = LL_UART_STOPBITS_1;				//停止位
	UART_InitStruct.Parity              = LL_UART_PARITY_NONE;				        //奇偶校验
	UART_InitStruct.TransferDirection   = LL_UART_DIRECTION_TX_RX;	                //接收-发送使能
	UART_InitStruct.InfraredModulation = DISABLE;
	UART_InitStruct.ClockSrc           = LL_RCC_UART_OPERATION_CLOCK_SOURCE_APBCLK1;

	switch((uint32_t)UARTx) {

		case UART0_BASE:
			#if 0
			//PA13:UART0-RX   PA14:UART0-TX
			GPIO_InitStruct.Pin        = LL_GPIO_PIN_13|LL_GPIO_PIN_14;
			GPIO_InitStruct.Mode       = LL_GPIO_MODE_DIGITAL;
			GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;         //推挽输出
			GPIO_InitStruct.Pull       = ENABLE;
			GPIO_InitStruct.RemapPin   = DISABLE;
			LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
			#else
			//PA2:UART0-RX   PA3:UART0-TX
			GPIO_InitStruct.Pin         = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
			GPIO_InitStruct.Mode        = LL_GPIO_MODE_DIGITAL;
			GPIO_InitStruct.OutputType  = LL_GPIO_OUTPUT_PUSHPULL;
			GPIO_InitStruct.Pull        = DISABLE;
			GPIO_InitStruct.RemapPin    = DISABLE;
			LL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
			#endif
		
			LL_UART_Init(UARTx, &UART_InitStruct);
			/*NVIC中断配置*/
			LL_UART_Enable_ReceiveTimeOut(UART0);
			LL_UART_EnableIT_ReceiveTimeOut(UART0);
			LL_UART_SetRxTimeOut(UART0,50);
			LL_UART_ClearFlag_RXTO(UART0);
			
			NVIC_DisableIRQ(UART0_IRQn);
			NVIC_SetPriority(UART0_IRQn,2);//中断优先级配置
			NVIC_EnableIRQ(UART0_IRQn);
			break;
		
		case UART1_BASE:
			#if 0
			//PB13:UART1-RX   PB14:UART1-TX
			GPIO_InitStruct.Pin         = LL_GPIO_PIN_13|LL_GPIO_PIN_14;
			GPIO_InitStruct.Mode        = LL_GPIO_MODE_DIGITAL;
			GPIO_InitStruct.OutputType  = LL_GPIO_OUTPUT_PUSHPULL;
			GPIO_InitStruct.Pull        = ENABLE;
			GPIO_InitStruct.RemapPin    = DISABLE;			
			LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
			#else
			//PC2:UART1-RX   PC3:UART1-TX
			GPIO_InitStruct.Pin        = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
			GPIO_InitStruct.Mode       = LL_GPIO_MODE_DIGITAL;
			GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
			GPIO_InitStruct.Pull       = DISABLE;
			GPIO_InitStruct.RemapPin   = DISABLE;
			LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
			#endif	
				
			LL_UART_Init(UARTx, &UART_InitStruct);
			/*NVIC中断配置*/
			LL_UART_Enable_ReceiveTimeOut(UART1);
			LL_UART_EnableIT_ReceiveTimeOut(UART1);
			LL_UART_SetRxTimeOut(UART1,200);
			LL_UART_ClearFlag_RXTO(UART1);
			
			NVIC_DisableIRQ(UART1_IRQn);
			NVIC_SetPriority(UART1_IRQn,2);//中断优先级配置
			NVIC_EnableIRQ(UART1_IRQn);
			break;
		
		
		case UART4_BASE:
			#if 0
			//PB2:UART4-RX   PB3:UART4-TX
			GPIO_InitStruct.Pin          = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
			GPIO_InitStruct.Mode         = LL_GPIO_MODE_DIGITAL;
			GPIO_InitStruct.OutputType   = LL_GPIO_OUTPUT_PUSHPULL;
			GPIO_InitStruct.Pull         = DISABLE;
			GPIO_InitStruct.RemapPin     = DISABLE;			
			LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
			LL_UART_Init(UARTx, &UART_InitStruct);
			#else
			//PA0:UART4-RX   PA1:UART4-TX
			GPIO_InitStruct.Pin            = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
			GPIO_InitStruct.Mode           = LL_GPIO_MODE_DIGITAL;
			GPIO_InitStruct.OutputType     = LL_GPIO_OUTPUT_PUSHPULL;
			GPIO_InitStruct.Pull           = DISABLE;
			GPIO_InitStruct.RemapPin       = DISABLE;
			LL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
			LL_UART_Init(UARTx, &UART_InitStruct);
		  	#endif
        
            /*NVIC中断配置*/
			NVIC_DisableIRQ(UART4_IRQn);
			NVIC_SetPriority(UART4_IRQn,2);//中断优先级配置
			NVIC_EnableIRQ(UART4_IRQn);
			break;
		
		case UART5_BASE:
			#if 0
			//PD0:UART5-RX   PD1:UART5-TX
			GPIO_InitStruct.Pin            = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
			GPIO_InitStruct.Mode           = LL_GPIO_MODE_DIGITAL;
			GPIO_InitStruct.OutputType     = LL_GPIO_OUTPUT_PUSHPULL;
			GPIO_InitStruct.Pull           = DISABLE;
			GPIO_InitStruct.RemapPin       = DISABLE;			
			LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
			#else 
			//PC4:UART5-RX   PC5:UART5-TX
			GPIO_InitStruct.Pin           = LL_GPIO_PIN_4|LL_GPIO_PIN_5;
			GPIO_InitStruct.Mode          = LL_GPIO_MODE_DIGITAL;
			GPIO_InitStruct.OutputType    = LL_GPIO_OUTPUT_PUSHPULL;
			GPIO_InitStruct.Pull          = DISABLE;
			GPIO_InitStruct.RemapPin      = DISABLE;
			LL_GPIO_Init(GPIOC, &GPIO_InitStruct);	
			#endif
      		LL_UART_Init(UARTx, &UART_InitStruct); 
            /*NVIC中断配置*/
			NVIC_DisableIRQ(UART5_IRQn);
			NVIC_SetPriority(UART5_IRQn,2);//中断优先级配置
			NVIC_EnableIRQ(UART5_IRQn);
			break;
				
		default:
		break;
    }

	LL_UART_EnableIT_ReceiveBuffFull(UARTx);
	LL_UART_EnableDirectionTx(UARTx);
	LL_UART_EnableDirectionRx(UARTx);		
}

/**
 * @description: 串口推出休眠模式
 * @param  uart_id 串口的ID
 * @return None
 */
void uart_exti_sleep_mode(uint8_t uart_id)
{
	LL_GPIO_InitTypeDef io_config = {0}; 
	if(uart_id == USB_UART) {
		//USB串口
		io_config.Pin        = usb_rx_pin | usb_tx_pin;
		io_config.Mode       = LL_GPIO_MODE_DIGITAL;
		io_config.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		LL_GPIO_Init(usb_pin_port, &io_config);
		LL_GPIO_EnablePinInput(usb_pin_port, usb_rx_pin | usb_tx_pin);
		LL_UART_EnableDirectionRx(UART0);
		LL_UART_EnableDirectionTx(UART0);
	} else if(uart_id == NB_UART) {
		//无线网络串口
		io_config.Pin        = wireless_rx_pin | wireless_tx_pin;
		io_config.Mode       = LL_GPIO_MODE_DIGITAL;
		io_config.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		LL_GPIO_Init(wireless_pin_port, &io_config);
		LL_GPIO_EnablePinInput(wireless_pin_port, wireless_rx_pin | wireless_tx_pin);
		LL_UART_EnableDirectionRx(UART1);
		LL_UART_EnableDirectionTx(UART1);
	}

	#if 0
	else {
		//RS485串口
		io_config.Pin        = rs485_tx_pin | rs485_rx_pin;
		io_config.Mode       = LL_GPIO_MODE_DIGITAL;
		io_config.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		LL_GPIO_Init(rs485_pin_port, &io_config);
		LL_GPIO_EnablePinInput(rs485_pin_port, rs485_tx_pin | rs485_rx_pin);
		LL_UART_EnableDirectionRx(UART4);
		LL_UART_EnableDirectionTx(UART4);
	}
	#endif
}

/**
 * @description: 串口进入休眠模式
 * @param  uart_id 串口的ID
 * @return None
 */
void uart_enter_sleep_mode(uint8_t uart_id)
{
	LL_GPIO_InitTypeDef io_config = {0}; 
	if(uart_id == USB_UART && LL_UART_IsEnbaledDirectionTx(UART0)) {
		io_config.Pin        = usb_rx_pin | usb_tx_pin;
		io_config.Mode       = LL_GPIO_MODE_INPUT;
		io_config.Pull       = DISABLE;
		io_config.RemapPin   = DISABLE;
		
		LL_GPIO_Init(usb_pin_port, &io_config);
		LL_GPIO_DisablePinInput(usb_pin_port, usb_rx_pin | usb_tx_pin);    //usb_rx_pin | usb_tx_pin
		LL_UART_DisableDirectionRx(UART0);
		LL_UART_DisableDirectionTx(UART0);
		device_info.debug_en = 0;
	} else if(uart_id == NB_UART && LL_UART_IsEnbaledDirectionTx(UART1)) {
		//无线网络串口
		io_config.Pin        = wireless_tx_pin | wireless_rx_pin;
		io_config.Mode       = LL_GPIO_MODE_OUTPUT;
		io_config.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
		io_config.Pull       = DISABLE;
		io_config.RemapPin   = DISABLE;
		
		LL_GPIO_Init(wireless_pin_port, &io_config);
		LL_GPIO_DisablePinInput(wireless_pin_port, wireless_tx_pin | wireless_rx_pin);
		LL_UART_DisableDirectionRx(UART1);
		LL_UART_DisableDirectionTx(UART1);

	}
	
	#if 0
	else if(uart_id == RS485_UART && LL_UART_IsEnbaledDirectionTx(UART4))// 
	{
		//RS232串口
		io_config.Pin        = rs485_tx_pin | rs485_rx_pin;
		io_config.Mode       = LL_GPIO_MODE_INPUT;
		io_config.Pull       = DISABLE;
		io_config.RemapPin   = DISABLE;
		
		LL_GPIO_Init(rs485_pin_port, &io_config);
		LL_GPIO_DisablePinInput(rs485_pin_port, rs485_tx_pin | rs485_rx_pin);
		LL_UART_DisableDirectionRx(UART4);
		LL_UART_DisableDirectionTx(UART4);

	}
	#endif
}


