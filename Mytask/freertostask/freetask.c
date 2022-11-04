#include "main.h"
#include "freetask.h"


xTaskHandle device_usbHandle;   //usb   256     6
xTaskHandle device_flashHandle; //W_FLASH   1024        4
xTaskHandle sensorHandle;  //传感器  512     2
xTaskHandle sleepHandle;   //休眠    256     7  // 1
xTaskHandle sever_nbHandle;//NB      512     5
#if 0
xTaskHandle mykeyHandle;   //按键    256     1  // 7
xTaskHandle modbusHandle;   //按键    256     1  // 7
xTaskHandle rtc_displayHandle;  //RTC &  display  1024   3
osSemaphoreId myBinarySem01Handle;
SemaphoreHandle_t flash_semap;
uint32_t fsaddr = 0;
#endif

uint8_t s_year = 0;
uint8_t s_mon = 0;
uint8_t s_day = 0;
uint8_t e_year = 0;
uint8_t e_mon = 0;
uint8_t e_day = 0;

uint8_t MCU_CLOSE = 0;
uint8_t key_waitnum=0;
uint8_t lora_one = 0 ;

void MX_FREERTOS_Init(void) 
{ 
	xTaskCreate(device_usbTask, "Uart", 256, NULL, 0, &device_usbHandle); 
	xTaskCreate(flashTask, "FLASH", 384, NULL, 5, &device_flashHandle);	
	LCD_config_create_task(NULL,4);
	xTaskCreate(sensorTask, "SENSOR", 256, NULL, 3, &sensorHandle);
	xTaskCreate(severTask, "SERVER", 512, NULL, 2, &sever_nbHandle);
	xTaskCreate(sleepTask, "SLEEP", 256, NULL, 1, &sleepHandle);
	#if MODBUS_ENTER
	xTaskCreate(modbustask, "MODBUS", 128, NULL, 6, &modbusHandle);
	#endif
}


void device_usbTask(void *pvParameters)
{
	uint8_t usb_buf[128];
	uint16_t us_length;
	while(1) {
		#if 1
		us_length = uart_recv_data(&COM0, usb_buf, 128, 20);
		if (us_length >= 7) {
			if (usb_buf[0] == 0XFF) {
				if (usb_buf[2] == 0X4B) {
					if (usb_buf[1] == 0X03) {
						device_param_get(usb_buf[3]);
					} else if (usb_buf[1] == 0X0F) {
						device_param_set(usb_buf);
					}
 				} else if (usb_buf[2] == 0X4D && us_length >= 14) {					
					device_rtc_set(usb_buf, &device_info.data_time);	
					RTC_WriteDate(&device_info.data_time);
				}
			} else if (usb_buf[0] == 0X7E && us_length >= 15) {
				if (usb_buf[2] == 0X02) {
					s_year = usb_buf[10];
					s_mon  = usb_buf[11];
					s_day  = usb_buf[12];
					e_year = usb_buf[13];
					e_mon  = usb_buf[14];
					e_day  = usb_buf[15];						
					device_info.event_flash = EVENT_RUN;
					xTaskNotify(device_flashHandle, R_FLASH, eSetBits);
				}
			}

			if ((usb_buf[3] == 0X2F  || usb_buf[3] == 0X29) && crc_16(usb_buf, us_length - 3) == (((uint16_t )usb_buf[us_length - 2] << 8) | usb_buf[us_length - 3])) {
				LL_FLASH_PageErase(FLASH, 1024 * 8);
				LL_RCC_SetChipReset();
			}
 		}
		#endif
		delay_vtask(10);
	}
}


void flashTask(void *pvParameters)
{
	uint32_t wr_status = 0;
	device_info.alarm_flag_STATUS.flasherror = 0; 	
	w25qxx_init();
	device_info.save_addr = hist_init();//循环第一个扇区 
  	while(1) {			
    	Trace_log("flash操作\r\n");
		RTC_ReadDate(&device_info.data_time);  // 读取实时时钟
		w25qxx_exti_sleep_mode();
		w25qxx_power_wakeup();

		if((W_FLASH & wr_status) == W_FLASH) {
			Trace_log("数据存储\r\n");		
			if(hist_save(&device_info.save_addr)) {      ////写	
				device_info.alarm_flag_STATUS.flasherror = 1;
			} else {	
				device_info.alarm_flag_STATUS.flasherror = 0;				
			}
		} else if((R_FLASH & wr_status) == R_FLASH) {  //读
			Trace_log("读取历史\r\n");
			hist_read_date(s_year, s_mon, s_day, e_year, e_mon, e_day);   ///读开始时间
		}

		if((C_FLASH & wr_status) == C_FLASH) {    ///清历史记录
			Trace_log("清除历史\r\n");
			device_clear_hist();
			delay_vtask(50);				
		}

		device_info.event_flash = EVENT_OK;   /*完成 标志*/
		xTaskNotifyWait(0X00, 0XFFFFFFFF, &wr_status, portMAX_DELAY);
  	}
 
}


