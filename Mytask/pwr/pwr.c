	
#include "main.h"
#include "pwr.h"


/********************单片机进入休眠模式**************************/
void DeepSleep(void)
{
    LL_PMU_SleepInitTypeDef lpm_config;
//    LL_RCC_SetSleepModeRCLPWorkMode(LL_RCC_RCLP_UNDER_SLEEP_CLOSE);  //休眠下关闭RCLP
    LL_RMU_EnablePowerDownReset(RMU);    //打开PDR
    LL_RMU_DisableBORPowerDownReset(RMU);//关闭BOR 2uA
    
    LL_ADC_Disable(ADC);         //关闭ADC
    LL_VREF_DisableVREF(VREF);   //关闭VREF1p2
    WRITE_REG(VREF->BUFCR, 0);   //关闭全部VREFbuf
    
    lpm_config.DeepSleep       = LL_PMU_SLEEP_MODE_DEEP;
    lpm_config.PowerMode       = LL_PMU_POWER_MODE_SLEEP_AND_DEEPSLEEP;
    lpm_config.WakeupFrequency = LL_PMU_SLEEP_WAKEUP_FREQ_RCHF_8MHZ;
    lpm_config.WakeupDelay     = LL_PMU_WAKEUP_DELAY_TIME_2US;
    lpm_config.CoreVoltageScaling = DISABLE; 
    LL_PMU_Sleep_Init(PMU, &lpm_config);
}
/********************初始化全部外设***************************/
void Init_all_io(void)
{
	uart_var_init();    //串口相关寄存器初始化
	DebugInit();    //调试串口COM0初始化
	if(device_info.save.tab_module == LORAWAN)
		Uartx_Init(UART1,9600);//初始化uart配置
	else
		Uartx_Init(UART1,115200);//初始化uart配置

	#if 0
	LL_UART_EnableDirectionTx(UART1);
	LL_UART_EnableDirectionRx(UART1);
	#endif
	
	GPIO_Init();      		/* GPIO 初始化 */
	exinterup_init();
	SD3078_IIC3_init();  	//时钟IIC接口初始化
	
	#if 0
	SENSOR_IIC2_init();
	TM1792_IIC6_INIT();
	AdcInit();
	#endif
	SPI1Init();
	GPIO_Init_pwr3v();
	GPIO_Init_pwrpcie();  
	device_info.MCU_statues = 0;
}

/********************休眠唤醒部分外设***************************/
void Init_some_io(void)
{
//	DebugInit();    //调试串口COM0初始化
//	Uartx_Init(UART1,9600);//初始化uart配置
//  LL_UART_EnableDirectionTx(UART1);
//	LL_UART_EnableDirectionRx(UART1);
//	if(LL_GPIO_IsInputPinSet(usbpower_gpio,usbpower_gpio_id))
	uart_exti_sleep_mode(USB_UART);
	uart_exti_sleep_mode(NB_UART);
//	GPIO_Init();      /* GPIO 初始化 */
	SD3078_IIC3_init();   //时钟IIC接口初始化
//	SENSOR_IIC2_init();
	
//	AdcInit();
	SPI1Init();
	GPIO_Init_pwr3v();
//	GPIO_Init_pwrpcie();  
	device_info.MCU_statues = 0;
}
/********************设备关机***************************/
void POWR_CLOSE(void)
{	
	sd3078_dealarm();     //关闭外部时钟中断	

	LL_GPIO_ALLPIN_LPM_MODE();    //复位所有IO口
//	exinterup_init_key();
	GPIO_deinit();
	exinterup_deinit();
	GPIO_deInit_pwr3v();
	GPIO_deInit_pwrpcie();
//	SENSOR_IIC2_deinit();
//	SENSOR_IIC4_deinit();
//	SD3078_IIC3_deinit();
	SPI1deInit();
//	TM1792_IIC6_DEINIT();
		
	uart_enter_sleep_mode(USB_UART);
  uart_enter_sleep_mode(NB_UART);
//	uart_enter_sleep_mode(RS485_UART);
	taskEXIT_CRITICAL();
  
	DeepSleep();
}

/********************设备休眠***************************/
void POWR_SLEEP(void)
{ 
	w25qxx_power_down();    //关闭FLASH	
	LL_GPIO_ResetOutputPin(pwr3v_gpio, pwr3v_gpio_id);	  //传感器供电断开

	GPIO_deinit();
//  SENSOR_IIC4_deinit();	
//	SENSOR_IIC2_deinit();
	SPI1deInit();
//	TM1792_IIC6_DEINIT();
	SD3078_IIC3_deinit();
//	exinterup_init();
		
	uart_enter_sleep_mode(USB_UART);
  uart_enter_sleep_mode(NB_UART);
//	uart_enter_sleep_mode(RS485_UART);
	DeepSleep();
}


