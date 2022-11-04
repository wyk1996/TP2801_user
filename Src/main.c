#include "main.h"
#include "fm33lc0xx.h"


#define TASK_LED_PRIORITY       (tskIDLE_PRIORITY + 1)
#define TASK_ADC_PRIORITY      (tskIDLE_PRIORITY + 1)
#define TASK_UART_PRIORITY      (tskIDLE_PRIORITY + 1)


QueueHandle_t  ADC_QueueHandle;   
QueueHandle_t dmaSemaphore;

ErrorStatus IWDT_Init(void)
{
	LL_IWDT_InitTypeDef IWDT_InitStruct;
	/* 默认不使用窗口 */
	IWDT_InitStruct.IwdtWindows    = 0;
	/*最长溢出时间*/
	IWDT_InitStruct.OverflowPeriod = LL_IWDT_IWDT_OVERFLOW_PERIOD_16000MS;
	return LL_IWDT_Init(IWDT, &IWDT_InitStruct);
}

void iap_done(void)
{
	uint32_t save_flag = 0;
	
	save_flag = flash_read_word(APP_UPDATE_FLAG);
	if(save_flag != 0X2801)
	{
		save_flag = 0X2801;
		LL_FLASH_PageErase(FLASH, APP_UPDATE_FLAG);
		flash_write(APP_UPDATE_FLAG, &save_flag, 1);
	}
}

int main(void)
{
    MF_Clock_Init();
    UserInit();
	device_param_read(&device_info.save);
    Init_all_io();
	DZ_init();
	channel_data_init();
	Trace_log("TP280x.Vx RUN!\r\n"); 
	sd3078_alarm();				
	IWDT_Init();
	iap_done();   
	device_info.event_again = 1;     		//重启或者初次上电
	device_info.event_flash_again = 1;     //重启或者初次上电
	device_info.device_en = 2;
	device_info.pt100_clb_sta = 0;
	device_info.event_up = EVENT_RUN;	
	device_data_to_modbusbuf();
	MX_FREERTOS_Init();      
	vTaskStartScheduler();
}


void vApplicationIdleHook(void)
{
    // TODO: 空闲任务
	#if 0
	0x2800 3d800
	#endif
}


  
//标准库需要的支持函数                 
struct __FILE
{
    int handle;
};
FILE __stdout;   

//重定义fputc函数
int fputc(int ch, FILE *f)
{      
	LL_UART_TransmitData(UART0, (uint8_t)ch);
	while(SET != LL_UART_IsActiveFlag_TXSE(UART0));
    return ch;
}



