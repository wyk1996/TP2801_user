#include "main.h"

/*****************************************************/
static void EC20_RST(void)     //模块按键重启
{
	//拉低500ms开机 
	Trace_log("4G模块按键重启\r\n");
	
	LL_GPIO_ResetOutputPin(pwrio_gpio, pwrio_gpio_id);    
	delay_vtask(500); 
	LL_GPIO_SetOutputPin(pwrio_gpio, pwrio_gpio_id);
	delay_vtask(500);
//	LL_GPIO_ResetOutputPin(pwrio_gpio, pwrio_gpio_id);
//	delay_vtask(1000);
}
/*****************************************************/
uint8_t EC20_4G_int()
{
//	uint8_t rx_size[100];
	EC20_RST();     //模块复位
	 if(TEST_AT(50))  //失败
	 {
	 	   return 1;
	 }
	 else
	 {	 		   
		 delay_vtask(2000);
	 	 if(TEST_CPIN())
	   {
	     return 2;
	   }
	 } 	 	
		 
	 delay_vtask(10);
//	 uart_send_ack(&GPRS, "AT+QIACT=1\r\n", wireless_buf2 ,"OK", recive_len ,300);
	 
	if(strcmp((char *)device_info.save.srv_apn, "") != 0)  //检测到已经设置APN
	{		
		if(device_info.save.tab_module == EC25 ||device_info.save.tab_module == EC25_G)   //4G
		{
			sprintf((char *)transmit_cmd, "AT+QICSGP=1,1,\"%s\",\"%s\",\"%s\",1\r\n", device_info.save.srv_apn,
																		 device_info.save.srv_apn_user_name,device_info.save.srv_apn_user_pass);   //set APN number
				uart_send_ack(&GPRS, transmit_cmd , wireless_buf, "OK", recive_len, 300);
			 uart_send_ack(&GPRS, "AT+QIACT=1\r\n", wireless_buf ,"OK", recive_len ,300);
		}
	}
	delay_vtask(50);
	SET_CFUN(1);  //开启全功能
	if(TEST_CREG(130))    //AT+CREG?   原本为62，因为注册网络时间较长，延长至130
	 { 
		  return 3;
	 }
   else
      TEST_CEREG(75);
	GET_IMSI((char *)nb.nb_imsi);
 nb.nb_csq = (uint8_t)GET_CSQ();
 GET_IMEI((char *)nb.nb_imei,"EC20");	
 GET_ICCID((char *)nb.nb_iccid,"EC20");
	
	return 0;
}
/*****************************************************/
uint8_t EC20_4G_online_ack(void)
{
	uint8_t wait_sec = 8;
	uint16_t size = 0;
	uint8_t reack=FALSE;
	while(wait_sec--)
	{
		size = uart_recv_data(&GPRS, wireless_buf, recive_len, 10000);
		if(size)
		{
		 Trace_log((uint8_t *)wireless_buf); ////////***打印串口1数据******///
			
			if(strstr((char *)wireless_buf, "CONNECT OK")   ||
			   strstr((char *)wireless_buf, "+QIOPEN: 0,0") ||
			   strstr((char *)wireless_buf, "ALREADY CONNECT")||
				 strstr((char *)wireless_buf, "+QIOPEN: 1,0"))
			{
				Trace_log("Connection to server successful\r\n");
					reack=TRUE;
				break;
			}
			else if(strstr((char *)wireless_buf, "FAIL") ||
					 strstr((char *)wireless_buf, "ERROR") ||
					 strstr((char *)wireless_buf, "+QIOPEN:"))
			{
				Trace_log("Connection to server failed\r\n");
				reack=FALSE;
				break;
			}
		}
	}
	return reack;
}
/*****************************************************/
uint8_t EC20_TCP_CONNET(void)
{		
	if(strlen((char *)device_info.save.srv_addr) != 0 || strlen((char *)device_info.save.srv_port) != 0)
	{
		if(device_info.save.tab_module == EC25 ||device_info.save.tab_module == EC25_G)
     sprintf((char *)transmit_cmd, "AT+QIOPEN=1,0,\"TCP\",\"%s\",%s,0,0\r\n",device_info.save.srv_addr,device_info.save.srv_port);		
//	  else  //2G
//		 sprintf((char *)conn_buf, "AT+QIOPEN=%d,\"TCP\",\"%s\",%s\r\n",conn_id,device_info.save.srv_addr,device_info.save.srv_port);		
	}
	uart_transmit_str(&GPRS,transmit_cmd);
  if(EC20_4G_online_ack() == TRUE)  //ok
	{		
		return TRUE;
	}
	else   //重新连接
	{
	   return FALSE;
	}
}

