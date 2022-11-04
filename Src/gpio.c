#include "gpio.h"
#include "user_init.h"

void GPIO_Init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = POWER_gpio_id;    ///
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(POWER_gpio, &GPIO_InitStruct);
	LL_GPIO_SetOutputPin(POWER_gpio, POWER_gpio_id);//����Ĭ�ϸߵ�ƽ

	GPIO_InitStruct.Pin = LED_gpio_id;    
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(LED_gpio, &GPIO_InitStruct);
	LL_GPIO_ResetOutputPin(LED_gpio, LED_gpio_id);//
  
	GPIO_InitStruct.Pin = rs485de_gpio_id;    
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(rs485de_gpio, &GPIO_InitStruct);
	LL_GPIO_ResetOutputPin(rs485de_gpio, rs485de_gpio_id);//485 ������ƿ�
	
	/**/
	GPIO_InitStruct.Pin = almio_gpio_id;    ///alm_io     �̵��������������
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(almio_gpio, &GPIO_InitStruct);
	if(device_info.alarm_statue == 1)
		LL_GPIO_SetOutputPin(almio_gpio, almio_gpio_id);
	else
		LL_GPIO_ResetOutputPin(almio_gpio, almio_gpio_id);   ///alm_io     Ĭ�Ϲر�

	GPIO_InitStruct.Pin = psmio_gpio_id;     //PSMIO   //ģ�����߻������� NB/4G    loraģ���Ӧģ����������豸����
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(psmio_gpio, &GPIO_InitStruct);
	LL_GPIO_ResetOutputPin(psmio_gpio, psmio_gpio_id);  //PSMIO

	GPIO_InitStruct.Pin = pwrio_gpio_id;  //PWRIO                 //////2G/4G/NB/lora  ��λ
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(pwrio_gpio, &GPIO_InitStruct);
//	if(device_info.save.tab_module == TP307V1_1_A || device_info.save.tab_module == LORAWAN || device_info.save.tab_module == EC25 || device_info.save.tab_module == EC25_G) //lora
		LL_GPIO_SetOutputPin(pwrio_gpio, pwrio_gpio_id);  //PWRIO  4G/lora   RES IO   Ĭ������	
//	else
//		LL_GPIO_ResetOutputPin(pwrio_gpio, pwrio_gpio_id);  //PWRIO  2G/NB   RES IO   Ĭ������	
	
	
	if(device_info.save.tab_module == TP307V1_1_A) {
		//lora
		GPIO_InitStruct.Pin = vddext_gpio_id;         //RESIO    //lora����
		GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		GPIO_InitStruct.Pull = ENABLE;
		LL_GPIO_Init(vddext_gpio, &GPIO_InitStruct);
		LL_GPIO_SetOutputPin(vddext_gpio, vddext_gpio_id);
	} else {
		GPIO_InitStruct.Pin = vddext_gpio_id;   //V1.8        ģ��VDD����
		GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = DISABLE;
		LL_GPIO_Init(vddext_gpio, &GPIO_InitStruct);
	}
	
}

/********************���ߡ��ػ�ģʽ������IO�ڽ����Ӧʡ��ģʽ***************************/
void GPIO_deinit(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	/**/	//���ػ�IO
	#if 0
	GPIO_InitStruct.Pin = POWER_gpio_id;    ///
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(POWER_gpio, &GPIO_InitStruct);
	LL_GPIO_SetOutputPin(POWER_gpio, POWER_gpio_id);//����Ĭ�ϸߵ�ƽ
	#endif
	
	/**/
	GPIO_InitStruct.Pin = rs485de_gpio_id;    
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(rs485de_gpio, &GPIO_InitStruct);
	LL_GPIO_ResetOutputPin(rs485de_gpio, rs485de_gpio_id);//485 ������ƿ�
	LL_GPIO_DisablePinInput(rs485de_gpio, rs485de_gpio_id);
	/**/
	
	GPIO_InitStruct.Pin = almio_gpio_id;    ///alm_io     �̵��������������
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(almio_gpio, &GPIO_InitStruct);
	if(device_info.alarm_statue == 1)
		LL_GPIO_SetOutputPin(almio_gpio, almio_gpio_id);
	else
		LL_GPIO_ResetOutputPin(almio_gpio, almio_gpio_id);   ///alm_io     Ĭ�Ϲر�
	  /**/
	GPIO_InitStruct.Pin = psmio_gpio_id;     //PSMIO   //ģ�����߻������� NB/4G    
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(psmio_gpio, &GPIO_InitStruct);
	LL_GPIO_DisablePinInput(psmio_gpio, psmio_gpio_id);
	#if 0
	LL_GPIO_ResetOutputPin(psmio_gpio, psmio_gpio_id);
	#endif
	  

	GPIO_InitStruct.Pin = pwrio_gpio_id;  //PWRIO                 //////2G/4G/NB/lora  ��λ
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(pwrio_gpio, &GPIO_InitStruct);
	LL_GPIO_DisablePinInput(pwrio_gpio, pwrio_gpio_id);
    

	GPIO_InitStruct.Pin = vddext_gpio_id;  //RESIO    //lora����   ģ��VDD-EX�������
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(vddext_gpio, &GPIO_InitStruct);
	LL_GPIO_DisablePinInput(vddext_gpio, vddext_gpio_id);
}
/*******PCIEģ�鹩��IO��ʼ��**********/
void GPIO_Init_pwrpcie(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = pwrpcie_gpio_id;    //pcie 
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(pwrpcie_gpio, &GPIO_InitStruct);
	LL_GPIO_ResetOutputPin(pwrpcie_gpio, pwrpcie_gpio_id); 	//ģ���ϵ�   ��ʼ���ߵ�ƽ���   Ĭ�Ϲر�
}
/*******PCIEģ�鹩��IO���ߵ͹���ģʽ�¸�λ**********/
void GPIO_deInit_pwrpcie(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};	
	/**/
	GPIO_InitStruct.Pin = pwrpcie_gpio_id;    //pcie 
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(pwrpcie_gpio, &GPIO_InitStruct);	
}
/*******����������IO��ʼ��**********/
void GPIO_Init_pwr3v(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = pwr3v_gpio_id;    // 
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(pwr3v_gpio, &GPIO_InitStruct);
	LL_GPIO_ResetOutputPin(pwr3v_gpio, pwr3v_gpio_id); 	//ģ���ϵ�   ��ʼ���ߵ�ƽ���   Ĭ�Ϲر�
}
/*******����������IO���ߵ͹���ģʽ�¸�λ**********/
void GPIO_deInit_pwr3v(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};	
	/**/
	GPIO_InitStruct.Pin = pwr3v_gpio_id;    
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(pwr3v_gpio, &GPIO_InitStruct);
	LL_GPIO_ResetOutputPin(pwr3v_gpio, pwr3v_gpio_id);//���⴫��������������ƿ� 
}

