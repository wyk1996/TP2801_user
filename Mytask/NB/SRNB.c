#include "main.h"



/*********************************************************/
static uint8_t srnb_open(void)
{
//	TAB_RST();   ///  软重启
//	LL_GPIO_ResetOutputPin(pwrio_gpio, pwrio_gpio_id);    
//	delay_vtask(500); 
//	LL_GPIO_SetOutputPin(pwrio_gpio, pwrio_gpio_id);
//	delay_vtask(1200);
//	LL_GPIO_ResetOutputPin(pwrio_gpio, pwrio_gpio_id);
//	delay_vtask(1000);
	if(TEST_AT(20) != 0)   //检测AT
	{
			return FALSE;
	}
	Trace_log("模块开机成功\r\n");
	if(TEST_CPIN())    //检测SIM卡
		return FALSE;
	SET_ATE(1);
	return TRUE;
}
/*********************************************************/
uint8_t srnb_init(void)    //模块初始化
{
	
	uart_send_ack(&GPRS, "AT+CPSMS=0\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500);    //关闭PSM模式
	uart_send_ack(&GPRS, "AT+CEDRXS=0,5,\"0101\"\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500);   //关闭eDRX
	uart_send_ack(&GPRS, "AT+CGMR\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500);     //读取模块信息
	
	if(TEST_CEREG(130))
	{
		Trace_log("模块注册网络失败\r\n");
		return 1;
	}
	
	if(TEST_CGATT(80))
	{
		Trace_log("网络附着失败\r\n");
		return 1;
	}	
   ///AT+CREG?   90s内注册不上网络即可结束
  	
	nb.nb_csq = GET_CSQ();  //CSQ     获取信号强度
	if(nb.nb_csq == 0)
	{
		return 1;
	}	
	else  
		nb.nb_cesq = GET_CESQ();     //获取信号质量
	
	uart_send_ack(&GPRS, "AT+CGPADDR=0\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500);  //显示 PDP 地址
	uart_send_ack(&GPRS, "AT+CSCON?\r\n", wireless_buf, "OK", sizeof(wireless_buf), 5000);  //信令连接状态
	
	Trace_log("模块初始化成功\r\n");
	return 0;	
}

/*********************************************************/
uint8_t SRNB_link_sever(void )   //TCP/UDP平台链接
{
	uint8_t rx_size_ack;
	char *ackp;
	uart_send_ack(&GPRS, "AT+ECSTATUS=PHY\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500);  //信令连接状态
	sprintf((char *)transmit_cmd, "AT+CTM2MSETPM=\"%s\",%s,86400\r\n", device_info.save.srv_addr,device_info.save.srv_port);    //set connect ip and port
	uart_send_ack(&GPRS, transmit_cmd , wireless_buf, "OK", sizeof(wireless_buf), 300);
	if(!uart_send_ack(&GPRS, "AT+CTM2MREG\r\n", wireless_buf, "+CTM2M: reg,0", sizeof(wireless_buf), 5000)) //发起注册
	{
		Trace_log("注册成功\r\n");
	}
	else
		return FALSE;
//	rx_size_ack = uart_recv_data(&GPRS, wireless_buf2, recive_len,5000);
	if(!uart_send_ack(&GPRS, "", wireless_buf, "+CTM2M: obsrv,0", sizeof(wireless_buf), 5000))
	{
		Trace_log("订阅成功\r\n");
	}
	else
		return FALSE;
	
	sprintf((char *)transmit_cmd, "RL+TIME=%02d%02d%02d%02d%02d%02d\r\n", device_info.data_time.year,
					device_info.data_time.month,
					device_info.data_time.day,
					device_info.data_time.hour,
					device_info.data_time.min,
					device_info.data_time.sec);
	Trace_log(transmit_cmd);
	if(uart_send_ack(&GPRS, transmit_cmd , wireless_buf, "+TIME:OK", sizeof(wireless_buf), 30000))
	{
		Trace_log("连接失败\r\n");
		return FALSE;
	}
	Trace_log("连接成功\r\n");	
	return TRUE;
}
/*********************************************************/
uint8_t  srnb_sevebuf(void )   //NBIOT平台发送数据
{			
	uint8_t j,k;
	uint16_t i;
	uint8_t len_add;
	uint16_t da;
	uint32_t adds;
	uint16_t BUFSIZ1;
	char *ackp;
	uint8_t shuju_ok = 0;
	memset(transmit_cmd,0,sizeof(transmit_cmd));
	
	device_info.Params_sever=0;	
/*	
	if(device_info.w24q32flash_err==1)   //flash故障
	{
		wl_hist_data.data_time.year = device_info.data_time.year;
		wl_hist_data.data_time.month = device_info.data_time.month;
		wl_hist_data.data_time.day = device_info.data_time.day;
		wl_hist_data.data_time.hour = device_info.data_time.hour;
		wl_hist_data.data_time.min = device_info.data_time.min;
		wl_hist_data.data_time.sec = device_info.data_time.sec;
		wl_hist_data.data.ch1_value = MPAbuf.sensor2data;
		wl_hist_data.data.ch2_value = MPAbuf.sensor1data;
	}
	else
	{
		adds = device_get_flahaddr_addr();  //读取最近一次历史数据，保持上报数据与存储数据一致性
		hist_read(&wl_hist_data, adds);
	}
	if(wl_hist_data.data_time.year == 255||	wl_hist_data.data_time.month == 255)
	{
		Trace_log("没有存储数据 发送实时数据\r\n");
		shuju_ok = 1;
		wl_hist_data.data_time.year = device_info.data_time.year;
		wl_hist_data.data_time.month = device_info.data_time.month;
		wl_hist_data.data_time.day = device_info.data_time.day;
		wl_hist_data.data_time.hour = device_info.data_time.hour;
		wl_hist_data.data_time.min = device_info.data_time.min;
		wl_hist_data.data_time.sec = device_info.data_time.sec;
		wl_hist_data.data.ch1_value = MPAbuf.sensor2data;
		wl_hist_data.data.ch2_value = MPAbuf.sensor1data;
	}
	else if(wl_hist_data.data_time.year != device_info.data_time.year ||	wl_hist_data.data_time.month != device_info.data_time.month || wl_hist_data.data_time.day !=device_info.data_time.day)
	{
		Trace_log("历史书存储错误 发送实时数据\r\n");
		shuju_ok = 2;
		wl_hist_data.data_time.year = device_info.data_time.year;
		wl_hist_data.data_time.month = device_info.data_time.month;
		wl_hist_data.data_time.day = device_info.data_time.day;
		wl_hist_data.data_time.hour = device_info.data_time.hour;
		wl_hist_data.data_time.min = device_info.data_time.min;
		wl_hist_data.data_time.sec = device_info.data_time.sec;
		wl_hist_data.data.ch1_value = MPAbuf.sensor2data;
		wl_hist_data.data.ch2_value = MPAbuf.sensor1data;
	}
	
	i = 0;
	transmit_cmd[i++] = 0x68;      //起始符
	transmit_cmd[i++] = 0x00;      //附加信息
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x01;      //协议版本号
	transmit_cmd[i++] = 0x01;      //厂商代码
	transmit_cmd[i++] = 0x07;      //命令码
	transmit_cmd[i++] = 0x00;      //加密标志
	for(j=0;j<20;j++)         //ICCID
		transmit_cmd[i++] = nb.nb_iccid[j];
	len_add = i;
	transmit_cmd[i++] = 0x00;    //数据长度
	transmit_cmd[i++] = 0x00;
	//数据区
	if(strlen((char *)device_info.save.srv_login)<16)     //表号
		k = 16-strlen((char *)device_info.save.srv_login);
	for(j=0;j<k;j++)
		transmit_cmd[i++] = 0x20;
	for(j=0;j<strlen((char *)device_info.save.srv_login);j++)
		transmit_cmd[i++] = device_info.save.srv_login[j];
	
	transmit_cmd[i++] = 0x01;    //上报周期   分钟
	transmit_cmd[i++] = device_info.save.srv_time;       //周期值
	transmit_cmd[i++] = 0x00;   //上报时间
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = hex_to_BCD(wl_hist_data.data_time.year);    //抄表时间 6字节  16进制BCD码表示  2014-5-23 10:00:00 140523100000
	transmit_cmd[i++] = hex_to_BCD(wl_hist_data.data_time.month);
	transmit_cmd[i++] = hex_to_BCD(wl_hist_data.data_time.day);
	transmit_cmd[i++] = hex_to_BCD(wl_hist_data.data_time.hour);
	transmit_cmd[i++] = hex_to_BCD(wl_hist_data.data_time.min);
	transmit_cmd[i++] = hex_to_BCD(wl_hist_data.data_time.sec);
	transmit_cmd[i++] = 0x00;      //标总量
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;     //工况总量
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;     //工况瞬时流量
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	
	da = (uint16_t)(wl_hist_data.data.ch2_value*10);    //温度
	transmit_cmd[i++] = da;
	transmit_cmd[i++] = da>>8;
	da = (uint16_t)(wl_hist_data.data.ch1_value*10);     //压力
//	da = 2345;
	transmit_cmd[i++] = da;
	transmit_cmd[i++] = da>>8;
	
	da = (113+nb.nb_csq)/2;        //信号强度
	transmit_cmd[i++] = da;
	transmit_cmd[i++] = 0x06;      //表具类型
	transmit_cmd[i++] = 0x00;      //阀门状态
	transmit_cmd[i++] = 0x02;      //供电类型   锂电池
	da = (uint16_t)device_info.vbat_bfb;
	transmit_cmd[i++] = da;        //电池1电池电量百分比
	transmit_cmd[i++] = 0x00;      //电池2电池电量百分比
	
	transmit_cmd[i++] = 0x0f;    //IMSI长度
	transmit_cmd[i++] = 0x00;     
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;     
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	
	transmit_cmd[i++] = 0x00;     //最新余额
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	
	transmit_cmd[i++] = 0x00;     //最新结算读数
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	
	transmit_cmd[i++] = 0x00;     //最新结算时间
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	
	transmit_cmd[i++] = 0x00;     //空
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;     
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	
	da = i+3;
	transmit_cmd[len_add] = da;
	transmit_cmd[len_add+1] = da>>8;
	
	da = crc_16(transmit_cmd,i);
	transmit_cmd[i++] = da;
	transmit_cmd[i++] = da>>8;
	transmit_cmd[i++] = 0x16;
	
	str_to_hex16(wireless_buf,transmit_cmd,i);
	sprintf((char *)transmit_cmd, "AT+CTM2MSEND=%s\r\n",wireless_buf);  //
	Trace_log("等待发送\r\n");
	uart_send_ack(&GPRS, "", wireless_buf, "CTM2M", sizeof(wireless_buf), 500);   //清空一下接收缓存区
	if(!uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "+CTM2MSEND:", sizeof(wireless_buf), 40000))
	{		
//			uart_send_ack(&GPRS, "", wireless_buf2, "OK", recive_len, 10000);
		if(strstr((const char *)wireless_buf, "+CTM2M: send,") != NULL)
		{
			Trace_log("发送数据完成\r\n");
		}
		else 
		{
			Trace_log("发送数据失败\r\n");
			return FALSE;
		}

		device_info.Params_sever=0;	
	}
	else
	{
		Trace_log("发送失败\r\n");
		return FALSE;	
	}
	
	if(!uart_send_ack(&GPRS, "", wireless_buf, "+CTM2MRECV:", recive_len, 20000))
	{
		Trace_log(wireless_buf);
		ackp = strstr((char *)wireless_buf,"+CTM2MRECV:");
		while(*ackp++ != ':');
		if(ackp[1] == 0x36 && ackp[2] == 0x38)
		{
			if(ackp[95] == 0x30 && ackp[96] == 0x31)
			{
				Trace_log("服务器验证完成并保存\r\n");
			}
			else
			{
				Trace_log("服务器验证失败\r\n");
				return FALSE;
			}
		}
		else
		{
			Trace_log("收到应答错误，发送失败\r\n");
			return FALSE;
		}		
	}
	else
		Trace_log("没有收到应答\r\n");
	*/
	return TRUE;
}
/*********************************************************/

uint8_t task_srnb(void)
{
	uint8_t da1,da2;
	uint8_t again_tosever = 0;
	uint8_t rets = FALSE;
	uint8_t GNSS_WORK = 0;
	begin_nb:
	PCIE_PW_OPEN(); 	//模块上电	
	Trace_log("设备网络重启\r\n");

	if(srnb_open() == TRUE)   //设备软启动，开机失败，直接结束
  {					
		if(!srnb_init())	  //网络注册，初始化成功
		{	
			if((nb.nb_imei[0]<'0' || nb.nb_imei[0]>'9')|| (nb.nb_iccid[0]<'0'||nb.nb_iccid[0]>'9')||(nb.nb_imei[0]<'0'||nb.nb_imei[0]>'9'))  //获取模块信息
			{
				Trace_log("获取模块信息\r\n");
				get_nb_message();				
			}
			
			delay_vtask(100);				 
		  if(SRNB_link_sever() == TRUE)   //连接服务器成功
		  {
				rets = srnb_sevebuf();
			  if(rets == FALSE)	  //发送数据失败   
			  {
					again_tosever++;
			  }
        else
          rets = TRUE;					
		  }
      else 
				again_tosever++;			
	  }
		else
			again_tosever=0;	
     if(again_tosever==1)
		 {
//			 again_tosever=0;	
//			 nb_OFF();
			 PCIE_PW_CLOSE(); 	//模块断电					 
			 goto begin_nb;   //重启
		 }		
	}
   			  
//	nb_OFF(); 
	PCIE_PW_CLOSE();	//模块断电
	
	if(rets == FALSE)
	{
	  Trace_log("发送失败\r\n");
		nb.nb_csq = 0;
		clear_allalarm();
  }
	else
		Trace_log("发送成功\r\n");
	
	return rets;
}