/*****************************************************/
/*****************************************************/
static uint8_t uart_wait_ack(uart_t *uart, uint8_t *recv_buf ,const char *str, uint16_t rd_size ,uint16_t wait_tick)
{
	uint16_t rx_size = 0;
	uint8_t ret = 1;
			
	while(wait_tick--)
	{
		rx_size = uart_recv_data(uart, recv_buf ,rd_size, 1 );
		
		if(rx_size)
		{   
			  if(device_info.debug_en)
				{ uart_transmit_buf(&COM0,(uint8_t *)recv_buf,rx_size);	  } ////////***打印串口1数据******///

				if(strstrlen(recv_buf, (uint8_t *)str , rx_size) == 0)
				{
						ret = 0;
						break;
				}
		}
	}
	return ret;
}
/*****************************************************/
static uint8_t EC20_TCP_sever_data(uint8_t *tx_buf, uint16_t tx_size )   //ok 返回TRUE   失败返回FALSE
{	
	uint8_t err_count = 2;
//	tx_cmd = transmit_cmd;
	uint8_t send_ok=FALSE;
	
	if(tx_size == 0)
	{
		tx_size = strlen((char *)tx_buf);
		if(tx_size == 0)
		{
			return FALSE;
		}
	}
	memset(transmit_cmd,0,sizeof(transmit_cmd));
	while(err_count--)
	{
		sprintf((char *)transmit_cmd, "AT+QISEND=0,%d\r\n", (int)tx_size);	
		if(uart_send_ack(&GPRS, transmit_cmd, transmit_cmd, ">", sizeof(transmit_cmd),5000))
		{  	
			Trace_log("发送收不到'>'\r\n");
			continue;
		}		
		uart_transmit_buf(&GPRS , tx_buf ,tx_size);
	  if(!uart_wait_ack(&GPRS, wireless_buf, "SEND OK",recive_len,5000))
	  {	
			send_ok=TRUE;
	 		Trace_log("发送成功\r\n");	
      break;			
		}
		else
		{  
			send_ok=FALSE;
//			  err_count=0;
				Trace_log("发送失败\r\n");				
		}	
//		break;
	}
	return send_ok;
}
/*****************************************************/
/*****************************************************/
static uint8_t MQTT_sever_data(uint8_t *tx_buf,uint16_t tx_len)   //ok 返回TRUE   失败返回FALSE   
{	
	uint8_t err_count = 2;
	uint8_t send_ok=FALSE;
  
	if(tx_len == 0)
	{
		tx_len = strlen((char *)tx_buf);
		if(tx_len == 0)
		  return FALSE;
	}
	
	while(err_count--)
	{
		sprintf((char *)transmit_cmd, "AT+QMTPUBEX=0,0,0,0,\"%s\",%d\r\n",device_info.save.srv_Topic ,tx_len);	
		if(uart_send_ack(&GPRS, transmit_cmd, transmit_cmd, ">", sizeof(transmit_cmd),5000) == FALSE)
		{  	
			Trace_log("发送收不到'>'\r\n");
			send_ok = FALSE;
			continue;
		}		
		uart_transmit_buf(&GPRS , tx_buf ,tx_len);
	  if(uart_strstr(&GPRS, wireless_buf, "+QMTPUBEX: 0,0,0",recive_len,5000) == TRUE)
	  {	
			send_ok=TRUE;
	 		Trace_log("发送成功\r\n");	
      break;			
		}
		else
		{  
			send_ok=FALSE;
			Trace_log("发送失败\r\n");				
		}	
	}
	return send_ok;
}
/*****************************************************/
uint8_t EC20_4G_mqttdata()   //失败返回1   成功返回0
{
	uint16_t len = 0;
	uint32_t adds;
	uint8_t i,baoj_id,baoj_status;
	float baoj_limit_h,baoj_limit_l,baoj_limit_f,baoj_data;
	uint8_t shuju_ok = 0;
		
	if(device_info.alarming_flag != 0 )      //报警
	{ 
		Trace_log("报警");
		while(device_info.alarming_flag != 0)			
		{
			for(i=0;i<16;i++)
			{
				if(device_info.alarm_flag_ID.ch_status[i] == 1)
				{
					baoj_id = i;
					baoj_status = device_info.alarm_flag_STATUS.ch_status[i];
					baoj_limit_h = device_info.save.channel_up[i];
					baoj_limit_l = device_info.save.channel_down[i];
					baoj_limit_f = device_info.save.channel_f[i];
					baoj_data = wl_hist_data.data[i];
					break;
				}
			}		
			sprintf((char *) wireless_buf, 
			"{\"times\":\"20%02d-%02d-%02d %02d:%02d:%02d\",\"sensorDatas\":["
			"{\"flag\":\"WRID\",\"value\":%.d},"
			"{\"flag\":\"ACTION\",\"value\":%d},"
			"{\"flag\":\"WRDATA\",\"value\":%.4f},"
			"{\"flag\":\"LIMIT_H\",\"value\":%.4f},"
			"{\"flag\":\"LIMIT_L\",\"str\":\"%.4f\"},"
			"{\"flag\":\"LIMIT_F\",\"value\":%.4f}]}",
			wl_hist_data.data_time.year,
			wl_hist_data.data_time.month,
			wl_hist_data.data_time.day,
			wl_hist_data.data_time.hour,
			wl_hist_data.data_time.min,
			wl_hist_data.data_time.sec,
			baoj_id,baoj_status,baoj_data,baoj_limit_h,baoj_limit_l,baoj_limit_f);
		
//		Trace_log(wireless_buf);
			len = strlen((char *)wireless_buf);
					
			if(MQTT_sever_data(wireless_buf,len)==FALSE)    //失败 
			{ 
				return FALSE;
			}
			device_info.alarming_flag--;
			device_info.alarm_flag_ID.ch_status[i] = 0;
			if(device_info.alarming_flag<=0 || device_info.alarming_flag>30)
				device_info.alarming_flag = 0;
		}
	}
	
	if(device_info.alarm_flag_STATUS.flasherror==0)   //flash没有故障
	{
		adds = device_get_flahaddr_addr();  //读取最近一次历史数据，保持上报数据与存储数据一致性
		hist_read(&wl_hist_data, adds);
		if(wl_hist_data.data_time.year == 255||	wl_hist_data.data_time.month == 255)
		{
			Trace_log("没有存储数据 发送实时数据\r\n");
			shuju_ok = 0;
		}
		else if(wl_hist_data.data_time.year != device_info.data_time.year ||	wl_hist_data.data_time.month != device_info.data_time.month || wl_hist_data.data_time.day !=device_info.data_time.day)
		{
			Trace_log("历史书存储错误 发送实时数据\r\n");
			shuju_ok = 0;
		}
		else
			shuju_ok = 1;
	}
	
		////历史补发	
  if(shuju_ok == 1)   //flash正常并且有历史存储    发送断点续传数据
	{		
		device_info.pend_num=device_pending_num_get();
		device_info.pend_addr=device_perv_addr_get();
		while(device_info.pend_num)
		{	
			hist_read(&wl_hist_data, device_info.pend_addr);
			if(  wl_hist_data.data_time.year==0 && wl_hist_data.data_time.month==0)
			{
					device_info.pend_num = 0;	 
					device_pending_num_set();		
					break;
			}
			if(wl_hist_data.next > 0X400000)
			{
		   	device_info.pend_num =0;
				device_pending_num_set();	
			 break;
			}	  	      
		 Trace_log("历史补发\r\n");			
		 	
			
			sprintf((char *) wireless_buf, 
			"{\"times\":\"20%02d-%02d-%02d %02d:%02d:%02d\",\"sensorDatas\":["
			"{\"flag\":\"Q\",\"value\":%d},"
			"{\"flag\":\"CH0\",\"value\":%.4f},"
			"{\"flag\":\"CH1\",\"value\":%.4f},"
			"{\"flag\":\"CH2\",\"value\":%.4f},"
			"{\"flag\":\"CH3\",\"value\":%.4f},"
			"{\"flag\":\"CH4\",\"value\":%.4f},"
			"{\"flag\":\"CH5\",\"value\":%.4f},"
			"{\"flag\":\"CH6\",\"value\":%.4f},"
			"{\"flag\":\"CH7\",\"value\":%.4f},"
			"{\"flag\":\"CH8\",\"value\":%.4f},"		
			"{\"flag\":\"CH9\",\"value\":%.4f}]}",
			wl_hist_data.data_time.year,
			wl_hist_data.data_time.month,
			wl_hist_data.data_time.day,
			wl_hist_data.data_time.hour,
			wl_hist_data.data_time.min,
			wl_hist_data.data_time.sec,
			nb.nb_csq,
			wl_hist_data.data[0],
			wl_hist_data.data[1],
			wl_hist_data.data[2],
			wl_hist_data.data[3],
			wl_hist_data.data[4],
			wl_hist_data.data[5],
			wl_hist_data.data[6],
			wl_hist_data.data[7],
			wl_hist_data.data[8],
			wl_hist_data.data[9]);
		
//		Trace_log(wireless_buf);
			len = strlen((char *)wireless_buf);		
			if(MQTT_sever_data(wireless_buf,len)==FALSE)    //失败 
			{ 
				return FALSE;
			}			
					// 设置地址待发数量 
			device_info.pend_num = device_info.pend_num - 1;
			device_info.pend_addr = wl_hist_data.next;

			device_perv_addr_set();	
			device_pending_num_set(); 
			delay_vtask(300);		 
		}	 
	}
//正常数据上报	
	if(shuju_ok == 0)
	{
		wl_hist_data.data_time.year = device_info.data_time.year;
		wl_hist_data.data_time.month = device_info.data_time.month;
		wl_hist_data.data_time.day = device_info.data_time.day;
		wl_hist_data.data_time.hour = device_info.data_time.hour;
		wl_hist_data.data_time.min = device_info.data_time.min;
		wl_hist_data.data_time.sec = device_info.data_time.sec;
		for(i=0;i<16;i++)
		{
			wl_hist_data.data[i] = device_info.channel_data[i];
		}
	}
	else
	{
		adds = device_get_flahaddr_addr();  //读取最近一次历史数据，保持上报数据与存储数据一致性
		hist_read(&wl_hist_data, adds);
	}
	
		sprintf((char *) wireless_buf, 
	"{\"times\":\"20%02d-%02d-%02d %02d:%02d:%02d\",\"sensorDatas\":["
	"{\"flag\":\"Q\",\"value\":%d},"
	"{\"flag\":\"CH0\",\"value\":%.4f},"
	"{\"flag\":\"CH1\",\"value\":%.4f},"
	"{\"flag\":\"CH2\",\"value\":%.4f},"
	"{\"flag\":\"CH3\",\"value\":%.4f},"
	"{\"flag\":\"CH4\",\"value\":%.4f},"
	"{\"flag\":\"CH5\",\"value\":%.4f},"
	"{\"flag\":\"CH6\",\"value\":%.4f},"
	"{\"flag\":\"CH7\",\"value\":%.4f},"
	"{\"flag\":\"CH8\",\"value\":%.4f},"		
	"{\"flag\":\"CH9\",\"value\":%.4f}]}",
	wl_hist_data.data_time.year,
	wl_hist_data.data_time.month,
	wl_hist_data.data_time.day,
	wl_hist_data.data_time.hour,
	wl_hist_data.data_time.min,
	wl_hist_data.data_time.sec,
	nb.nb_csq,
	wl_hist_data.data[0],
	wl_hist_data.data[1],
	wl_hist_data.data[2],
	wl_hist_data.data[3],
	wl_hist_data.data[4],
	wl_hist_data.data[5],
	wl_hist_data.data[6],
	wl_hist_data.data[7],
	wl_hist_data.data[8],
	wl_hist_data.data[9]);
//	Trace_log(wireless_buf);
	len = strlen((char *)wireless_buf);		
	if(MQTT_sever_data(wireless_buf,len)==FALSE)    //失败 
	{ 
		return FALSE;
	}
	return TRUE;	
}

