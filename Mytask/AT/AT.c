#include "main.h"
#include "AT.h"

// uint8_t at_buf2[256];
 extern uint8_t wireless_buf[recive_len];   //接收数组
// extern uint8_t sevebuf[output_len];     //发送数组
 
/*******************************************************/
char TEST_AT(uint8_t ATT)   //检测AT命令
{
//  uint8_t check_count = 60;
	while(ATT-- )
	{
		if(!uart_send_ack(&GPRS, "AT\r\n", wireless_buf ,"OK", recive_len ,300))
			return 0;
		// Trace_log("TEST AT\r\n");
	}
	Trace_log("AT无响应，模块无应答\r\n");//网络模块异常
	return 1;
}
/*******************************************************/
char SET_ATW(void)     //AT&W
{
	char re;
	re = uart_send_ack(&GPRS, "AT&W\r\n", wireless_buf, "OK", recive_len, 300);
	return re;
	
}
/*******************************************************
ate 0  关闭    1  开启
*******************************************************/
char SET_ATE(char ate)   //设置回显
{
	char re;
	if(ate == 0)
	{
		re = uart_send_ack(&GPRS, "ATE0\r\n", wireless_buf, "OK", recive_len, 300);
		Trace_log("关闭回显\r\n");
	}
	else if(ate == 1)
	{
		re = uart_send_ack(&GPRS, "ATE1\r\n", wireless_buf, "OK", recive_len, 300);  //默认模式
		Trace_log("开启回显\r\n");
	}
	return re;
}
/*******************************************************/
char TEST_ATI(void)     //读取模块信息
{
	if(uart_send_ack(&GPRS, "ATI\r\n", wireless_buf, "OK", recive_len, 300))
	{
		// Trace_log("模块信息读取失败\r\n");
		return 1;
	}
	else
		return 0;
}
/*******************************************************
imei   字符串传递变量
lei    模块类型
*******************************************************/
void GET_IMEI(char *imei,char *lei)     //获取IMEI
{
	uint16_t rx_size;
	uint8_t wait_cnt = 4;//*temp_buf;
	uint8_t imei_size=0;
	char *p;
	// uart_transmit_str(&GPRS, (uint8_t *)"AT+CGSN=1\r\n");
	while(wait_cnt--)
	{	
		if(strcmp(lei,"BC26")==NULL)
		{
			p=0;
			uart_transmit_str(&GPRS, (uint8_t *)"AT+CGSN=1\r\n");
			rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,350);
			if(rx_size)
			{
				Trace_log(wireless_buf);
				if(strstr((char *)wireless_buf, "+CGSN:"))
				{
					p=strstr(( char *)wireless_buf, "+CGSN:" );
					if(device_info.save.tab_module == M101)    //多一个空格
						p++;
					else if(device_info.save.tab_module == BC95)    //少一个空格
						p--;
					strncpy((char*)imei,(char*)(p+7),15);		
					imei[16] = 0;
					break;
				}
			}
	  }
		else if(strcmp(lei,"EC20")==NULL || strcmp(lei,"EC200T")==NULL || strcmp(lei,"M26")==NULL)
		{
			uart_transmit_str(&GPRS, (uint8_t *)"AT+CGSN\r\n");
			rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,350);
			if(rx_size)
			{
				Trace_log(wireless_buf);
				if(strstr((char *)wireless_buf, "OK") && strstr((char *)wireless_buf, "CGSN") )
				{
					p = (char *)wireless_buf;
					while(*p++ != 0x0A);
					if(p[0]=='+')
					{
						break;
					}
					for(imei_size = 0; p[imei_size] != 0x0d; imei_size++)
					{
						imei[imei_size] = p[imei_size];
					}
					imei[imei_size]=0;
					break;
				}
			}
		}
	}
	uart_recv_data(&GPRS, wireless_buf, recive_len,500);
}
/*******************************************************/
void GET_IMSI(char *imsi)     //获取IMSI
{
	uint16_t rx_size;
	uint8_t wait_cnt =4, imsi_size = 0,*temp_buf;
	uart_transmit_str(&GPRS, (uint8_t *)"AT+CIMI\r\n");    //cimi
	while(wait_cnt--)
	{
		rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,300);
		if(rx_size)
		{
			Trace_log(wireless_buf);

			if(strstr((char *)wireless_buf, "OK"))
			{
				temp_buf = wireless_buf;				
				while(*temp_buf++ != 0x0A);
				for(imsi_size = 0; temp_buf[imsi_size] != 0x0d; imsi_size++)
				{
					imsi[imsi_size] = temp_buf[imsi_size];
				}
				imsi[imsi_size]=0;
				break;
			}		
		}
	}
	uart_recv_data(&GPRS, wireless_buf, recive_len,500);
}
/*******************************************************
iccid   字符串传递变量
lei    模块类型
*******************************************************/
void GET_ICCID(char *iccid,char *lei)    // 获取iccid
{

	uint16_t rx_size;
	uint8_t wait_cnt = 4;	
	uint8_t *p;	
	uint8_t iccid_size=0;

	if(device_info.save.tab_module == M101)
	{
		uart_transmit_str(&GPRS, (uint8_t *)"AT+ECICCID\r\n"); 
		while(wait_cnt--)
		{
			p=0;
			rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,5000);
			if(rx_size)
			{
				Trace_log(wireless_buf);
				if(strstr((char *)wireless_buf, "+ECICCID:"))
				{
					p=(uint8_t *)strstr(( char *)wireless_buf, "+ECICCID:" );
					
					strncpy(iccid,(char*)(p+10),20);		
					iccid[21] = 0;
					break;
				}
			}
		}
	}
	if(device_info.save.tab_module == BC95)
	{
		uart_transmit_str(&GPRS, (uint8_t *)"AT+NCCID\r\n"); 
		while(wait_cnt--)
		{
			p=0;
			rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,5000);
			if(rx_size)
			{
				Trace_log(wireless_buf);
				if(strstr((char *)wireless_buf, "+NCCID:"))
				{
					p=(uint8_t *)strstr(( char *)wireless_buf, "+NCCID:" );
					
					strncpy(iccid,(char*)(p+7),20);		
					iccid[21] = 0;
					break;
				}
			}
		}
	}
	else
	{
		uart_transmit_str(&GPRS, (uint8_t *)"AT+QCCID\r\n");  
		while(wait_cnt--)
		{	
			p=0;
			rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,500);
			if(rx_size)
			{
				Trace_log(wireless_buf);
				if(strcmp(lei,"EC20")==NULL || strcmp(lei,"EC200T")==NULL || strcmp(lei,"BC26")==NULL)
				{
					if(strstr((char *)wireless_buf, "+QCCID:"))
					{
						p=(uint8_t *)strstr(( char *)wireless_buf, "+QCCID:" );
						
						strncpy(iccid,(char*)(p+8),20);		
						iccid[21] = 0;
						break;
					}
				}
				else if(strcmp(lei,"M26")==NULL)
				{			
					if(strstr((char *)wireless_buf, "OK")  && strstr((char *)wireless_buf, "QCCID"))
					{
						p = wireless_buf;
						while(*p++ != 0x0A);
						for(iccid_size = 0; p[iccid_size] != 0x0d; iccid_size++)
						{
							iccid[iccid_size] = p[iccid_size];
						}
						iccid[iccid_size]=0;
						break;
					}
				}		
			}
		}
	}
	uart_recv_data(&GPRS, wireless_buf, recive_len,500);
}
/*******************************************************/
void M26_GET_ICCID(char *iccid)    //2G获取iccid
{
	uint16_t rx_size;
	uint8_t wait_cnt = 4;
	char *p;	
	uint8_t iccid_size=0;
	uart_transmit_str(&GPRS, (uint8_t *)"AT+QCCID\r\n");  
	while(wait_cnt--)
	{	
		p=0;
		rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,500);
		if(rx_size)
		{
			Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "OK")  && strstr((char *)wireless_buf, "QCCID"))
			{
				p = (char *)wireless_buf;
				while(*p++ != 0x0A);
				for(iccid_size = 0; p[iccid_size] != 0x0d; iccid_size++)
				{
					iccid[iccid_size] = p[iccid_size];
				}
				iccid[iccid_size]=0;
				break;
			}
		}
	}
}
/*******************************************************
cfun   默认设置1   启用全功能
*******************************************************/
char SET_CFUN(char cfun)   //设置启用性能
{
	char re;
	// uart_send_ack(&GPRS, "AT+CFUN=?\r\n", wireless_buf, "OK", recive_len, 15000);
	// uart_send_ack(&GPRS, "AT+CFUN?\r\n", wireless_buf, "OK", recive_len, 15000);
	if(cfun == 0)
	{
		// Trace_log("关闭全功能\r\n");
		re = uart_send_ack(&GPRS, "AT+CFUN=0\r\n", wireless_buf, "OK", recive_len, 15000);
	}
	else if(cfun == 1)
	{
		// Trace_log("开启全功能\r\n");
		re = uart_send_ack(&GPRS, "AT+CFUN=1\r\n", wireless_buf, "OK", recive_len, 15000);  //默认模式
	}
	else if(cfun == 4)
	{
		re = uart_send_ack(&GPRS, "AT+CFUN=4\r\n", wireless_buf, "OK", recive_len, 15000);  //默认模式
		// Trace_log("开启...\r\n");
	}
	else if(cfun == 7)
	{
		re = uart_send_ack(&GPRS, "AT+CFUN=7\r\n", wireless_buf, "OK", recive_len, 15000);  //默认模式
		// Trace_log("开启...\r\n");
	}
	return re;
}
/*******************************************************
*ipr  波特率
*******************************************************/
char SET_IPR(char *ipr)   //设置波特率   默认115200
{
	// uint8_t rx_size[20];AT+NATSPEED=9600,3,1,2,1
	// sprintf((char *)transmit_cmd, "AT+NATSPEED=9600,3,1,2,1\r\n");
	sprintf((char *)transmit_cmd, "AT+IPR=%s\r\n",ipr);
	if(uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "OK", recive_len, 300))
	{
		// Trace_log("设置波特率失败\r\n");
		return 1;
	}
	else
		return 0;
}
/*******************************************************/
char TEST_CPIN(void)     //检测SIM卡读取状态
{
	uint8_t check_count = 10;
	uint8_t check_result;
	while(check_count-- )
	{
		sprintf((char *)transmit_cmd, "AT+CPIN?\r\n");
		uart_transmit_str(&GPRS,transmit_cmd);
		check_result = check_recv3(&GPRS, wireless_buf ,"+CPIN: READY","+CPIN: SIM PIN","+CPIN: SIM PUK", recive_len ,300);
		if(check_result == 0)
		{
			Trace_log("SIM卡正常\r\n");
			return 0;
		}
		else if(check_result == 1)
		{
			if(strcmp((char *)device_info.save.srv_apn, "") != 0)
			{				
				sprintf((char *)transmit_cmd, "AT+CPIN=%s\r\n",device_info.save.srv_sim_pin);
				if(!uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "OK", recive_len, 350))
				{
					// Trace_log("设置PIN码成功\r\n");
				}
				else
				{
					// Trace_log("设置PIN码失败\r\n");
					return 1;
				}
			}
			else
				Trace_log("请输入PIN码\r\n");
		}
		else if(check_result == 2)
		{
			if(strcmp((char *)device_info.save.srv_apn, "") != 0)
			{				
				sprintf((char *)transmit_cmd, "AT+CPIN=\"%s\",\"%s\"\r\n",device_info.save.srv_sim_puk,device_info.save.srv_sim_pin);
				if(!uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "OK", recive_len, 350))
				{
					// Trace_log("设置PUK码成功\r\n");
				}
				else
				{
					// Trace_log("设置PUK码失败\r\n");
					return 1;
				}
			}
			else
				Trace_log("请输入PUK码\r\n");
		}
		delay_vtask(500);
	}
	Trace_log("SIM卡异常\r\n");
	return 1;
}
/*******************************************************
wait_count  检测次数
*******************************************************/
char TEST_CGATT(uint8_t wait_count)    //检测网络附着状态
{	
	while(wait_count--)
	{ 
		if( !uart_send_ack(&GPRS, "AT+CGATT?\r\n", wireless_buf, "OK", recive_len, 350))
		{
			// Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "0"))    //BC95返回数据少一个空格  +CGATT:0    其他模块返回+CGATT: 0
			{
				Trace_log( "网络附着中，请等待...\r\n");
			}
			else if(strstr((char *)wireless_buf, "1"))
			{
				Trace_log( "网络附着成功\r\n");
				return 0;
			}
		}
		delay_vtask(800);
	}
	return 1;
}
/*******************************************************
wait_count  检测次数
*******************************************************/
char TEST_CREG(uint8_t wait_count)     //检测网络注册状态
{
//	uint16_t rx_size;
	uint8_t cregerr = 0;
	
	while(wait_count--)
	{ 
		if( !uart_send_ack(&GPRS, "AT+CREG?\r\n", wireless_buf, "OK", recive_len, 350))
		{
			// Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "+CREG: 0,2"))
			{
				Trace_log( "CS业务网络注册中，请等待...\r\n");
			}
			else if(strstr((char *)wireless_buf, "+CREG: 0,1"))
			{
				Trace_log( "CS业务本地网络\r\n");
				return 0;
			}
			else if(strstr((char *)wireless_buf, "+CREG: 0,5"))
			{
				Trace_log( "CS业务漫游网络\r\n");
				return 0;
			}
			else if(strstr((char *)wireless_buf, "+CREG: 0,3"))
			{			
				Trace_log( "CS业务网络初始化失败,请检测SIM状态\r\n");//网络初始化失败,请检测SIM是否在有效期
				break;
			}
			else if(strstr((char *)wireless_buf, "+CREG: 0,4"))
			{
				Trace_log("未知错误\r\n");
				cregerr++;
			}
		}
		if(cregerr > 5)
				break;
		delay_vtask(800);
	}
	return 1;
}
/*******************************************************
wait_count  检测次数
*******************************************************/
char TEST_CEREG(uint8_t wait_count)     //检测网络注册状态
{
//	uint16_t rx_size;
	
	while(wait_count--)
	{ 
		if( !uart_send_ack(&GPRS, "AT+CEREG?\r\n", wireless_buf, "OK", recive_len, 350))
		{
			if(strstr((char *)wireless_buf, "0,2"))     //BC95返回数据少一个空格  +CEREG:0,2    其他模块返回+CEREG: 0,2
			{
				Trace_log( "PS业务网络注册中，请等待...\r\n");
			}
			else if(strstr((char *)wireless_buf, "0,1"))
			{
				Trace_log( "PS业务本地网络\r\n");
				wait_count = 0;
				return 0;
			}
			else if(strstr((char *)wireless_buf, "0,5"))
			{
				Trace_log( "PS业务漫游网络\r\n");
				wait_count = 0;
				return 0;
			}
			else if(strstr((char *)wireless_buf, "0,3"))
			{			
				Trace_log( "PS业务网络初始化失败,请检测SIM状态\r\n");//网络初始化失败,请检测SIM是否在有效期
				break;
			}
			else if(strstr((char *)wireless_buf, "0,4"))
			{
				Trace_log("未知错误\r\n");
				break;
			}
		}
		delay_vtask(800);
	}
	return 1;
}
/*******************************************************/
uint8_t GET_CSQ(void)     //检测信号强度
{
	char *buf_cache;
	uint8_t csq=0;
  char signalnum=10;
	while(signalnum--)
	{
		delay_vtask(500);
		if( !uart_send_ack(&GPRS, "AT+CSQ\r\n", wireless_buf, "OK", recive_len, 300))
		{
			buf_cache = strstr((const char *)wireless_buf, "+CSQ:" );
			buf_cache += 5;
//			if(*buf_cache == ' ')      //BC95返回数据少一个空格  +CSQ:8,99    其他模块返回+CSQ: 8,99
//				buf_cache++;
			csq = atoi(buf_cache);
			if(csq > 35)
			{
				csq = 0;
				continue;
			}
			else	if(csq == 0)
			{
				Trace_log("BC26没有信号\r\n");
			}
			else	if(csq<=5)
			{
				Trace_log("信号较弱，发送数据将不稳定\r\n");
				break;
			}
			
			if(csq<=35 && csq>0)
				break;							
		}
		else
			break;
	}	
	return csq;
}
/*******************************************************/
uint8_t GET_CESQ(void)    //检测信号质量  BC26
{
	char *buf_cache;
	uint8_t cesq=0;
  char signalnum=10;
	
	while(signalnum--)
	{
		if( !uart_send_ack(&GPRS, "AT+CESQ\r\n", wireless_buf, "OK", recive_len, 350))
		{
			buf_cache = strstr((const char *)wireless_buf, "+CESQ:" );
			{
				buf_cache += 7;
				cesq = atoi(buf_cache);
				if(cesq > 70)
				{
					cesq = 0;
				}
			}
			if(cesq<=5)
			{
				Trace_log("信号质量较差，发送数据将不稳定\r\n");
			  break;
			}
			else
				break;
		}
		else
			break;
		delay_vtask(300);
	}	
	return cesq;
}
/*******************************************************/
uint16_t GET_CBC(void)     //检测模块供电电压
{
	char *buf_cache;
	uint16_t cbc=0;
	if( !uart_send_ack(&GPRS, "AT+CBC\r\n", wireless_buf, "OK", recive_len, 300))
	{
		buf_cache = strstr((const char *)wireless_buf, "+CBC:" );
		{
			buf_cache += 10;
			cbc = atoi(buf_cache);
		}
	}
	#if 0
	if(cbc==0)
	{
		if( !uart_send_ack(&GPRS, "AT+CBC\r\n", wireless_buf, "+CBC: ", recive_len, 300))
		{
			buf_cache = strstr((const char *)wireless_buf, "+CBC:" );
		{
			buf_cache += 10;
			cbc = atoi(buf_cache);
		}
		}
	}
	#endif
	
	return cbc;
}
/*******************************************************/
char SET_QPOWD(void)    //模块命令关机
{
	char re;
	re = uart_send_ack(&GPRS, "AT+QPOWD=0\r\n", wireless_buf, "OK", recive_len, 1000);
	if(re == 1)
		Trace_log("模块关机失败");
	return re;
}
/*******************************************************/
char SET_QRST(void)   //模块指令重启   BC26有
{
	if(!uart_send_ack(&GPRS, "AT+QRST=1\r\n", wireless_buf ,"0000", recive_len ,350))
	{	
		return 1;
	}
	else		
		return 0;
}
/*******************************************************/
char SET_NRB(void)   //模块指令重启   BC95有
{
	if(!uart_send_ack(&GPRS, "AT+QRST=1\r\n", wireless_buf ,"0000", recive_len ,350))
	{	
		return 1;
	}
	else		
		return 0;
}
/*******************************************************/
#if 0
void TAB_RST(void)     //模块按键重启
{
	//拉低500ms开机 
	Trace_log("模块按键重启\r\n");
	
	LL_GPIO_ResetOutputPin(pwrio_gpio, pwrio_gpio_id);    
	delay_vtask(500); 
	LL_GPIO_SetOutputPin(pwrio_gpio, pwrio_gpio_id);
	delay_vtask(500);
	LL_GPIO_ResetOutputPin(pwrio_gpio, pwrio_gpio_id);
	delay_vtask(1000);
}
#endif
/*******************************************************/
void PCIE_PW_OPEN(void)    //打开PICE模块电源
{
	LL_GPIO_SetOutputPin(pwrpcie_gpio, pwrpcie_gpio_id); 	//模块上电
	Trace_log("打开模块电源\r\n");
	delay_vtask(1000);
}
/*******************************************************/
void PCIE_PW_CLOSE(void)    //关闭PICE模块电源
{
	LL_GPIO_ResetOutputPin(pwrpcie_gpio, pwrpcie_gpio_id); 	//模块断电
	Trace_log("关闭模块电源\r\n");
	delay_vtask(1000);
}
/*******************************************************/
char SET_QBAND(char qband)    //设置运营商信息  NBIOT模块
{
//	uart_send_ack(&GPRS, "AT+NBAND=?\r\n", wireless_buf, "OK", recive_len, 300);
//	uart_send_ack(&GPRS, "AT+NBAND?\r\n", wireless_buf, "OK", recive_len, 300);
	if(qband == 3)
	{
		if(device_info.save.tab_module == BC95)
			strcpy((char *)transmit_cmd,"AT+NBAND=3\r\n");
		else
			strcpy((char *)transmit_cmd,"AT+QBAND=1,3\r\n");    //锁定中国联通频段
	}
	else if(qband == 5)
	{
		if(device_info.save.tab_module == BC95)
			strcpy((char *)transmit_cmd,"AT+NBAND=5\r\n");
		else
			strcpy((char *)transmit_cmd,"AT+QBAND=1,5\r\n");    //锁定中国电信频段
	}
	else if(qband == 8)
	{
		if(device_info.save.tab_module == BC95)
			strcpy((char *)transmit_cmd,"AT+NBAND=8\r\n");
		else
			strcpy((char *)transmit_cmd,"AT+QBAND=1,8\r\n");   //锁定中国移动频段
	}
	else
	{
		if(device_info.save.tab_module == BC95)
			return 1;
		else
			strcpy((char *)transmit_cmd,"AT+QBAND=0\r\n");      //不锁定频段
	}
	
	if(!uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300))
	{	
		return 1;
	}
	else		
		return 0;
		
}
/******************************************************************/
char SET_QRELLOCK(void)        //AT命令快速解锁   NBIOT模块
{
	if(!uart_send_ack(&GPRS, "AT+QRELLOCK\r\n", wireless_buf ,"OK", recive_len ,350))
	{	
		return 1;
	}
	else		
		return 0;
}
/******************************************************************/
uint8_t strstrlen(uint8_t *str1, uint8_t *str2, uint16_t len)  //字符串对比   
{
	uint16_t i = 0, j = 0, k = 0;
	for(; i < len; i++)
	{
		for(j = i; str1[j] == str2[k] && str2[k] != '\0';j++ ,k++ );
		
		if(j > 0 && str2[k] == '\0')
		{
			return 0;
		}
		k = 0;
	}
	return 1;
}
/******************************************************************
*uart  COM口
*recv_buf   数据存储数组
*str0     验证数组0   验证正确返回0
*str1     验证数组1   验证正确返回1
rd_size    接收数组长度空间
wait_tick    等待延时n ms
*****************************************************************/
uint8_t check_recv2(uart_t *uart, uint8_t *recv_buf ,const char *str0,const char *str1, uint16_t rd_size ,uint16_t wait_tick)
{
	
	uint16_t rx_size = 0;
	uint8_t ret =2;
			
	while(wait_tick--)
	{
		rx_size = uart_recv_data(uart, recv_buf ,rd_size, 1 );
		
		if(rx_size)
		{
		
		 Trace_log((uint8_t *)recv_buf); ////////***打印串口1数据******///
			if(strstr((const char *)recv_buf, str0) != NULL)
			{
				ret = 0;
				break;
			}
			else if(strstr((const char *)recv_buf, str1) != NULL)
			{	
				ret = 1;
				break;
			}
		}
//		delay_vtask(1);  //ms延时
	}
	return ret;
}	
/******************************************************************
*uart  COM口
*recv_buf   数据存储数组
*str0     验证数组0   验证正确返回0
*str1     验证数组1   验证正确返回1
*str2     验证数组2   验证正确返回2
rd_size    接收数组长度空间
wait_tick    等待延时n ms
******************************************************************/
uint8_t check_recv3(uart_t *uart, uint8_t *recv_buf ,const char *str0,const char *str1,const char *str2, uint16_t rd_size ,uint16_t wait_tick)
{
	
	uint16_t rx_size = 0;
	uint8_t ret =3;
			
	while(wait_tick--)
	{
		rx_size = uart_recv_data(uart, recv_buf ,rd_size, 1 );
		
		if(rx_size)
		{
		
		 Trace_log((uint8_t *)recv_buf); ////////***打印串口1数据******///
			if(strstr((const char *)recv_buf, str0) != NULL)
			{
				ret = 0;
				break;
			}
			else if(strstr((const char *)recv_buf, str1) != NULL)
			{	
				ret = 1;
				break;
			}
			else if(strstr((const char *)recv_buf, str2) != NULL)
			{	
				ret = 2;
				break;
			}
		}
//		delay_vtask(1);  //ms延时
	}
	return ret;
}
/******************************************************************/


