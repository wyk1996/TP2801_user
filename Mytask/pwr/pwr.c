	
#include "main.h"
#include "pwr.h"


/********************��Ƭ����������ģʽ**************************/
void DeepSleep(void)
{
    LL_PMU_SleepInitTypeDef lpm_config;
//    LL_RCC_SetSleepModeRCLPWorkMode(LL_RCC_RCLP_UNDER_SLEEP_CLOSE);  //�����¹ر�RCLP
    LL_RMU_EnablePowerDownReset(RMU);    //��PDR
    LL_RMU_DisableBORPowerDownReset(RMU);//�ر�BOR 2uA
    
    LL_ADC_Disable(ADC);         //�ر�ADC
    LL_VREF_DisableVREF(VREF);   //�ر�VREF1p2
    WRITE_REG(VREF->BUFCR, 0);   //�ر�ȫ��VREFbuf
    
    lpm_config.DeepSleep       = LL_PMU_SLEEP_MODE_DEEP;
    lpm_config.PowerMode       = LL_PMU_POWER_MODE_SLEEP_AND_DEEPSLEEP;
    lpm_config.WakeupFrequency = LL_PMU_SLEEP_WAKEUP_FREQ_RCHF_8MHZ;
    lpm_config.WakeupDelay     = LL_PMU_WAKEUP_DELAY_TIME_2US;
    lpm_config.CoreVoltageScaling = DISABLE; 
    LL_PMU_Sleep_Init(PMU, &lpm_config);
}
/********************��ʼ��ȫ������***************************/
void Init_all_io(void)
{
	uart_var_init();    //������ؼĴ�����ʼ��
	DebugInit();    //���Դ���COM0��ʼ��
	if(device_info.save.tab_module == LORAWAN)
		Uartx_Init(UART1,9600);//��ʼ��uart����
	else
		Uartx_Init(UART1,115200);//��ʼ��uart����

	#if 0
	LL_UART_EnableDirectionTx(UART1);
	LL_UART_EnableDirectionRx(UART1);
	#endif
	
	GPIO_Init();      		/* GPIO ��ʼ�� */
	exinterup_init();
	SD3078_IIC3_init();  	//ʱ��IIC�ӿڳ�ʼ��
	
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

/********************���߻��Ѳ�������***************************/
void Init_some_io(void)
{
//	DebugInit();    //���Դ���COM0��ʼ��
//	Uartx_Init(UART1,9600);//��ʼ��uart����
//  LL_UART_EnableDirectionTx(UART1);
//	LL_UART_EnableDirectionRx(UART1);
//	if(LL_GPIO_IsInputPinSet(usbpower_gpio,usbpower_gpio_id))
	uart_exti_sleep_mode(USB_UART);
	uart_exti_sleep_mode(NB_UART);
//	GPIO_Init();      /* GPIO ��ʼ�� */
	SD3078_IIC3_init();   //ʱ��IIC�ӿڳ�ʼ��
//	SENSOR_IIC2_init();
	
//	AdcInit();
	SPI1Init();
	GPIO_Init_pwr3v();
//	GPIO_Init_pwrpcie();  
	device_info.MCU_statues = 0;
}
/********************�豸�ػ�***************************/
void POWR_CLOSE(void)
{	
	sd3078_dealarm();     //�ر��ⲿʱ���ж�	

	LL_GPIO_ALLPIN_LPM_MODE();    //��λ����IO��
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

/********************�豸����***************************/
void POWR_SLEEP(void)
{ 
	w25qxx_power_down();    //�ر�FLASH	
	LL_GPIO_ResetOutputPin(pwr3v_gpio, pwr3v_gpio_id);	  //����������Ͽ�

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