void sensorTask(void *pvParameters)
{	
	device_info.event_sensorread = EVENT_RUN;
	device_info.sensor_sam_timer = 2;   //开机默认存储加上主动上报
	AdcInit_sensor();
	AdcInit_P();
	PT100_K_D_get();
	for(;;)	{
		device_info.event_sensorread = EVENT_RUN;
		LL_ADC_Enable(ADC);         //使能ADC
		LL_VREF_EnableVREF(VREF);   //使能VREF1p2
		LL_GPIO_SetOutputPin(pwr3v_gpio, pwr3v_gpio_id);
		delay_vtask(50);	
		GET_PT100_data();
		port_data_get();
		LL_GPIO_ResetOutputPin(pwr3v_gpio, pwr3v_gpio_id);
		wait_time();
		xTaskNotify(device_LCDHandle, 0x00, eSetBits);   				   ////顯示
		device_info.event_sensorread = EVENT_OK;   /*完成 标志*/
		xTaskNotifyWait(0X00, 0XFFFFFFFF, 0, portMAX_DELAY);     //portMAX_DELAY
	}
}


static void Power_Off_Handle(void)
{
	Trace_log("设备进入关机\r\n");
	taskENTER_CRITICAL();	
	MCU_CLOSE = 0;			
	LCD_CLOSE();      //关闭显示
	while(!LL_GPIO_IsInputPinSet(key_gpio,key_gpio_id));	
	device_info.MCU_statues = 2;
	device_nbvbat_set(nb.Vbat_data);        //存储电池电量数据	
	LL_GPIO_ResetOutputPin(pwrpcie_gpio, pwrpcie_gpio_id);   			 //模块断电	
	LL_GPIO_ResetOutputPin(LED_gpio, LED_gpio_id);			
	w25qxx_power_down();    //关闭FLASH
	Trace_log("设备关机\r\n");
	uart_enter_sleep_mode(USB_UART);
	uart_enter_sleep_mode(NB_UART);
	vTaskDelay(100);
	LL_GPIO_ResetOutputPin(POWER_gpio, POWER_gpio_id);
	while(1) {
		if(LL_GPIO_IsInputPinSet(usbpower_gpio,usbpower_gpio_id))     //有USB供电   设备会重启   保持有外部供电的情况下设备不会关机
			LL_RCC_SetChipReset();
	}	
}

static void Power_Sleep_Handle(void)
{
	if(!LL_GPIO_IsInputPinSet(usbpower_gpio,usbpower_gpio_id)) {    
		//没有外部USB插入   
		Trace_log("没有USB插入\r\n");	
		device_info.waibu_pwr_usb = 0;
		if(device_info.device_en == 1) {
			device_info.device_en = 2;
			device_info.event_up = EVENT_RUN;
			xTaskNotify(sever_nbHandle, NET_IT, eSetBits);
		} else if(device_info.save.tab_protocol != MODBUS) {
			Trace_log("设备休眠sleep......\r\n");	
			device_info.MCU_statues = 1;		
			POWR_SLEEP();
			Init_some_io();
			Trace_log("设备从休眠中唤醒\r\n");						
		}
	} else {
		device_info.waibu_pwr_usb = SET;
	}	
}


