#include "adc.h"
#include "user_init.h"

uint32_t adc_vbat;
uint64_t adc_vint;
uint32_t vref_vref;


void AdcInit_P(void)    //外部供电充电模式下启动电池电压检测和外部输入电压检测
{
	LL_GPIO_InitTypeDef         GPIO_InitStruct;	
	LL_ADC_CommonInitTypeDef    ADC_CommonInitStruct;
	LL_ADC_InitTypeDef          adc_config ;

	//配置引脚为模拟功能
	//电池电压采样
	GPIO_InitStruct.Pin        = ADC_IN8_gpioc_id;
	GPIO_InitStruct.Mode       = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull       = DISABLE;
	GPIO_InitStruct.RemapPin   = DISABLE;	
	LL_GPIO_Init(ADC_IN8_gpioc, &GPIO_InitStruct);
	
	//外部输入电压采样
	GPIO_InitStruct.Pin        = ADC_IN1_gpioc_id;
	GPIO_InitStruct.Mode       = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull       = DISABLE;
	GPIO_InitStruct.RemapPin   = DISABLE;	
	LL_GPIO_Init(ADC_IN1_gpioc, &GPIO_InitStruct);	

	//ADC 时钟设置
	ADC_CommonInitStruct.AdcClockSource    = LL_RCC_ADC_OPERATION_CLOCK_PRESCALLER_RCHF; //RCHF
	ADC_CommonInitStruct.AdcClockPrescaler = LL_RCC_ADC_OPERATION_CLOCK_PRESCALER_DIV32; //16分频
	LL_ADC_CommonInit(&ADC_CommonInitStruct);
	
	//ADC 寄存器设置
	adc_config.ADC_ContinuousConvMode   = LL_ADC_CONV_SINGLE;//单次模式
	adc_config.ADC_AutoMode             = LL_ADC_SINGLE_CONV_MODE_AUTO;//自动
	adc_config.ADC_ScanDirection        = LL_ADC_SEQ_SCAN_DIR_FORWARD;//通道正序扫描
	adc_config.ADC_ExternalTrigConv     = LL_ADC_EXT_TRIGGER_NONE;//禁止触发信号
	adc_config.ADC_OverrunMode 		      = LL_ADC_OVR_DATA_OVERWRITTEN;//覆盖上次数据
	adc_config.ADC_WaitMode 		        = LL_ADC_WAIT_MODE_NO_WAIT;//等待 
	adc_config.ADC_SamplingStartControl = LL_ADC_SAMPLING_START_CONTROL_BY_REG;//由START寄存器启动ADC采样
	adc_config.ADC_SamplingTimeControl  = LL_ADC_SAMPLING_TIME_CONTROL_BY_REG;//由寄存器控制ADC采样时间
	adc_config.ADC_Channel_Swap_Wiat    = LL_ADC_SAMPLEING_INTERVAL_11_CYCLES;//通道切换等待时间
	adc_config.ADC_Channel_Fast_Time    = LL_ADC_FAST_CH_SAMPLING_TIME_128_ADCCLK;//快速通道采样时间
	adc_config.ADC_Channel_Slow_Time    = LL_ADC_FAST_CH_SAMPLING_TIME_128_ADCCLK;//慢速通道采样时间
	adc_config.ADC_Oversampling         = ENABLE;//过采样关闭
	adc_config.ADC_OverSampingRatio     = LL_ADC_OVERSAMPLING_8X;//8倍过采样
	adc_config.ADC_OversamplingShift    = LL_ADC_OVERSAMPLING_RESULT_DIV8;//数据右移, /8	
	LL_ADC_Init(ADC, &adc_config);
}