/*****************************************************/
/*****************************************************/
/*****************************************************/
static uint8_t EC200S_GET_GNSS(void)
{
	uint8_t gnss = 60;
	uint8_t gnss_err = 0;
	char *buf_cache;
	uint8_t i,addr,jiexi;
//	float yushu;
	char gnss_time[6];
	union sen_data   //
	{
		int8_t data[4];
		float value;
	};
	union sen_data s_data;

	if(device_info.save.GPS_stu == OFF)
		device_info.gps_time = 0;
	else if(device_info.gps_time >= 24)
	{
		device_info.gps_time = 0;
		device_info.gps_satues = 0;
		sprintf((char *) transmit_cmd,"AT+QGPSCFG=\"outport\",\"none\"\r\n");
		uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "OK", recive_len, 300);
		sprintf((char *) transmit_cmd,"AT+QGPSCFG=\"nmeasrc\",1\r\n");
		uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "OK", recive_len, 300);
		sprintf((char *) transmit_cmd,"AT+QGPSCFG=\"gnssconfig\",7\r\n");
		uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "OK", recive_len, 300);
		sprintf((char *) transmit_cmd,"AT+QGPS=1\r\n");
		uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "OK", recive_len, 500);
		
		delay_vtask(5000);
		while(gnss--)
		{
			if(!uart_send_ack(&GPRS, "AT+QGPSGNMEA=\"RMC\"\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300))
			{
				gnss_err = 0;
				buf_cache = strstr((const char *)wireless_buf, "+QGPSGNMEA:" );
				addr = 0;
				i = 0;				
				while(*(buf_cache+i) != '\0')
				{
					if(*(buf_cache+i) == ',')
					  addr++;
					if(addr == 2)
					{
						i++;
						if(*(buf_cache+i) == 'A')  //检测到有效定位信息
						{
						  jiexi = 1;
							break;
						}
						else
							break;
					}
					i++;
				}				
			}
			else
				gnss_err++;
			if(gnss_err>5)
				return 3;
			if(jiexi == 1)
				break;
			delay_vtask(2000);  //ms延时
		}
		
		if(jiexi == 1)
		{
			buf_cache += 16;
			while(*(buf_cache++) != ',');     //查找第1个“，”
			for(i=0;i<6;i++)
			{
				gnss_time[i] = *(buf_cache++);
			}
			gnss_data_time->hour = ((gnss_time[0]-0x30)*10 + (gnss_time[1]-0x30)+8);    //hour
			gnss_data_time->min = ((gnss_time[2]-0x30)*10 + (gnss_time[3]-0x30));     //min
			gnss_data_time->sec = ((gnss_time[4]-0x30)*10 + (gnss_time[5]-0x30));     //sec
			if(gnss_data_time->hour >=24)
				gnss_data_time->hour -=24;
			while(*(buf_cache++) != ',');     //查找第2个“，”
			while(*(buf_cache++) != ',');     //查找第3个“，”
			
			s_data.value = atof(buf_cache);
			device_info.GPS_N = s_data.value;
      		
      while(*(buf_cache++) != ',');     //查找第4个“，”		
//      gnssdata[0] = *(buf_cache++);      //纬度方向			
			while(*(buf_cache++) != ',');     //查找第5个“，”

			s_data.value = atof(buf_cache);
			device_info.GPS_E = s_data.value;
			while(*(buf_cache++) != ',');     //查找第6个“，”
//			gnssdata[5] = *(buf_cache++);      //经度方向
			while(*(buf_cache++) != ',');     //查找第7个“，”
			while(*(buf_cache++) != ',');     //查找第8个“，”
			while(*(buf_cache++) != ',');     //查找第9个“，”
			for(i=0;i<6;i++)
			{
				gnss_time[i] = *(buf_cache++);
			}
			gnss_data_time->day = ((gnss_time[0]-0x30)*10 + (gnss_time[1]-0x30));    //day
			gnss_data_time->month = ((gnss_time[2]-0x30)*10 + (gnss_time[3]-0x30));     //month
			gnss_data_time->year = ((gnss_time[4]-0x30)*10 + (gnss_time[5]-0x30));     //year
			uart_send_ack(&GPRS, "AT+QGPSEND\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300);    //关闭GPS
			device_info.gps_satues = 1;
			return 1;			
		}
	}

	return 0;
}
/***************************************************************************/
uint8_t MQTT_CONNECT_LINE(void)
{
	memset((char *)transmit_cmd,0,sizeof(transmit_cmd));
//	sprintf((char *)transmit_cmd, "AT+QIACT=1\r\n");	   //激活PDP场景
//	if(uart_send_ack(&GPRS, transmit_cmd, wireless_buf2, "OK", recive_len,60000) == FALSE)
//	{
//		Trace_log("PDP场景激活失败\r\n");
//	}
	sprintf((char *)transmit_cmd, "AT+QMTCFG=\"recv/mode\",0,1,1\r\n");	   //配置MQTT接收模式
	uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "OK", recive_len,300);
	
	sprintf((char *)transmit_cmd, "AT+QMTOPEN=0,\"%s\",%s\r\n",device_info.save.srv_addr,device_info.save.srv_port);   //打开MQTT链接
	if(uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "+QMTOPEN: 0,0", recive_len,60000) == FALSE)
	{
		Trace_log("打开网络失败\r\n");
		return  FALSE;
	}
	else
	  Trace_log("打开网络成功\r\n");

	sprintf((char *)transmit_cmd, "AT+QMTCONN=0,\"%s\",\"%s\",\"%s\"\r\n",device_info.save.srv_login,device_info.save.srv_UserName,device_info.save.srv_Password);   //MQTT登录
	if(uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "+QMTCONN: 0,0,0", recive_len,60000) == FALSE)
	{
		Trace_log("MQTT登录失败\r\n");
		return  FALSE;
	}
	else
		Trace_log("MQTT登录成功\r\n");
	  
	sprintf((char *)transmit_cmd, "AT+QMTSUB=0,1,\"%s\",0\r\n",device_info.save.srv_Subacription);   //MQTT订阅
	if(uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "+QMTSUB: 0,1,0,0", recive_len,60000) == FALSE)
	{
		Trace_log("MQTT订阅失败\r\n");
		return  FALSE;
	}
	else
		Trace_log("MQTT订阅成功\r\n");
	
	return TRUE;
}
/*****************************************************/
/*****************************************************/
static uint8_t Tink_getParam(void)    
{
	uint16_t len = 0;
//	    uint8_t get_parmbuf[20]={0};	
	memset(wireless_buf,0,sizeof(wireless_buf));	
	sprintf((char *)wireless_buf, "getParam");
	len = strlen((char *)wireless_buf);		
	if(MQTT_sever_data(wireless_buf,len)==FALSE)    //失败 
	{ 
		return FALSE;
	}
	return TRUE; 
}
/*****************************************************/
//uint8_t *ec20_reddatabuf;	
/*****************************************************/
/*****************************************************/
static uint8_t EC20_params(void)
{
	uint16_t len;
	
//	memset(transmit_cmd,0,sizeof(transmit_cmd));
	memset(wireless_buf,0,sizeof(wireless_buf));
	
	Trace_log("同步参数\r\n");
	sprintf((char *)wireless_buf,"{\"params\":{\"AT\":%d,\"CT\":%d,\"BR\":0}}",device_info.save.srv_time,device_info.save.SENSOR_SAM_TIME);
//	Trace_log(wireless_buf);
	len = strlen((char *)wireless_buf);
	if(device_info.save.tab_protocol == MQTT)   //MQTT
	{		
		if(MQTT_sever_data(wireless_buf,len)==FALSE)
		 return FALSE;
	}
	else if(device_info.save.tab_protocol == TCP)   //TCP
	{
		EC20_TCP_sever_data(wireless_buf, len);
	}
	
	sprintf((char *) wireless_buf, 
	"{\"addTime\":\"20%02d-%02d-%02d %02d:%02d:%02d\",\"sensorDatas\":["
	"{\"flag\":\"IMEI\",\"str\":\"%s\"},"
	"{\"flag\":\"ICCID\",\"str\":\"%s\"}]}",
	device_info.data_time.year,device_info.data_time.month,device_info.data_time.day,device_info.data_time.hour,device_info.data_time.min,device_info.data_time.sec,
	nb.nb_imei,nb.nb_iccid);
	
//	Trace_log(wireless_buf);
	len = strlen((char *)wireless_buf);
	if(device_info.save.tab_protocol == MQTT)   //MQTT
	{		
		if(MQTT_sever_data(wireless_buf,len)==FALSE)
		 return FALSE;
	}
	else if(device_info.save.tab_protocol == TCP)   //TCP
	{
		EC20_TCP_sever_data(wireless_buf, len);
	}
	
	device_info.Params_sever=0;
	delay_vtask(2000);
	return TRUE;
}
/*****************************************************/
uint16_t EC20_recv_data()
{	
	uint16_t size,da;
	uint8_t wait_tick = 5;
//	uint8_t i;
	char *getp;
//	char SAM[5];
	uint8_t reddatalen[8];
	uint8_t datalen=0;
  uint8_t f_write=0;
//  float f_da;

	while(wait_tick--)
	{
		delay_vtask(500);
//		memset(sevebuf, 0, output_len1);

		if(device_info.save.tab_module == EC25 ||device_info.save.tab_module == EC25_G)  //4G
		{uart_transmit_buf(&GPRS,(uint8_t *) "AT+QIRD=0\r\n" ,11);	}	
		size = uart_recv_data(&GPRS, wireless_buf, recive_len, 300);
		if(size)
		{
				if(device_info.debug_en)
				{ 
					uart_transmit_buf(&COM0,(uint8_t *)wireless_buf,size);	 
				} ////////打印串口1数据///
			if(strstr((char *)wireless_buf, "+QIRD:"))
			{		   	
				 getp =strstr(( char *)wireless_buf, "+QIRD:");
				 if(device_info.save.tab_module == EC25 ||device_info.save.tab_module == EC25_G)   //4G
				 {   
					 strncpy((char *)reddatalen,(getp+7),5);
					 datalen=atoi((char *)reddatalen);
				 }
				 if(datalen>255 ) datalen=255;
				 if(datalen >30)
				 {
					while(*getp++ != '{');
					getp--;
					strncpy((char *)transmit_cmd,(getp),datalen);				
					break;
				 }
			}
		}				 
	}
	
	
	if(datalen != 0 && (strstr((char *)transmit_cmd,"{") !=NULL) )
	{
		getp=0;
		getp =strstr(( char *)transmit_cmd, "\"AT\":" );
		if(getp!=NULL)
		{
			while(*getp++ != ':');
			da = atoi((char *)getp);
			if(device_info.save.srv_time != da)
			{
				device_info.save.srv_time = da; 
				if(device_info.save.srv_time<1 || device_info.save.srv_time>1440) 
					device_info.save.srv_time = 60;
				f_write = 1;
				Trace_log("修改AT\r\n");
			}
		}
		getp =strstr(( char *)transmit_cmd, "\"CT\":" );
		if(getp!=NULL)
		{
			while(*getp++ != ':');
			da = atoi((char *)getp);
			if(device_info.save.SENSOR_SAM_TIME != da)
			{
				device_info.save.SENSOR_SAM_TIME = da; 
				if(device_info.save.SENSOR_SAM_TIME<1 || device_info.save.SENSOR_SAM_TIME>250) 
					 device_info.save.SENSOR_SAM_TIME = 6;
				f_write = 1;
				Trace_log("修改CT\r\n");
			}
		}

		getp =strstr(( char *)transmit_cmd, "\"BR\":" );
		if(getp!=NULL)
		{
			while(*getp++ != ':');
			da = atoi(getp);
			if(da == 1)
			{
				f_write = 1;	
				device_vbat_reset();
				Trace_log("电池电量复位\r\n");
			}				
		}
   if(f_write == 1)
   {
	   f_write=0;  
     device_param_write(&device_info.save);
		 EC20_params();
   }
	 
	}
	return size;
}
/*****************************************************/
void EC20_4G_sever_leave(void)
{	
  if(device_info.save.tab_module == EC25 ||device_info.save.tab_module == EC25_G)
	uart_send_ack(&GPRS, "AT+QICLOSE=0\r\n", wireless_buf ,"OK", recive_len ,1000);
//	else   //2G
//  uart_send_ack(&GPRS, "AT+QICLOSE\r\n", wireless_buf2 ,"OK", recive_len ,1000);
	
}
/*****************************************************/
uint8_t ec20_tcp_databuf(void )
{
	uint16_t len = 0;
	uint32_t adds;
	uint8_t baoj_id,baoj_status,i;
	float baoj_limit_h,baoj_limit_l,baoj_limit_f,baoj_data;
	uint8_t shuju_ok = 0;
	
	//发送登录包
	Trace_log("发送登录包\r\n");
	memset(wireless_buf,0,sizeof(wireless_buf));
	sprintf((char *)wireless_buf,"%s",device_info.save.srv_login);
	len = strlen((char *)wireless_buf);				
	if( EC20_TCP_sever_data(wireless_buf, len)==FALSE)    //失败 
	{ 
		return FALSE;
	}
	
	if(device_info.Params_sever==1 )  //第一次发送同步数据
	{	
		device_info.Params_sever=0;
		EC20_params();//第一次同步数据
	}
	
	if(device_info.alarming_flag != 0 )      //报警
	{ 
		Trace_log("报警");
		while(	device_info.alarming_flag != 0)			
		{
			for(i=0;i<16;i++)
			{
				if(device_info.alarm_flag_ID.ch_status[i] == 1)
				{
					baoj_id = i;
					baoj_status = device_info.alarm_flag_STATUS.ch_status[i];
					baoj_limit_h = device_info.save.channel_up[i];
					baoj_limit_l = device_info.save.channel_down[i];
					baoj_limit_f = device_info.save.channel_f[i];
					baoj_data = wl_hist_data.data[i];
					break;
				}
			}		
			//////////////////数据发送部分/////////////////////////////////////////////
			sprintf((char *)wireless_buf,"{\"WRID\":%d,\"ACTION\":%d,\"WRDATA\":%.4f,\"LIMIT_H\":%.4f,\"LIMIT_L\":%.4f,\"LIMIT_F\":%.4f}",
			baoj_id,baoj_status,baoj_data,baoj_limit_h,baoj_limit_l,baoj_limit_f);	
		
//		Trace_log(wireless_buf);
			len = strlen((char *)wireless_buf);
					
			if(EC20_TCP_sever_data(wireless_buf,len)==FALSE)    //失败 
			{ 
				return FALSE;
			}
			device_info.alarming_flag--;
			device_info.alarm_flag_ID.ch_status[i] = 0;
			if(device_info.alarming_flag<=0 || device_info.alarming_flag>30)
				device_info.alarming_flag = 0;
		}
	}

	if(device_info.alarm_flag_STATUS.flasherror==0)   //flash没有故障
	{	
		adds = device_get_flahaddr_addr();  //读取最近一次历史数据，保持上报数据与存储数据一致性
		hist_read(&wl_hist_data, adds);		
		if(wl_hist_data.data_time.year == 255||	wl_hist_data.data_time.month == 255)
		{
			Trace_log("没有存储数据 发送实时数据\r\n");
			shuju_ok = 0;
		}
		else if(wl_hist_data.data_time.year != device_info.data_time.year ||	wl_hist_data.data_time.month != device_info.data_time.month || wl_hist_data.data_time.day !=device_info.data_time.day)
		{
			Trace_log("历史书存储错误 发送实时数据\r\n");
			shuju_ok = 0;
		}
		else
			shuju_ok = 1;
	}

		////历史补发	
  if(shuju_ok == 1)   //flash正常并且有历史存储    发送断点续传数据
	{	
		i=0;
		device_info.pend_num=device_pending_num_get();
		device_info.pend_addr=device_perv_addr_get();
		while(device_info.pend_num)
		{	
			Trace_log("历史补发\r\n");
			hist_read(&wl_hist_data, device_info.pend_addr);
			if(  wl_hist_data.data_time.year==0 && wl_hist_data.data_time.month==0)
			{
					device_info.pend_num = 0;	 
					device_pending_num_set();		
					break;
			}
			if(wl_hist_data.next > 0X400000)
			{
		   	device_info.pend_num =0;
				device_pending_num_set();	
			 break;
			}	  	      

			if(device_info.pend_num>1)  //两包数据打包发送
			{		
				sprintf((char *)wireless_buf,"{\"t\":\"20%02d%02d%02d%02d%02d%02d\",\"CH0\":%.2f,\"CH1\":%.2f,\"CH2\":%.2f,\"CH3\":%.2f,\"CH4\":%.4f,\"CH5\":%.4f,\"CH6\":%.4f,\"CH7\":%.4f,\"CH8\":%.4f,\"CH9\":%.4f,\"Q\":%d,\"R\":%d}",
				wl_hist_data.data_time.year,
				wl_hist_data.data_time.month,
				wl_hist_data.data_time.day,
				wl_hist_data.data_time.hour,
				wl_hist_data.data_time.min,
				wl_hist_data.data_time.sec,
				wl_hist_data.data[0],
				wl_hist_data.data[1],
				wl_hist_data.data[2],
				wl_hist_data.data[3],
				wl_hist_data.data[4],
				wl_hist_data.data[5],
				wl_hist_data.data[6],
				wl_hist_data.data[7],
				wl_hist_data.data[8],
				wl_hist_data.data[9],
				nb.nb_csq,
				nb.device_restart_num
				);						
	    								
        len = strlen((char *)wireless_buf);					
				if(EC20_TCP_sever_data(wireless_buf,len)==FALSE)    //失败 
				{ 
					return FALSE;
				}
			}	
			// 设置地址待发数量 /
		  device_info.pend_num = device_info.pend_num - 1;
	  	device_info.pend_addr = wl_hist_data.next;

		  device_perv_addr_set();	
		  device_pending_num_set();			
		}
	}
	
	if(shuju_ok == 0)
	{
		wl_hist_data.data_time.year = device_info.data_time.year;
		wl_hist_data.data_time.month = device_info.data_time.month;
		wl_hist_data.data_time.day = device_info.data_time.day;
		wl_hist_data.data_time.hour = device_info.data_time.hour;
		wl_hist_data.data_time.min = device_info.data_time.min;
		wl_hist_data.data_time.sec = device_info.data_time.sec;
		for(i=0;i<16;i++)
		{
			wl_hist_data.data[i] = device_info.channel_data[i];
		}
	}
	else
	{
		adds = device_get_flahaddr_addr();  //读取最近一次历史数据，保持上报数据与存储数据一致性
		hist_read(&wl_hist_data, adds);
	}		
		//正常数据上报
	Trace_log("实时数据发送\r\n");		
	sprintf((char *)wireless_buf,"{\"t\":\"20%02d%02d%02d%02d%02d%02d\",\"Q\":%d,\"R\":%d,\"CH0\":%.2f,\"CH1\":%.2f,\"CH2\":%.2f,\"CH3\":%.2f,\"CH4\":%.4f,\"CH5\":%.4f,\"CH6\":%.4f,\"CH7\":%.4f,\"CH8\":%.4f,\"CH9\":%.4f}",
			wl_hist_data.data_time.year,
			wl_hist_data.data_time.month,
			wl_hist_data.data_time.day,
			wl_hist_data.data_time.hour,
			wl_hist_data.data_time.min,
			wl_hist_data.data_time.sec,
			nb.nb_csq,
			nb.device_restart_num,
			wl_hist_data.data[0],
			wl_hist_data.data[1],
			wl_hist_data.data[2],
			wl_hist_data.data[3],
			wl_hist_data.data[4],
			wl_hist_data.data[5],
			wl_hist_data.data[6],
			wl_hist_data.data[7],
			wl_hist_data.data[8],
			wl_hist_data.data[9]);	

	len = strlen((char *)wireless_buf);		
	if( EC20_TCP_sever_data(wireless_buf, len)==FALSE)    //失败 
	{ 
		return FALSE;
	}
	
	return TRUE;	
}
/*****************************************************/
uint8_t hex_to_BCD(uint8_t data)
{
	uint8_t re=0;
	uint8_t h,l;
	
	h = data/10;
	l = data%10;	
	re = h*16+l;
	
	return re;
}
/*****************************************************/
uint8_t EC20_TCP_task()   //
{
	uint8_t send_ok = FALSE;
	PCIE_PW_CLOSE();		//模块关闭电源
	PCIE_PW_OPEN();           //模块上电
	delay_vtask(1000);
	Trace_log("TCP task\r\n");
	device_info.event_again = 0;
		if(!EC20_4G_int())
		{
			Trace_log("初始化完成\r\n");
		}
		else 
		{		 
			Trace_log("初始化失败\r\n");
			goto erracb;	
		}
	
		Trace_log("TCP链接\r\n");
		EC20_4G_sever_leave();

	  if(EC20_TCP_CONNET()==TRUE)
		{	 
			Trace_log("login successfully\r\n");		
		}
		else 
		{
			Trace_log("login again\r\n");
			EC20_4G_sever_leave();		  

		    if(EC20_TCP_CONNET()==TRUE)
		    {
		     	Trace_log("login successfully\r\n");		
		    }
				else
				{	
					Trace_log("login defeated\r\n");
				  goto erracb;
				}
		}		
		nb.nb_csq = (uint8_t)GET_CSQ();	
//		GET_IMEI((char *)nb.nb_imei,"EC20");

		send_ok = ec20_tcp_databuf();
		
		if(send_ok == TRUE)
		{
			EC20_recv_data();	  //解析接收数据	 
			EC20_4G_sever_leave();
		}
    else	 
		{			
		  goto erracb;
		}			

		SET_QPOWD();		
		PCIE_PW_CLOSE();		//模块关闭电源
    return 0;	
		
	erracb:
		PCIE_PW_CLOSE();		//模块关闭电源
	  Trace_log("发送失败，数据备份\r\n");
		if(	device_info.pend_num ==0)//数据备份做断点续传
		{	
			device_info.pend_addr=device_get_flahaddr_addr();
			device_perv_addr_set();
		}
		device_info.pend_num =device_info.pend_num + 1;
		device_pending_num_set();
	  nb.nb_csq = 0;
		return 1;	
		
}