void sleepTask(void *pvParameters)
{
	uint32_t event_sleep = 0;
	device_info.vabt_timer = 360;
	delay_vtask(10);
	for(;;) {
		iwdt_reload();  //喂狗

		if(device_info.save.srv_time == 0) {   //上报时间重置
			device_info.save.srv_time = 60;
		}
			
		if((device_info.wait_min*10) >= (device_info.save.srv_time*60)) {
			device_info.alarm_barck_flag = 0;
			device_info.wait_min = 0;
			device_info.sensor_sam_timer = 2;   //采样+上报
			Trace_log("传感器采样+数据上报\r\n");
			device_info.event_sensorread = EVENT_RUN;
			xTaskNotify(sensorHandle, 0x00, eSetBits);
		} else if(device_info.alarm_barck_flag >= device_info.save.SENSOR_SAM_TIME) {   //n*10秒   默认6*10=60秒
			device_info.alarm_barck_flag=0;
			device_info.sensor_sam_timer = 1;  //采样运行
			Trace_log("唤醒传感器采样\r\n");
			device_info.event_sensorread = EVENT_RUN;
			xTaskNotify(sensorHandle, 0x00, eSetBits);
		} else if(device_info.waibu_pwr_usb == 1) {
			if(device_info.alarm_barck_flag >= 1)    //调试模式下20秒读一次数据
			{
				device_info.alarm_barck_flag = 0;
				Trace_log("调试模式传感器读取\r\n");
				device_info.event_sensorread = EVENT_RUN;
				xTaskNotify(sensorHandle, 0x00, eSetBits);
			}
		}
		
			
		if((device_info.vabt_timer*10) >= 3600 ) {   //1小时更新电池电量  时钟校准置位
			VBAT_get();			
			device_info.time_calib = 1;
		}
			
		if(!LL_GPIO_IsInputPinSet(key_gpio,key_gpio_id) || device_info.key == 1)   //MCU在运行状态或者低功耗状态下
		{
			Trace_log("按键触发\r\n");
			device_info.key = 0;
			while(!LL_GPIO_IsInputPinSet(key_gpio,key_gpio_id)) {
				Trace_log("按键检测中\r\n");
				key_waitnum++;
				if(key_waitnum > 5)
					break;
				delay_vtask(150);
			}

			if(key_waitnum > 5) {
				MCU_CLOSE = SET;	
				key_waitnum	= RESET;			 
			} else {
				Trace_log("读取传感器\r\n");
				device_info.alarm_barck_flag = 0;
				key_waitnum=0;
				device_info.event_sensorread = EVENT_RUN;   /*完成 标志*/
				xTaskNotify(sensorHandle, 0, eSetBits);
			}			
		}
		
		if(MCU_CLOSE == 1) {
			Power_Off_Handle();
		}
		
		if(device_info.event_sensorread == EVENT_OK && device_info.event_flash == EVENT_OK && device_info.event_display == EVENT_OK && device_info.event_up == EVENT_OK) {	
			Power_Sleep_Handle();
		}	
		xTaskNotifyWait(0X00, 0XFFFFFFFF, &event_sleep,20);
	}
}




void severTask(void *pvParameters)
{		
	uint32_t eventsever_bit = 0 ;
  	uint8_t server_delay = 0;	
	device_info.Params_sever = 1;    //设置第一次开机上报配置信息
	device_info.time_calib = 1;    //首次开机默认校准时间
	device_info.event_up = EVENT_RUN;
	lora_buf.lora_updata_errnum = 0;
	device_info.gps_time = 25;   //开机使能GPS定位 NB
	device_info.lorawan_net_status = 0;
	device_info.mqtt_statue = 0;
  	PCIE_PW_CLOSE();
	delay_vtask(1500);
 
	if(device_info.save.tab_module == TP307V1_1_A) { //lora	 
		PCIE_PW_OPEN();
    	if(lora_one == 0) {
			delay_vtask(3000);
			lora_read();
			lora_one = 1;
		}
		lora_buf.lora_updata_errnum=0;
		lora_buf.LoRa_NETid_set=atoi((char *)device_info.save.srv_addr);
		lora_buf.LoRa_Band_set = atoi((char *)device_info.save.srv_login);
		lora_buf.LoRa_Txpower_set = atoi((char *)device_info.save.srv_port);
		lora_buf.LoRa_sudu_set = atoi((char *)device_info.save.srv_Password);		    
	}
 	eventsever_bit = NET_IT;

	for(;;) {
		server_delay = 0;
		if(device_info.save.tab_module >1) //通讯模块选择不为空
		{
			Trace_log("设备数据发送\r\n");
			if(device_info.save.tab_module == LORAWAN || device_info.save.tab_module == M101 || device_info.save.tab_module == BC95)
				Uartx_Init(UART1,9600);//初始化uart配置
			else
				Uartx_Init(UART1,115200);//初始化uart配置
			GPIO_Init();      /* GPIO 初始化 */
			if(device_info.event_up == EVENT_RUN && device_info.save.tab_model != 0  && device_info.save.tab_protocol != 0) {
				device_info.wait_min=0;
				if(device_info.save.tab_module == EC25||device_info.save.tab_module == EC25_G ) {   ///4G    			 					      			 
					if(device_info.save.tab_protocol == MQTT) { //MQTT{ 
						EC20_MQTT_task();						
					} else  if(device_info.save.tab_protocol == TCP) {      //tcp
						EC20_TCP_task();
					}
			 	} else if(device_info.save.tab_module == BC26 || device_info.save.tab_module == BC20 || device_info.save.tab_module == BC95) {
					//NB 
				 	if(device_info.save.tab_protocol == NBIOT) {   
						//NBIOT
						task_nb();   //NBIOT协议发送数据
				 	} else if(device_info.save.tab_protocol == UDP) {   
						//TCP UDP   || device_info.save.tab_protocol == TCP
						task_udpDZDG();    //大港油田定制版本
				 	}
			 	} else if(device_info.save.tab_module == M101) {//深燃NB 
					task_srnb();
			 	} else if(device_info.save.tab_module == TP307V1_1_A) { //lora
			 		  	
					if((eventsever_bit & NET_IT) == NET_IT) {     /////lora send data 
						   			
						lora_get_base();
						if(lora_data()) {   /////ok ?
							nb.nb_csq = lora_get_signal();
						} else {
							if(!lora_data()) {
								lora_buf.lora_updata_errnum++;
							}					
						} if(nb.nb_csq  < 1 ) {
							lora_buf.lora_updata_errnum++;
						} if( lora_buf.lora_updata_errnum >10  ) {
						 	lora_buf.lora_updata_errnum =0;
						 	lora_rest();	 
						}		 
					}
					if( (eventsever_bit & lora_IT) == lora_IT) {   ///get lora configuration parameters	 				
						lora_read();
					}

					if((eventsever_bit & loraset_IT) == loraset_IT) { ///set lora configuration parameters  and device.c 
							 
						if(lora_buf.LoRa_NETid_set != atoi((char *)device_info.save.srv_addr))     /////set lora net id
						{	
							lora_buf.LoRa_NETid_set=atoi((char *)device_info.save.srv_addr);
							lora_set_NSID(lora_buf.LoRa_NETid_set);
						}

						if((lora_buf.LoRa_Band_set != atoi((char *)device_info.save.srv_login))  || (lora_buf.LoRa_sudu_set != atoi((char *)device_info.save.srv_Password)) )
						{ 
							lora_buf.LoRa_Band_set = atoi((char *)device_info.save.srv_login);
							lora_buf.LoRa_sudu_set = atoi((char *)device_info.save.srv_Password); 
							lora_buf.LoRa_Band_set=lora_buf.LoRa_Band_set*1000000;				     /////set lora net band   xxxxMhz to    xxxhz 
							lora_set_BAND(lora_buf.LoRa_Band_set, lora_buf.LoRa_sudu_set ); 
						}

						if(lora_buf.LoRa_Txpower_set != atoi((char *)device_info.save.srv_port))   ////set lora tx power 
						{	
							lora_buf.LoRa_Txpower_set = atoi((char *)device_info.save.srv_port);
							lora_set_Txpower(lora_buf.LoRa_Txpower_set);
						}
					}			  
			 	} else if(device_info.save.tab_module == LORAWAN) {//lorawan		  	
					if((eventsever_bit & NET_IT) == NET_IT)     /////lora send data 
					{		   		
						task_lorawan();
					}

					if( (eventsever_bit & loraset_IT) == loraset_IT) ///set lorawan configuration parameters  and device.c 
					{					
						lorawan_setstate();
					}			  
			 	}	 	 
			}
		}
		device_info.event_up = EVENT_OK;
		xTaskNotifyWait(0X00, 0XFFFFFFFF, &eventsever_bit, portMAX_DELAY);
  	}
}