void AdcInit_sensor(void)    //ADC采样传感器数据
{
	LL_GPIO_InitTypeDef         GPIO_InitStruct;	
	LL_ADC_CommonInitTypeDef    ADC_CommonInitStruct;
	LL_ADC_InitTypeDef          adc_config ;

	//配置引脚为模拟功能
	GPIO_InitStruct.Pin        = ADC_IN3_PD0_3|ADC_IN4_PD1_2|ADC_IN5_PD2_1;
	GPIO_InitStruct.Mode       = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull       = DISABLE;
	GPIO_InitStruct.RemapPin   = DISABLE;	
	LL_GPIO_Init(ADC_IN_PT100_port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin        = ADC_IN6_PA_13;
	GPIO_InitStruct.Mode       = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull       = DISABLE;
	GPIO_InitStruct.RemapPin   = DISABLE;	
	LL_GPIO_Init(ADC_IN_3_3V_port, &GPIO_InitStruct);
	
	//ADC 时钟设置
	ADC_CommonInitStruct.AdcClockSource    = LL_RCC_ADC_OPERATION_CLOCK_PRESCALLER_RCHF; //RCHF
	ADC_CommonInitStruct.AdcClockPrescaler = LL_RCC_ADC_OPERATION_CLOCK_PRESCALER_DIV32; //16分频
	LL_ADC_CommonInit(&ADC_CommonInitStruct);
	
	//ADC 寄存器设置
	adc_config.ADC_ContinuousConvMode   = LL_ADC_CONV_SINGLE;//单次模式
	adc_config.ADC_AutoMode             = LL_ADC_SINGLE_CONV_MODE_AUTO;//自动
	adc_config.ADC_ScanDirection        = LL_ADC_SEQ_SCAN_DIR_FORWARD;//通道正序扫描
	adc_config.ADC_ExternalTrigConv     = LL_ADC_EXT_TRIGGER_NONE;//禁止触发信号
	adc_config.ADC_OverrunMode 		      = LL_ADC_OVR_DATA_OVERWRITTEN;//覆盖上次数据
	adc_config.ADC_WaitMode 		        = LL_ADC_WAIT_MODE_NO_WAIT;//等待 
	adc_config.ADC_SamplingStartControl = LL_ADC_SAMPLING_START_CONTROL_BY_REG;//由START寄存器启动ADC采样
	adc_config.ADC_SamplingTimeControl  = LL_ADC_SAMPLING_TIME_CONTROL_BY_REG;//由寄存器控制ADC采样时间
	adc_config.ADC_Channel_Swap_Wiat    = LL_ADC_SAMPLEING_INTERVAL_11_CYCLES;//通道切换等待时间
	adc_config.ADC_Channel_Fast_Time    = LL_ADC_FAST_CH_SAMPLING_TIME_128_ADCCLK;//快速通道采样时间
	adc_config.ADC_Channel_Slow_Time    = LL_ADC_FAST_CH_SAMPLING_TIME_128_ADCCLK;//慢速通道采样时间
	adc_config.ADC_Oversampling         = ENABLE;//过采样关闭
	adc_config.ADC_OverSampingRatio     = LL_ADC_OVERSAMPLING_8X;//8倍过采样
	adc_config.ADC_OversamplingShift    = LL_ADC_OVERSAMPLING_RESULT_DIV8;//数据右移, /8	
	LL_ADC_Init(ADC, &adc_config);
}
/****************************************************************************************************************/
static uint32_t adc_get_vref_sample(void)   //获取基准电压
{
    uint16_t ADCRdresult;
    LL_VREF_EnableVREFBuffer(VREF);//使能VREF BUFFER
	vTaskDelay(2);
	LL_ADC_EnalbleSequencerChannel(ADC, LL_ADC_INTERNAL_CH_VREF);//通道选择VREF
			
	LL_ADC_ClearFlag_EOC(ADC);//清标志			
	LL_ADC_Enable(ADC);   	 // 启动ADC
	LL_ADC_StartConversion(ADC);  // 开始转换
	// 等待转换完成
	while (LL_ADC_IsActiveFlag_EOC(ADC) == RESET);
	LL_ADC_ClearFlag_EOC(ADC);//清标志
	ADCRdresult =LL_ADC_ReadConversionData12(ADC);//获取采样值

	LL_ADC_DisableSequencerChannel(ADC, LL_ADC_INTERNAL_CH_VREF);//通道关闭VREF	
	LL_ADC_Disable(ADC);    // 关闭ADC
	LL_VREF_DisableVREFBuffer(VREF);//关闭VREF BUFFER			
		// 转换结果  
    return ADCRdresult;
}

/****************************************************************************************************************/
static uint32_t adc_get_data(uint32_t channal)
{
	uint32_t result;
	LL_ADC_EnalbleSequencerChannel(ADC, channal);//通道选择ADC_0

    LL_ADC_ClearFlag_EOC(ADC);//清标志			
    LL_ADC_Enable(ADC);   	 // 启动ADC
    LL_ADC_StartConversion(ADC);  // 开始转换
    // 等待转换完成
    while (LL_ADC_IsActiveFlag_EOC(ADC) == RESET);
    LL_ADC_ClearFlag_EOC(ADC);//清标志
    result =LL_ADC_ReadConversionData12(ADC);//获取采样值

    LL_ADC_Disable(ADC);    // 关闭ADC
    LL_ADC_DisableSequencerChannel(ADC,channal );//通道关闭ADC_0  LL_ADC_EXTERNAL_CH_0|LL_ADC_EXTERNAL_CH_1
	return result;
}

/****************************************************************************************************************/
void get_adc_vbat_value(void)
{    	
	char ADC_VALUE[10];	
	vref_vref = adc_get_vref_sample();
	#if 0
	vTaskDelay(3);
	#endif
	adc_vint =  adc_get_data(LL_ADC_EXTERNAL_CH_8);
	adc_vbat = (adc_vint * 3000 * ADC_VREF ) /(vref_vref * 4095);
	device_info.vbat_value = adc_vbat;
	#if 0
	device_info.vbat_value = adc_vbat*2*1.22-742;            //由于输入阻抗太大，加入了算法纠偏
	#endif
	 
	sprintf((char *)ADC_VALUE,"%d",device_info.vbat_value);
	Trace_log("电池电压：");
	Trace_log(ADC_VALUE);
	Trace_log("mV\r\n");	
	
	if(device_info.vbat_value>3600 && device_info.vbat_value<=4200)
	{
		device_info.channel_data[0] = (device_info.vbat_value-3600)/6;
		if(device_info.channel_data[0]>100)
			device_info.channel_data[0] = 100.00;
	}
	else if(device_info.vbat_value < 3600)
		device_info.channel_data[0] = 0;
	else
		device_info.channel_data[0] = 100.00;
	
	adc_vint =  adc_get_data(LL_ADC_EXTERNAL_CH_1);
	adc_vbat = (adc_vint * 3000 * ADC_VREF ) /(vref_vref * 4095);
	device_info.vbat_value = adc_vbat*11;
	#if 0
	device_info.vbat_value = adc_vbat*2*1.22-802;            //由于输入阻抗太大，加入了算法纠偏
	#endif
	 
	sprintf((char *)ADC_VALUE,"%d",device_info.vbat_value);
	Trace_log("外部输入电压：");
	Trace_log(ADC_VALUE);
	Trace_log("mV\r\n");
	
	if(device_info.vbat_value>4500)
		device_info.waibu_pwr_24 = 1;
	else
		device_info.waibu_pwr_24 = 0;
}


void PT100_K_D_get(void)
{
	device_info.PT100_K[0] = 100/(device_info.save.pt100_402[0]-device_info.save.pt100_100[0]);
	device_info.PT100_D[0] = 100 - device_info.save.pt100_100[0]*device_info.PT100_K[0];
	device_info.PT100_K[1] = 100/(device_info.save.pt100_402[1]-device_info.save.pt100_100[1]);
	device_info.PT100_D[1] = 100 - device_info.save.pt100_100[1]*device_info.PT100_K[1];
	device_info.PT100_K[2] = 100/(device_info.save.pt100_402[2]-device_info.save.pt100_100[2]);
	device_info.PT100_D[2] = 100 - device_info.save.pt100_100[2]*device_info.PT100_K[2];
}



uint8_t GET_PT100_data(void)
{
	uint8_t i;
	uint8_t re = TRUE;
	float xda1;  
	char ADC_VALUE[40];	

	if(LL_GPIO_IsInputPinSet(usbpower_gpio,usbpower_gpio_id)) {    //没有外部USB插入 
		xda1 = 0;
	} else {
		xda1 = -0.7;
	}

	//PT100A
	Trace_log("读取PT100温度\r\n");
	vref_vref = adc_get_vref_sample();	
	adc_vint =  adc_get_data(LL_ADC_EXTERNAL_CH_5);  
	device_info.channel_data[1] = (adc_vint * 30.0 * ADC_VREF ) /(vref_vref * 4095);  //3000*0.01	
	for(i = 0; i < 99; i++) {
		vref_vref = adc_get_vref_sample();
		adc_vint =  adc_get_data(LL_ADC_EXTERNAL_CH_5);
		device_info.channel_data[1] += (adc_vint * 30.0 * ADC_VREF ) /(vref_vref * 4095);

		#if 0
		printf("vref_vref:%d\r\n",vref_vref);
		printf("adc_vint:%d\r\n",adc_vint);
		printf("device_info.channel_data:%d\r\n",device_info.channel_data[1]);
		#endif
		
	}

	device_info.channel_data[1] = device_info.channel_data[1]/3+1650;    //PT100分压电阻上电压     带运放桥式分压电路
	device_info.channel_data[1] = 3300*249/device_info.channel_data[1]-249;   //vout_p 

	#if 0
	printf("device_info.channel_data_temp:%d\r\n",device_info.channel_data[1]);
	#endif


	if(device_info.pt100_clb_sta == 1)   //校准
	{
		if(device_info.channel_data[1] >95 && device_info.channel_data[1]<105)
		{
			device_info.save.pt100_100[0] = device_info.channel_data[1];
			device_info.pt100_clb_sta = 0;
		}
		else if(device_info.channel_data[1] >195 && device_info.channel_data[1]<205)
		{
			device_info.save.pt100_402[0] = device_info.channel_data[1];
			device_info.pt100_clb_sta = 0;
		}
		if(device_info.pt100_clb_sta == 0)
		{
			device_info.PT100_K[0] = 100/(device_info.save.pt100_402[0]-device_info.save.pt100_100[0]);
			device_info.PT100_D[0] = 100 - device_info.save.pt100_100[0]*device_info.PT100_K[0];
		}
	}	
	device_info.channel_data[1] = device_info.channel_data[1]*device_info.PT100_K[0]+device_info.PT100_D[0];   //*1.01+0.476
	device_info.channel_data[1] = Pt100_RToT(device_info.channel_data[1])+device_info.save.channel_c[1]+xda1;
	alarm_check(1);
	
	//PT100B
	vref_vref = adc_get_vref_sample()-xda1;	
	adc_vint =  adc_get_data(LL_ADC_EXTERNAL_CH_4);
	device_info.channel_data[2] = (adc_vint * 30.0 * ADC_VREF ) /(vref_vref * 4095);
	for(i=0;i<99;i++)
	{
		vref_vref = adc_get_vref_sample();
		adc_vint =  adc_get_data(LL_ADC_EXTERNAL_CH_4);
		device_info.channel_data[2] += (adc_vint * 30.0 * ADC_VREF ) /(vref_vref * 4095);
	}	
	device_info.channel_data[2] = device_info.channel_data[2]/3+1650;    //PT100分压电阻上电压  带运放桥式分压电路
	device_info.channel_data[2] = 3300*249/device_info.channel_data[2]-249;   //
	if(device_info.pt100_clb_sta == 1)
	{
		if(device_info.channel_data[2] >95 && device_info.channel_data[2]<105)
		{
			device_info.save.pt100_100[1] = device_info.channel_data[2];
			device_info.pt100_clb_sta = 0;
		}
		else if(device_info.channel_data[2] >195 && device_info.channel_data[2]<205)
		{
			device_info.save.pt100_402[1] = device_info.channel_data[2];
			device_info.pt100_clb_sta = 0;
		}
		if(device_info.pt100_clb_sta == 0)
		{		
			device_info.PT100_K[1] = 100/(device_info.save.pt100_402[1]-device_info.save.pt100_100[1]);
			device_info.PT100_D[1] = 100 - device_info.save.pt100_100[1]*device_info.PT100_K[1];
		}		
	}	
	device_info.channel_data[2] = device_info.channel_data[2]*device_info.PT100_K[1]+device_info.PT100_D[1];   //*1.01+0.476
	device_info.channel_data[2] = Pt100_RToT(device_info.channel_data[2])+device_info.save.channel_c[2]+xda1;
	alarm_check(2);
	
	//PT100C
	vref_vref = adc_get_vref_sample();	
	adc_vint =  adc_get_data(LL_ADC_EXTERNAL_CH_3);
	device_info.channel_data[3] = (adc_vint * 30.0 * ADC_VREF ) /(vref_vref * 4095);
	for(i=0;i<99;i++)
	{
		vref_vref = adc_get_vref_sample();
		adc_vint =  adc_get_data(LL_ADC_EXTERNAL_CH_3);
		device_info.channel_data[3] += (adc_vint * 30.0 * ADC_VREF ) /(vref_vref * 4095);
	}	
	device_info.channel_data[3] = device_info.channel_data[3]/3+1650;    //PT100分压电阻上电压  
	device_info.channel_data[3] = 3300*249/device_info.channel_data[3]-249;   //
	if(device_info.pt100_clb_sta == 1)
	{
		if(device_info.channel_data[3] >95 && device_info.channel_data[3]<105)
		{
			device_info.save.pt100_100[2] = device_info.channel_data[3];
			device_info.pt100_clb_sta = 0;
		}
		else if(device_info.channel_data[3] >195 && device_info.channel_data[3]<205)
		{
			device_info.save.pt100_402[2] = device_info.channel_data[3];
			device_info.pt100_clb_sta = 0;
		}
		if(device_info.pt100_clb_sta == 0)
		{		
			device_info.PT100_K[2] = 100/(device_info.save.pt100_402[2]-device_info.save.pt100_100[2]);
			device_info.PT100_D[2] = 100 - device_info.save.pt100_100[2]*device_info.PT100_K[2];
		}
	}	
	device_info.channel_data[3] = device_info.channel_data[3]*device_info.PT100_K[2]+device_info.PT100_D[2];   //*1.01+0.476
	device_info.channel_data[3] = Pt100_RToT(device_info.channel_data[3])+device_info.save.channel_c[3]+xda1;
	alarm_check(3);
	
	sprintf((char *)ADC_VALUE,"%.4f   %.4f  %.4f  ",device_info.channel_data[1],device_info.channel_data[2],device_info.channel_data[3]);
	Trace_log("PT100温度：");
	Trace_log(ADC_VALUE);
	Trace_log("℃\r\n");
		
	return re;
}