/*****************************************************/
static void close_MQTT(void)
{
	uart_send_ack(&GPRS, "AT+QMTDISC=0\r\n", wireless_buf ,"OK", recive_len ,3000);
}
/*****************************************************/
uint8_t EC20_MQTT_task(void)
{
	uint8_t send_ok = FALSE;
//	PCIE_PW_CLOSE();		//模块关闭电源
	PCIE_PW_OPEN();           //模块上电
	delay_vtask(1000);
	Trace_log("MQTT task\r\n");
	device_info.event_again = 0;
	
		if(!EC20_4G_int())
		{
			Trace_log("初始化完成r\n");
		}
		else 
		{		 
			Trace_log("初始化失败\r\n");
			goto end_ec20;	
		}
	
		Trace_log("MOTT连接\r\n");
//		EC20_4G_sever_leave();  //关闭socket服务
		
		if(device_info.save.GPS_stu == 1)
			EC200S_GET_GNSS();

		EC20_4G_sever_leave();
		
	  	if(MQTT_CONNECT_LINE()==TRUE)    //连接服务器
		{	 
			Trace_log("login successfully\r\n");		
		}
		else 
		{
			goto end_ec20;
		}	
		
	  	if(device_info.Params_sever==1)  //第一次发送同步数据
    	{	 
			EC20_params();//第一次同步数据
		}		

		send_ok = EC20_4G_mqttdata();   //标准版数据先发实时数据再发历史数据
	  if(send_ok == TRUE)  //数据发送成功   
		{
			Trace_log("MQTT订阅\r\n");
			Tink_getParam();
			EC20_recv_data();	  //解析接收数据		
		  //	Trace_log("解析接收数据ovel\r\n");
		}
    else	 
		{			
		    goto end_ec20;
		}
		close_MQTT();
    SET_QPOWD();		
		PCIE_PW_CLOSE();		//模块关闭电源
		return 0;	
		
end_ec20:   //失败
		close_MQTT();
		SET_QPOWD();
		PCIE_PW_CLOSE();		//模块关闭电源
		if(device_info.save.device_dabao == OFF)
		{
			Trace_log("发送失败，数据备份\r\n");
			if(	device_info.pend_num ==0)//数据备份做断点续传
			{	
				device_info.pend_addr=device_get_flahaddr_addr();/////////////////////////
				device_perv_addr_set();
			}
			device_info.pend_num =device_info.pend_num + 1;
			device_pending_num_set();
		}
		else
			Trace_log("发送失败\r\n");
	  nb.nb_csq = 0;
		clear_allalarm();
		return 1;	
}
/*****************************************************/
uint8_t EC20_MQTT_task_s(void)    //不休眠秒级模式下
{
	if(device_info.mqtt_statue == 0)
	{
	  PCIE_PW_CLOSE();		//模块关闭电源
	  PCIE_PW_OPEN();           //模块上电
	  delay_vtask(1000);
		Trace_log("MQTT task\r\n");
	  device_info.event_again = 0;
		if(!EC20_4G_int())
		{
			Trace_log("初始化完成r\n");
			device_info.mqtt_statue = 1;
		}
		else 
		{		 
			Trace_log("初始化失败\r\n");
			goto end_ec20;	
		}
	}
	
	if(device_info.mqtt_statue == 1)
	{
		Trace_log("MOTT连接\r\n");
		EC20_4G_sever_leave();  //关闭socket服务

	  if(MQTT_CONNECT_LINE()==TRUE)    //连接服务器
		{	 
			Trace_log("login successfully\r\n");
			device_info.mqtt_statue = 2;		
		}
		else 
		{
			goto end_ec20;
		}
	}
 	
	if(device_info.mqtt_statue == 2)
	{
	  if(device_info.Params_sever==1 )  //第一次发送同步数据
    {	 
			device_info.Params_sever=0;
			EC20_params();//第一次同步数据
		}		
		
	  if( EC20_4G_mqttdata()==TRUE)  //实时数据	 
		{
			Trace_log("MQTT订阅\r\n");
			Tink_getParam();
			EC20_recv_data();	  //解析接收数据		
		  //	Trace_log("解析接收数据ovel\r\n");
		}
    else	 
		{			
		    goto end_ec20;
		}
		return 0;
	}		
		
end_ec20:   //失败
	SET_QPOWD();
	PCIE_PW_CLOSE();		//模块关闭电源
	device_info.mqtt_statue = 0;
	if(device_info.save.device_dabao == OFF)
	{
		Trace_log("发送失败，数据备份\r\n");
		if(	device_info.pend_num ==0)//数据备份做断点续传
		{	
			device_info.pend_addr=device_get_flahaddr_addr();///////////////*//////////
			device_perv_addr_set();
		}
		device_info.pend_num =device_info.pend_num + 1;
		device_pending_num_set();
	}
	else
		Trace_log("发送失败\r\n");
	nb.nb_csq = 0;
	clear_allalarm();
	return 1;	
}
/*****************************************************/