#if MODBUS_ENTER
void modbustask(void *pvParameters) 
{
	
	uint16_t tx_len1 = 0; 
	uint16_t rx_len1 = 0;
	uint32_t modbusevent_bit;
	uint8_t modbusbuff1[128],modbusbuff2[128];
	
	modbusbuff1 = pvPortMalloc(128);		
	modbusbuff2 = pvPortMalloc(128);
	
	Uartx_Init(UART4,9600);;     ////modbus配置需要重启才生效
	LL_UART_EnableDirectionTx(UART4);
	LL_UART_EnableDirectionRx(UART4);
	for(;;)
	{
		if(device_info.save.tab_protocol == MODBUS)   //modbus  需要配置  //可屏蔽该判断，开放兼容到所有中
		{	
			rx_len1=uart_recv_data(&MODBUS485, modbusbuff1, 128, 300);/////接收数据
	
			if(rx_len1)  ///可加长度判断  len  > 5
			{	
				memset(modbusbuff2,0,128);
				if(device_info.debug_en) 
					uart_transmit_buf(&COM0, modbusbuff1,rx_len1);
				tx_len1 = mbrtu_proc(modbusbuff1, rx_len1, modbusbuff2);  //  modubs_porc
				if(tx_len1)
				{			
					uart_transmit_buf(&MODBUS485, modbusbuff2, tx_len1);    //////ack
				}
				if(device_info.modbus_read_en == 1)   //modbus协议读取过数据之后   立即更新传感器数据
				{
					device_info.modbus_read_en = 0;
					xTaskNotify(sensorHandle, 0x00, eSetBits);
				}
				if(device_info.modbus_write_en == 1)   //modbus写入过数据之后   立即存储
				{
					device_info.modbus_write_en = 0;
					modbusbuf_to_device_data();  //写入过数据之后，modbus寄存器的数据转入flash存储器
					device_param_write(&device_info.save);
				}								
			}
			delay_vtask(50);    ///可优化
		}
   else 
		{			
			delay_vtask(50);
			xTaskNotifyWait(0X00, 0XFFFFFFFF, &modbusevent_bit, portMAX_DELAY);  			
		}		
	}
}


#endif