#if 0
/*******�ⲿ�жϳ�ʼ������**********/
void exinterup_init_key(void)
{
	LL_GPIO_InitTypeDef gpioInitStruct = {0};	

	LL_RCC_SetEXTIClockSource(RCC_OPCCR1_EXTICKS);      // �ⲿ�ж�ʱ��ѡ��LSCLK
	LL_RCC_Enable_SleepmodeExternalInterrupt();//����ʹ���ⲿ�жϲ���	
	LL_RCC_Group1_EnableOperationClock(LL_RCC_OPERATION1_CLOCK_EXTI);//EXTI����ʱ��ʹ�� 
	
	gpioInitStruct.Pin = key_gpio_id;				
	gpioInitStruct.Mode = LL_GPIO_MODE_INPUT;
	gpioInitStruct.Pull = DISABLE;
	LL_GPIO_Init(key_gpio, &gpioInitStruct);
	
	LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_13);
	LL_EXTI_EnableDigitalFilter(GPIO_COMMON, LL_GPIO_EXTI_LINE_13);
	LL_GPIO_SetExitLine4(GPIO_COMMON, LL_GPIO_EXTI_LINE_13_PD5);
	LL_EXTI_SetTrigEdge(GPIO_COMMON, LL_GPIO_EXTI_LINE_13, LL_GPIO_EXTI_TRIGGER_RISING);

	//�ж����ȼ�����
	NVIC_DisableIRQ(GPIO_IRQn);
	NVIC_SetPriority(GPIO_IRQn, 2);
	NVIC_EnableIRQ(GPIO_IRQn);
}
#endif

#if 0
/*******�ⲿ�жϳ�ʼ������**********/
void usb_int_fangdian(void)
{
	LL_GPIO_InitTypeDef gpioInitStruct = {0};
	
	gpioInitStruct.Pin = usbpower_gpio_id;   //usb  ��Դ����������
	gpioInitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	gpioInitStruct.Pull = DISABLE;   
	LL_GPIO_Init(usbpower_gpio, &gpioInitStruct);
	LL_GPIO_ResetOutputPin(usbpower_gpio, usbpower_gpio_id);//���⴫��������������ƿ� 
	delay_vtask(30);
	gpioInitStruct.Pin = usbpower_gpio_id;   //usb  ��Դ����������
	gpioInitStruct.Mode = LL_GPIO_MODE_INPUT;
	gpioInitStruct.Pull = DISABLE;   
	LL_GPIO_Init(usbpower_gpio, &gpioInitStruct);
}
/*******�ⲿ�жϳ�ʼ������**********/
#endif

void exinterup_init(void)
{
    LL_GPIO_InitTypeDef gpioInitStruct = {0};	

	LL_RCC_SetEXTIClockSource(RCC_OPCCR1_EXTICKS);      // �ⲿ�ж�ʱ��ѡ��LSCLK
	LL_RCC_Enable_SleepmodeExternalInterrupt();//����ʹ���ⲿ�жϲ���	
	LL_RCC_Group1_EnableOperationClock(LL_RCC_OPERATION1_CLOCK_EXTI);//EXTI����ʱ��ʹ�� 
		
    gpioInitStruct.Pin = key_gpio_id;				
    gpioInitStruct.Mode = LL_GPIO_MODE_INPUT;
    gpioInitStruct.Pull = DISABLE;
    LL_GPIO_Init(key_gpio, &gpioInitStruct);
		
	gpioInitStruct.Pin = usbpower_gpio_id;   //usb  ��Դ����������
	gpioInitStruct.Mode = LL_GPIO_MODE_INPUT;
	gpioInitStruct.Pull = DISABLE;   
	LL_GPIO_Init(usbpower_gpio, &gpioInitStruct);
		
	gpioInitStruct.Pin = LL_GPIO_PIN_0;				//ʱ���ж�
	gpioInitStruct.Mode = LL_GPIO_MODE_INPUT;
	gpioInitStruct.Pull = DISABLE;
	LL_GPIO_Init(GPIOB, &gpioInitStruct);		
		
	LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_13);
	LL_EXTI_EnableDigitalFilter(GPIO_COMMON, LL_GPIO_EXTI_LINE_13);
	LL_GPIO_SetExitLine13(GPIO_COMMON, LL_GPIO_EXTI_LINE_13_PD5);
	LL_EXTI_SetTrigEdge(GPIO_COMMON, LL_GPIO_EXTI_LINE_13, LL_GPIO_EXTI_TRIGGER_FALLING);

	LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_10);
	LL_EXTI_EnableDigitalFilter(GPIO_COMMON, LL_GPIO_EXTI_LINE_10);
	LL_GPIO_SetExitLine10(GPIO_COMMON, LL_GPIO_EXTI_LINE_10_PC11);
	LL_EXTI_SetTrigEdge(GPIO_COMMON, LL_GPIO_EXTI_LINE_10, LL_GPIO_EXTI_TRIGGER_RISING);
				
    LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_4);
    LL_EXTI_EnableDigitalFilter(GPIO_COMMON, LL_GPIO_EXTI_LINE_4);
    LL_GPIO_SetExitLine4(GPIO_COMMON, LL_GPIO_EXTI_LINE_4_PB0);
    LL_EXTI_SetTrigEdge(GPIO_COMMON, LL_GPIO_EXTI_LINE_4, LL_GPIO_EXTI_TRIGGER_RISING);
    
    // �ж����ȼ����� 
    NVIC_DisableIRQ(GPIO_IRQn);
    NVIC_SetPriority(GPIO_IRQn, 2);
    NVIC_EnableIRQ(GPIO_IRQn);
}


/****************************************************************************************/
void exinterup_deinit(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Pin = LL_GPIO_PIN_10;				//ʱ���ж� 
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_10);
	
	GPIO_InitStruct.Pin = usbpower_gpio_id;   //usb  ��Դ����������
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Pull = DISABLE;   //LL_GPIO_PULL_NO
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	LL_GPIO_Init(usbpower_gpio, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = key_gpio_id;   //��������������   ���ⲿ����
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Pull = DISABLE;   //LL_GPIO_PULL_NO
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	LL_GPIO_Init(key_gpio, &GPIO_InitStruct);
}
/****************************************************************************************/
void USB_Callback(void)
{	
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	Trace_log("USB����\r\n");
	xTaskNotifyFromISR(sleepHandle, 0x00, eSetBits, &xHigherPriorityTaskWoken);	
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void GPIO_IRQHandler(void)
{
    if (SET == LL_EXTI_IsActiveFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_13))
    {
        LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_13);
			  device_info.key = 1;
//			Trace_log("��������...\r\n");
			  if(device_info.MCU_statues == 2)
				{
//					NVIC_SystemReset();
					LL_RCC_SetChipReset();
				}
//				else
//					sd3078_alarm_Callback();
    }
		
		if (SET == LL_EXTI_IsActiveFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_10))    //USB
    {
        LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_10);
			  if(device_info.MCU_statues == 2)
				{
//					NVIC_SystemReset();
					LL_RCC_SetChipReset();
				}
				else if(device_info.MCU_statues == 1)
				{
					device_info.MCU_statues = 0;
//					USB_Callback();
//					Init_all_io();
				}
    }
		
		if(SET == LL_EXTI_IsActiveFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_4))
		{
			  LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_4);
//			  Trace_log("���ӻ���\r\n");
//			  iwdt_reload();  //ι��
				sd3078_alarm_Callback();		
		}
}


void iwdt_reload(void)
{
   LL_IWDG_ReloadCounter(IWDT);
}

void GPIO_ToggleBits(GPIO_Type* GPIOx, uint32_t GPIO_Pin)
{
	if(GPIO_Pin&GPIOx->DO)
	{
		LL_GPIO_ResetOutputPin(GPIOx,GPIO_Pin);	
	}
	else
	{	
        LL_GPIO_SetOutputPin(GPIOx,GPIO_Pin);
	}
}

