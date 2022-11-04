#include "main.h"
#include "lorawan_yxd.h"


lorawan_at lorawan;
uint8_t lorawan_first = 0;

/*********************��������***********************************************************/
static uint8_t lorawan_save(void)
{
	Trace_log("AT+SAVE\r\n");
	if(!uart_send_ack(&GPRS, "AT+SAVE\r\n", wireless_buf ,"OK", recive_len ,300))
			return TRUE;
  else
		return FALSE;
}
/*********************��������ģ��***********************************************************/
static uint8_t lorawan_reset(void)
{
	Trace_log("AT+RESET\r\n");
	if(!uart_send_ack(&GPRS, "AT+RESET\r\n", wireless_buf ,"OK", recive_len ,300))
			return TRUE;
  else
		return FALSE;
}
/*********************��������***********************************************************/
static uint8_t lorawan_set_echo(void)
{
	Trace_log("AT+ECHO=0\r\n");
	if(!uart_send_ack(&GPRS, "AT+ECHO=0\r\n", wireless_buf ,"OK", recive_len ,300))
			return TRUE;
  else
		return FALSE;
}
/*********************���û���ģʽ***********************************************************/
static uint8_t lorawan_set_SM(void)
{
	Trace_log("AT+SM=1\r\n");
	if(!uart_send_ack(&GPRS, "AT+SM=1\r\n", wireless_buf ,"OK", recive_len ,300))
			return TRUE;
  else
		return FALSE;
}
/*********************����AT����***********************************************************/
static uint8_t lorawan_testAT(void)
{
	uint8_t ATT=5;
	while(ATT-- )
	{
		Trace_log("AT\r\n");
		LL_UART_TransmitData(UART1,0XFF);    //���ڻ���lorawan����
		if(!uart_send_ack(&GPRS, "AT\r\n", wireless_buf ,"OK", recive_len ,300))
		{
//			lorawan_set_echo();
//			lorawan_save();
			Trace_log("AT����\r\n");//����ģ���쳣
			return TRUE;
		}
	}
//	Uartx_Init(UART1,9600);//��ʼ��uart����
//	ATT = 5;
//	while(ATT-- )
//	{
//		Trace_log("AT\r\n");
//		if(!uart_send_ack(&GPRS, "AT\r\n", wireless_buf2 ,"OK", recive_len ,300))
//		{
//			uart_send_ack(&GPRS, "AT+BR=115200\r\n", wireless_buf ,"OK", recive_len ,300);
//  		Uartx_Init(UART1,115200);//��ʼ��uart����
//			lorawan_set_echo();
//			lorawan_save();		
//      Trace_log("AT����\r\n");//����ģ���쳣			
//	    return TRUE;
//		}
//	}
//	Uartx_Init(UART1,115200);//��ʼ��uart����
	Trace_log("AT����Ӧ��ģ����Ӧ��\r\n");//����ģ���쳣
	return FALSE;
}
/*********************��ȡDEVEUI***********************************************************/
static void lorawan_read_DEVEUI(void)
{
	uint8_t rx_size;
	uint8_t ATT=3;
	char *p;
	uint8_t i;
	
	while(ATT--)
	{
		rx_size = 0;
		p = 0;
		uart_transmit_str(&GPRS, (uint8_t *)"AT+DEVEUI?\r\n");
		Trace_log("AT+DEVEUI?\r\n");
		rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,350);
		if(rx_size)
		{
			Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "+DEVEUI:"))
			{
				p=strstr(( char *)wireless_buf, "+DEVEUI:" );
//				strncpy((char*)lorawan.DEVEUI,(char*)(p+8),16);	
        p = p+8;				
				for(i=0;i<16;i++)
				{
					if(*(p+i) != '\r')
					{
						device_info.save.srv_login[i] = *(p+i);
					}
					else
						break;
				}				
				device_info.save.srv_login[i] = '\0';
				break;
			}
		}
	}	
}
/*********************����DEVEUI***********************************************************/
static void lorawan_set_DEVEUI(void)
{
	memset(transmit_cmd,0,sizeof(transmit_cmd));
	sprintf((char *)transmit_cmd,"AT+DEVEUI=%s\r\n",device_info.save.srv_login);
	Trace_log(transmit_cmd);
	uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300);
}
/*********************��ȡAPPEUI***********************************************************/
static void lorawan_read_APPEUI(void)
{
	uint8_t rx_size;
	uint8_t ATT=3;
	char *p;
	uint8_t i;
	
	while(ATT--)
	{
		rx_size = 0;
		p = 0;
		uart_transmit_str(&GPRS, (uint8_t *)"AT+APPEUI?\r\n");
		Trace_log("AT+APPEUI?\r\n");
		rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,350);
		if(rx_size)
		{
			Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "+APPEUI:"))
			{
				p=strstr(( char *)wireless_buf, "+APPEUI:" );	
				p = p+8;				
				for(i=0;i<16;i++)
				{
					if(*(p+i) != '\r')
					{
						device_info.save.srv_UserName[i] = *(p+i);
					}
					else
						break;
				}				
				device_info.save.srv_UserName[i] = '\0';
				break;
			}
		}
	}	
}
/*********************����APPEUI***********************************************************/
static void lorawan_set_APPEUI(void)
{	
	memset(transmit_cmd,0,sizeof(transmit_cmd));
	sprintf((char *)transmit_cmd,"AT+APPEUI=%s\r\n",device_info.save.srv_UserName);
	Trace_log(transmit_cmd);
	uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300);
}
/*********************��ȡDEVADDR***********************************************************/
static void lorawan_read_DEVADDR(void)
{
	uint8_t rx_size;
	uint8_t ATT=3;
	char *p;
	uint8_t i;
	
	while(ATT--)
	{
		rx_size = 0;
		p = 0;
		uart_transmit_str(&GPRS, (uint8_t *)"AT+DEVADDR?\r\n");
		Trace_log("AT+DEVADDR?\r\n");
		rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,350);
		if(rx_size)
		{
			Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "+DEVADDR:"))
			{
				p=strstr(( char *)wireless_buf, "+DEVADDR:" );		
				p = p+9;				
				for(i=0;i<8;i++)
				{
					if(*(p+i) != '\r')
					{
						device_info.save.srv_addr[i] = *(p+i);
					}
					else
						break;
				}				
				device_info.save.srv_addr[i] = '\0';
				break;
			}
		}
	}	
}
/*********************����DEVADDR***********************************************************/
static void lorawan_set_DEVADDR(void)
{	
	if(lorawan.JOINMODE == 1)
	{
		memset(transmit_cmd,0,sizeof(transmit_cmd));
		sprintf((char *)transmit_cmd,"AT+DEVADDR=%s\r\n",device_info.save.srv_addr);
		Trace_log(transmit_cmd);
		uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300);
	}
}
/*********************��ȡAPPKEY***********************************************************
static void lorawan_read_APPKEY(void)
{
	uint8_t rx_size;
	uint8_t ATT=3;
	char *p;
	uint8_t i;
	
	while(ATT--)
	{
		rx_size = 0;
		p = 0;
		uart_transmit_str(&GPRS, (uint8_t *)"AT+APPKEY?\r\n");
		rx_size = uart_recv_data(&GPRS, wireless_buf2, recive_len,350);
		if(rx_size)
		{
			Trace_log(wireless_buf2);
			if(strstr((char *)wireless_buf2, "+APPKEY:"))
			{
				p=strstr(( char *)wireless_buf2, "+APPKEY:" );		
				p = p+8;				
				for(i=0;i<32;i++)
				{
					if(*(p+i) != '\r')
					{
						APPKEY[i] = *(p+i);
					}
					else
						break;
				}				
				APPKEY[i] = '\0';
				break;
			}
		}
	}	
}
*********************����APPKEY***********************************************************/
static void lorawan_set_APPKEY(void)
{	
	uint8_t unit[50];
	if(device_info.save.srv_apn[0] != '\0')
	{
		device_hex_to_string(device_info.save.srv_apn,unit,sizeof(device_info.save.srv_apn),50);
		memset(transmit_cmd,0,sizeof(transmit_cmd));
		sprintf((char *)transmit_cmd,"AT+APPKEY=%s\r\n",unit);
		Trace_log(transmit_cmd);
		uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300);
	}
}
/*********************��ȡNWKSKEY***********************************************************
static void lorawan_read_NWKSKEY(void)
{
	uint8_t rx_size;
	uint8_t ATT=3;
	char *p;
	uint8_t i;
	
	while(ATT--)
	{
		rx_size = 0;
		p = 0;
		uart_transmit_str(&GPRS, (uint8_t *)"AT+NWKSKEY?\r\n");
		rx_size = uart_recv_data(&GPRS, wireless_buf2, recive_len,350);
		if(rx_size)
		{
			Trace_log(wireless_buf2);
			if(strstr((char *)wireless_buf2, "+NWKSKEY:"))
			{
				p=strstr(( char *)wireless_buf2, "+NWKSKEY:" );		
				p = p+9;				
				for(i=0;i<32;i++)
				{
					if(*(p+i) != '\r')
					{
						lorawan.NWKSKEY[i] = *(p+i);
					}
					else
						break;
				}				
				lorawan.NWKSKEY[i] = '\0';
				break;
			}
		}
	}	
}
*********************����NWKSKEY***********************************************************/
static void lorawan_set_NWKSKEY(void)
{	
	uint8_t unit[50];
	if(device_info.save.srv_apn_user_name[0] != '\0')
	{
		device_hex_to_string(device_info.save.srv_apn_user_name,unit,sizeof(device_info.save.srv_apn_user_name),50);
		memset(transmit_cmd,0,sizeof(transmit_cmd));
		sprintf((char *)transmit_cmd,"AT+NWKSKEY=%s\r\n",unit);
		Trace_log(transmit_cmd);
		uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300);
	}
}
/*********************��ȡAPPSKEY***********************************************************
static void lorawan_read_APPSKEY(void)
{
	uint8_t rx_size;
	uint8_t ATT=3;
	char *p;
	uint8_t i;
	
	while(ATT--)
	{
		rx_size = 0;
		p = 0;
		uart_transmit_str(&GPRS, (uint8_t *)"AT+APPSKEY?\r\n");
		rx_size = uart_recv_data(&GPRS, wireless_buf2, recive_len,350);
		if(rx_size)
		{
			Trace_log(wireless_buf2);
			if(strstr((char *)wireless_buf2, "+APPSKEY:"))
			{
				p=strstr(( char *)wireless_buf2, "+APPSKEY:" );		
				p = p+9;				
				for(i=0;i<32;i++)
				{
					if(*(p+i) != '\r')
					{
						lorawan.APPSKEY[i] = *(p+i);
					}
					else
						break;
				}				
				lorawan.APPSKEY[i] = '\0';
				break;
			}
		}
	}	
}
*********************����APPSKEY***********************************************************/
static void lorawan_set_APPSKEY(void)
{	
	uint8_t unit[50];
	if(device_info.save.srv_apn_user_pass[0] != '\0')
	{
		device_hex_to_string(device_info.save.srv_apn_user_pass,unit,sizeof(device_info.save.srv_apn_user_pass),50);
		memset(transmit_cmd,0,sizeof(transmit_cmd));
		sprintf((char *)transmit_cmd,"AT+APPSKEY=%s\r\n",unit);
		Trace_log(transmit_cmd);
		uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300);
	}
}
/*********************��ȡNETID***********************************************************/
static void lorawan_read_NETID(void)
{
	uint8_t rx_size;
	uint8_t ATT=3;
	char *p;
	uint8_t i;
	
	while(ATT--)
	{
		rx_size = 0;
		p = 0;
		uart_transmit_str(&GPRS, (uint8_t *)"AT+NETID?\r\n");
		Trace_log("AT+NETID?\r\n");
		rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,350);
		if(rx_size)
		{
			Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "+NETID:"))
			{
				p=strstr(( char *)wireless_buf, "+NETID:" );
				p = p+7;				
				for(i=0;i<8;i++)
				{
					if(*(p+i) != '\r')
					{
						device_info.save.srv_Password[i] = *(p+i);
					}
					else
						break;
				}				
				device_info.save.srv_Password[i] = '\0';
				break;
			}
		}
	}	
}
/*********************��ȡCHGROUP***********************************************************/
static void lorawan_read_CHGROUP(void)
{
	uint8_t rx_size;
	uint8_t ATT=3;
	char *p;
	uint8_t i;
	
	while(ATT--)
	{
		rx_size = 0;
		p = 0;
		uart_transmit_str(&GPRS, (uint8_t *)"AT+CHGROUP?\r\n");
		Trace_log("AT+CHGROUP?\r\n");
		rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,350);
		if(rx_size)
		{
			Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "+CHGROUP:"))
			{
				p=strstr(( char *)wireless_buf, "+CHGROUP:" );
				while(*p++ != ':');
				i = atoi(p);
        if(i!=0)
        {
					lorawan.CHGROUP = i;
				}
				else
					lorawan.CHGROUP = 1;    //Ĭ��Ϊ1 CH0~CH7
				break;
			}
		}
	}	
}
/*********************����CHGROUP***********************************************************/
static void lorawan_set_CHGROUP(void)
{	
	memset(transmit_cmd,0,sizeof(transmit_cmd));
	sprintf((char *)transmit_cmd,"AT+CHGROUP=%d\r\n",lorawan.CHGROUP);
	Trace_log(transmit_cmd);
	uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300);
}
/*********************��ȡULDLMODE***********************************************************/
static void lorawan_read_ULDLMODE(void)
{
	uint8_t rx_size;
	uint8_t ATT=3;
	char *p;
	uint8_t i;
	
	while(ATT--)
	{
		rx_size = 0;
		p = 0;
		uart_transmit_str(&GPRS, (uint8_t *)"AT+ULDLMODE?\r\n");
		Trace_log("AT+ULDLMODE?\r\n");
		rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,350);
		if(rx_size)
		{
			Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "+ULDLMODE:"))
			{
				p=strstr(( char *)wireless_buf, "+ULDLMODE:" );
				while(*p++ != ':');
				i = atoi(p);
        if(i<2)
        {
					lorawan.ULDLMODE = i;
				}
				else
					lorawan.ULDLMODE = 0;    //Ĭ��Ϊ0 ��Ƶģʽ
				break;
			}
		}
	}	
}
/*********************����ULDLMODE***********************************************************/
static void lorawan_set_ULDLMODE(void)
{	
	memset(transmit_cmd,0,sizeof(transmit_cmd));
	sprintf((char *)transmit_cmd,"AT+ULDLMODE=%d\r\n",lorawan.ULDLMODE);
	Trace_log(transmit_cmd);
	uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300);
}
/*********************��ȡJOINMODE***********************************************************/
static void lorawan_read_JOINMODE(void)
{
	uint8_t rx_size;
	uint8_t ATT=3;
	char *p;
	uint8_t i;
	
	while(ATT--)
	{
		rx_size = 0;
		p = 0;
		uart_transmit_str(&GPRS, (uint8_t *)"AT+JOINMODE?\r\n");
		Trace_log("AT+JOINMODE?\r\n");
		rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,350);
		if(rx_size)
		{
			Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "+JOINMODE:"))
			{
				p=strstr(( char *)wireless_buf, "+JOINMODE:" );
				while(*p++ != ':');
				i = atoi(p);
        if(i<2)
        {
					lorawan.JOINMODE = i;
				}
				else
					lorawan.JOINMODE = 0;    //Ĭ��Ϊ0 OTAAģʽ
				break;
			}
		}
	}	
}
/*********************����JOINMODE***********************************************************/
static void lorawan_set_JOINMODE(void)
{	
	memset(transmit_cmd,0,sizeof(transmit_cmd));
	sprintf((char *)transmit_cmd,"AT+JOINMODE=%d\r\n",lorawan.JOINMODE);
	Trace_log(transmit_cmd);
	uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300);
}
/*********************��ȡCLASS***********************************************************/
static void lorawan_read_CLASS(void)
{
	uint8_t rx_size;
	uint8_t ATT=3;
	char *p;
	uint8_t i;
	
	while(ATT--)
	{
		rx_size = 0;
		p = 0;
		uart_transmit_str(&GPRS, (uint8_t *)"AT+CLASS?\r\n");
		Trace_log("AT+CLASS?\r\n");
		rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,350);
		if(rx_size)
		{
			Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "+CLASS:"))
			{
				p=strstr(( char *)wireless_buf, "+CLASS:" );
				while(*p++ != ':');
				i = atoi(p);
        if(i<3)
        {
					lorawan.CLASS = i;
				}
				else
					lorawan.CLASS = 0;    //Ĭ��Ϊ0 CLASS Aģʽ
				break;
			}
		}
	}	
}
/*********************����CLASS***********************************************************/
static void lorawan_set_CLASS(void)
{	
	memset(transmit_cmd,0,sizeof(transmit_cmd));
	sprintf((char *)transmit_cmd,"AT+CLASS=%d\r\n",lorawan.CLASS);
//	sprintf((char *)transmit_cmd,"AT+CLASS=2\r\n");
	Trace_log(transmit_cmd);
	uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300);
}
/*********************��ȡSTATUS***********************************************************/
static void lorawan_read_STATUS(void)
{
	uint8_t rx_size;
	uint8_t ATT=3;
	char *p;
	uint8_t i;
	
	while(ATT--)
	{
		rx_size = 0;
		p = 0;
		uart_transmit_str(&GPRS, (uint8_t *)"AT+STATUS?\r\n");
		Trace_log("AT+STATUS?\r\n");
		rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,350);
		if(rx_size)
		{
			Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "+STATUS:"))
			{
				p=strstr(( char *)wireless_buf, "+STATUS:" );
				while(*p++ != ':');
				i = atoi(p);
        if(i<9)
        {
					lorawan.STATUS = i;
				}
				break;
			}
		}
	}	
}
/*********************��ȡBAND***********************************************************/
static void lorawan_read_BAND(void)
{
	uint8_t rx_size;
	uint8_t ATT=3;
	char *p;
	uint8_t i;
	
	while(ATT--)
	{
		rx_size = 0;
		p = 0;
		uart_transmit_str(&GPRS, (uint8_t *)"AT+BAND?\r\n");
		Trace_log("AT+BAND?\r\n");
		rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,350);
		if(rx_size)
		{
			Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "+BAND:"))
			{
				p=strstr(( char *)wireless_buf, "+BAND:" );
				while(*p++ != ':');
				i = atoi(p);
        if(i<9)
        {
					lorawan.BAND = i;
				}
				else
					lorawan.BAND = 1;    //Ĭ��Ϊ1 CN470
				break;
			}
		}
	}	
}
/*********************����BAND***********************************************************/
static void lorawan_set_BAND(void)
{	
	memset(transmit_cmd,0,sizeof(transmit_cmd));
	sprintf((char *)transmit_cmd,"AT+BAND=%d\r\n",lorawan.BAND);
	Trace_log(transmit_cmd);
	uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300);
}
/*********************��ȡAPPPORT***********************************************************/
static void lorawan_read_APPPORT(void)
{
	uint8_t rx_size;
	uint8_t ATT=3;
	char *p;
	uint8_t i;
	
	while(ATT--)
	{
		rx_size = 0;
		p = 0;
		uart_transmit_str(&GPRS, (uint8_t *)"AT+APPPORT?\r\n");
		Trace_log("AT+APPPORT?\r\n");
		rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,350);
		if(rx_size)
		{
			Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "+APPPORT:"))
			{
				p=strstr(( char *)wireless_buf, "+APPPORT:" );
				while(*p++ != ':');
				i = atoi(p);
        if(i<224)
        {
					lorawan.APPPORT = i;
				}
				else
					lorawan.APPPORT = 1;    //Ĭ��Ϊ1 
				break;
			}
		}
	}	
}
/*********************����APPPORT***********************************************************/
static void lorawan_set_APPPORT(void)
{	
	memset(transmit_cmd,0,sizeof(transmit_cmd));
	sprintf((char *)transmit_cmd,"AT+APPPORT=%d\r\n",lorawan.APPPORT);
	Trace_log(transmit_cmd);
	uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300);
}
/*********************��ȡDATARATE***********************************************************/
static void lorawan_read_DATARATE(void)
{
	uint8_t rx_size;
	uint8_t ATT=3;
	char *p;
	uint8_t i;
	
	while(ATT--)
	{
		rx_size = 0;
		p = 0;
		uart_transmit_str(&GPRS, (uint8_t *)"AT+DATARATE?\r\n");
		Trace_log("AT+DATARATE?\r\n");
		rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,350);
		if(rx_size)
		{
			Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "+DATARATE:"))
			{
				p=strstr(( char *)wireless_buf, "+DATARATE:" );
				while(*p++ != ':');
				i = atoi(p);
        if(i<6)
        {
					lorawan.DATARATE = i;
				}
				else
					lorawan.DATARATE = 0;    //Ĭ��Ϊ0
				break;
			}
		}
	}	
}
/*********************����DATARATE***********************************************************/
static void lorawan_set_DATARATE(void)
{	
	memset(transmit_cmd,0,sizeof(transmit_cmd));
	sprintf((char *)transmit_cmd,"AT+DATARATE=%d\r\n",lorawan.DATARATE);
	Trace_log(transmit_cmd);
	uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300);
}
/*********************��ȡRSSI***********************************************************/
static void lorawan_read_RSSI(void)
{
	uint8_t rx_size;
	uint8_t ATT=3;
	char *p;
	uint8_t i;
	
	while(ATT--)
	{
		rx_size = 0;
		p = 0;
		uart_transmit_str(&GPRS, (uint8_t *)"AT+RSSI?\r\n");
		Trace_log("AT+RSSI?\r\n");
		rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,350);
		if(rx_size)
		{
			Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "+RSSI:"))
			{
				p=strstr(( char *)wireless_buf, "+RSSI:" );
				while(*p++ != ':');
				i = atoi(p);
        nb.nb_csq = 200+i;
				break;
			}
		}
	}	
}
/*********************��ȡTXPWR***********************************************************/
static void lorawan_read_TXPWR(void)
{
	uint8_t rx_size;
	uint8_t ATT=3;
	char *p;
	uint8_t i;
	
	while(ATT--)
	{
		rx_size = 0;
		p = 0;
		uart_transmit_str(&GPRS, (uint8_t *)"AT+TXPWR?\r\n");
		Trace_log("AT+TXPWR?\r\n");
		rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,350);
		if(rx_size)
		{
			Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "+TXPWR:"))
			{
				p=strstr(( char *)wireless_buf, "+TXPWR:" );
				while(*p++ != ':');
				i = atoi(p);
        lorawan.TXPWR = i;
				break;
			}
		}
	}	
}
/*********************����TXPWR***********************************************************/
static void lorawan_set_TXPWR(void)
{	
	memset(transmit_cmd,0,sizeof(transmit_cmd));
	if(lorawan.TXPWR>20)
		sprintf((char *)transmit_cmd,"AT+TXPWR=%d,1\r\n",lorawan.TXPWR);  //����ǿ���书��
  else
	  sprintf((char *)transmit_cmd,"AT+TXPWR=%d\r\n",lorawan.TXPWR);
	Trace_log(transmit_cmd);
	uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300);
}
/*********************��ȡADR***********************************************************/
static void lorawan_read_ADR(void)
{
	uint8_t rx_size;
	uint8_t ATT=3;
	char *p;
	uint8_t i;
	
	while(ATT--)
	{
		rx_size = 0;
		p = 0;
		uart_transmit_str(&GPRS, (uint8_t *)"AT+ADR?\r\n");
		Trace_log("AT+ADR?\r\n");
		rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,350);
		if(rx_size)
		{
			Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "+ADR:"))
			{
				p=strstr(( char *)wireless_buf, "+ADR:" );
				while(*p++ != ':');
				i = atoi(p);
				if(i<2)
          lorawan.ADR = i;
				else
					lorawan.ADR = 1;    //Ĭ�Ͽ���
				break;
			}
		}
	}	
}
/*********************����ADR***********************************************************/
static void lorawan_set_ADR(void)
{	
	memset(transmit_cmd,0,sizeof(transmit_cmd));
	sprintf((char *)transmit_cmd,"AT+ADR=%d\r\n",lorawan.ADR);
	Trace_log(transmit_cmd);
	uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300);
}
/*********************����HOST***********************************************************/
static void lorawan_set_HOST(void)
{	
	memset(transmit_cmd,0,sizeof(transmit_cmd));
	sprintf((char *)transmit_cmd,"AT+HOTS=1\r\n");
	Trace_log(transmit_cmd);
	uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300);
}
/*********************����JOIN***********************************************************/
static void lorawan_set_JOIN(void)
{	
	memset(transmit_cmd,0,sizeof(transmit_cmd));
	sprintf((char *)transmit_cmd,"AT+JOIN=0,1\r\n");
	Trace_log(transmit_cmd);
	uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300);
}
/*********************�ȴ�lorawan����***********************************************************/
static uint8_t wait_lorawan_net_connnect(void)
{
	uint8_t rx_size;
	uint8_t wait_time=50;
	Trace_log("�ȴ�lorawan����\r\n");//����ģ���쳣
  lorawan_reset();
	while(wait_time--)
	{
		rx_size = uart_recv_data(&GPRS, wireless_buf, recive_len,5000);
		if(rx_size)
		{
			Trace_log(wireless_buf);
			if(strstr((char *)wireless_buf, "+JOIN:Fail"))
			{
				Trace_log("�豸����ʧ��\r\n");//����ģ���쳣
				return FALSE;
			}
			else if(strstr((char *)wireless_buf, "+JOIN:OK"))
			{
				Trace_log("�豸�����ɹ�\r\n");//����ģ���쳣
				uart_recv_data(&GPRS, wireless_buf, recive_len,500);
				Trace_log(wireless_buf);
				return TRUE;
			}
			else if(strstr((char *)wireless_buf, "Startup"))
			{
				sprintf((char *)transmit_cmd,"AT+JOIN=1\r\n");
				Trace_log(transmit_cmd);
				uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK", recive_len ,300);	
//				sprintf((char *)transmit_cmd,"ATIM=F12C34*56@\r\n");      //ģ������   ��ѯ����ʹ��
//				Trace_log(transmit_cmd);
//				uart_send_ack(&GPRS, transmit_cmd, wireless_buf2 ,"OK", recive_len ,300);	
//				sprintf((char *)transmit_cmd,"AT+ENC?\r\n");
//				Trace_log(transmit_cmd);
//				uart_send_ack(&GPRS, transmit_cmd, wireless_buf2 ,"OK", recive_len ,300);					
			}
		}
		else
			Trace_log("û���յ�����\r\n");//����ģ���쳣
		if(device_info.lorawan_set_data_en==1)
		{
			return FALSE;
		}
	}
	return FALSE;
}
/*********************LORAWAN��ȡ���ò���***********************************************************/
void lorawan_readstate(void)
{
	lorawan_read_DEVEUI();
	lorawan_read_APPEUI();
	lorawan_read_DEVADDR();
//	lorawan_read_APPKEY();
//	lorawan_read_NWKSKEY();
//	lorawan_read_APPSKEY();
	lorawan_read_NETID();
	lorawan_read_CHGROUP();
	lorawan_read_ULDLMODE();
	lorawan_read_JOINMODE();
	lorawan_read_CLASS();
	lorawan_read_STATUS();
	lorawan_read_BAND();
	lorawan_read_APPPORT();
	lorawan_read_DATARATE();
	lorawan_read_RSSI();
	lorawan_read_TXPWR();
	lorawan_read_ADR();
	
}
/*********************LORAWAN���ò���***********************************************************/
void lorawan_setstate(void)
{
	device_info.lorawan_set_data_en=0; 
	if(device_info.lorawan_net_status == 0)
	  PCIE_PW_OPEN(); 	//ģ���ϵ�
	if(lorawan_testAT()==TRUE)
	{
		lorawan_set_JOIN();
		lorawan_set_echo();
    lorawan_save();
		lorawan_reset();
    delay_vtask(300);		
		lorawan_set_DEVEUI();
		lorawan_set_APPEUI();
		lorawan_set_DEVADDR();
		lorawan_set_APPKEY();
		lorawan_set_NWKSKEY();
		lorawan_set_APPSKEY();
		lorawan_set_CHGROUP();
		lorawan_set_ULDLMODE();
		lorawan_set_JOINMODE();
		lorawan_set_CLASS();
		lorawan_set_BAND();
		lorawan_set_APPPORT();
		lorawan_set_DATARATE();
		lorawan_set_TXPWR();
		lorawan_set_ADR();
		lorawan_set_HOST();
		lorawan_set_SM();	

//sprintf((char *)transmit_cmd,"ATIM=F12C34*56@\r\n");       //ģ������   ��ѯ����ʹ��
//				Trace_log(transmit_cmd);
//				uart_send_ack(&GPRS, transmit_cmd, wireless_buf2 ,"OK", recive_len ,300);
//sprintf((char *)transmit_cmd,"AT+ENC=6DB6D972FE6BCE9C4217518EDC624E2B7400DA09\r\n");
//				Trace_log(transmit_cmd);
//				uart_send_ack(&GPRS, transmit_cmd, wireless_buf2 ,"OK", recive_len ,300);	
				
		lorawan_save();
		device_info.lorawan_net_status = 0;
	}
	else
		Trace_log("���ò���ʧ��\r\n");//����ģ���쳣
}
/*********************LORAWAN��ȡ����״̬����***********************************************************/
void lorawan_work_statues(void)
{
	lorawan_read_NETID();
	lorawan_read_STATUS();
	lorawan_read_RSSI();
}
/*********************LORAWAN��ʼ��***********************************************************/
uint8_t lorawan_init(void)
{
	if(lorawan_testAT()==TRUE)
	{
		if(lorawan_first == 0)
		{
			lorawan_readstate();
			lorawan_setstate();			
			lorawan_first = 1;
		}
		if(device_info.lorawan_set_data_en == 1)
		{	
			lorawan_setstate();
		}
		if(wait_lorawan_net_connnect() == TRUE)
		  return TRUE;
	}

	Trace_log("ģ���ʼ��ʧ��\r\n");//����ģ���쳣
	return FALSE;
}
/*********************LORAWAN����ͬ������***********************************************************/
uint8_t lorawan_send_setdata(void)
{
	uint8_t i;
	uint16_t crc_word;
	union flo_data   //����������
	{
		int8_t data[4];
		float value;
	};
	union flo_data f_data;
/*	
	memset(transmit_cmd,0,output_len);
	i = 0;
	transmit_cmd[i++] = 0x55;
	transmit_cmd[i++] = 0xaa;
	transmit_cmd[i++] = 0x02;      //���ݰ����ͱ�ǩ
	transmit_cmd[i++] = 0x01;
	transmit_cmd[i++] = 0x00;      //���ݰ�����
	f_data.value = device_info.save.sensor2_up;    //����ֵ ѹ������
	transmit_cmd[i++] = f_data.data[3]; 
	transmit_cmd[i++] = f_data.data[2]; 
	transmit_cmd[i++] = f_data.data[1]; 
	transmit_cmd[i++] = f_data.data[0];
	f_data.value = device_info.save.sensor2_down;    //����ֵ ѹ������
	transmit_cmd[i++] = f_data.data[3]; 
	transmit_cmd[i++] = f_data.data[2]; 
	transmit_cmd[i++] = f_data.data[1]; 
	transmit_cmd[i++] = f_data.data[0];
	f_data.value = device_info.save.sensor2_f;    //����ֵ ѹ������
	transmit_cmd[i++] = f_data.data[3]; 
	transmit_cmd[i++] = f_data.data[2]; 
	transmit_cmd[i++] = f_data.data[1]; 
	transmit_cmd[i++] = f_data.data[0];
	f_data.value = device_info.save.sensor2_hc;    //����ֵ ѹ���ز�
	transmit_cmd[i++] = f_data.data[3]; 
	transmit_cmd[i++] = f_data.data[2]; 
	transmit_cmd[i++] = f_data.data[1]; 
	transmit_cmd[i++] = f_data.data[0];
	
	f_data.value = device_info.save.sensor1_up;    //����ֵ �¶�����
	transmit_cmd[i++] = f_data.data[3]; 
	transmit_cmd[i++] = f_data.data[2]; 
	transmit_cmd[i++] = f_data.data[1]; 
	transmit_cmd[i++] = f_data.data[0];
	f_data.value = device_info.save.sensor1_down;    //����ֵ �¶�����
	transmit_cmd[i++] = f_data.data[3]; 
	transmit_cmd[i++] = f_data.data[2]; 
	transmit_cmd[i++] = f_data.data[1]; 
	transmit_cmd[i++] = f_data.data[0];
	f_data.value = device_info.save.sensor1_f;    //����ֵ �¶Ȳ���
	transmit_cmd[i++] = f_data.data[3]; 
	transmit_cmd[i++] = f_data.data[2]; 
	transmit_cmd[i++] = f_data.data[1]; 
	transmit_cmd[i++] = f_data.data[0];
	f_data.value = device_info.save.sensor1_hc;    //����ֵ �¶Ȼز�
	transmit_cmd[i++] = f_data.data[3]; 
	transmit_cmd[i++] = f_data.data[2]; 
	transmit_cmd[i++] = f_data.data[1]; 
	transmit_cmd[i++] = f_data.data[0];
	
	transmit_cmd[i++] = device_info.save.srv_time>>8;      //�豸�ϱ�ʱ����
	transmit_cmd[i++] = device_info.save.srv_time;
	transmit_cmd[i++] = device_info.save.SENSOR_SAM_TIME;    //�豸����ʱ����
	transmit_cmd[i++] = 0x00;                             //��ظ�λ��־λ
	
	transmit_cmd[4] = i+2;
	crc_word = crc_16((uint8_t *)transmit_cmd, i);
	transmit_cmd[i++] = crc_word>>8;
	transmit_cmd[i++] = crc_word;
	
//	uart_transmit_buf(&COM0,sevebuf,i);
//	memset(transmit_cmd,0,output_len1);
	memset(wireless_buf,0,sizeof(wireless_buf));
	str_to_hex16((uint8_t *)wireless_buf,(uint8_t *)transmit_cmd,i);
	sprintf((char *)transmit_cmd,"AT+DTRX=%s\r\n",wireless_buf);
	Trace_log(transmit_cmd);
	if(!uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK+SENT:", recive_len ,10000))
	{
		Trace_log("���ݷ��ͳɹ�\r\n");//����ģ���쳣
		return TRUE;
	}
	else
		Trace_log("���ݷ���ʧ��\r\n");//����ģ���쳣*/
		return FALSE;
	
}
/*********************LORAWAN����ʵʱ����***********************************************************/
uint8_t lorawan_send_newdata(void)
{
	uint8_t i;
	uint32_t adds;
	uint16_t crc_word;
	union time_data   //��������
	{
		uint8_t data[4];
		uint32_t value;
	};
	union time_data t_data;
	
	union sen_data   //����������
	{
		int8_t data[4];
		float value;
	};
	union sen_data s_data;
/*	
	device_info.alarming_flag = 0;   //������б�����־
	if(device_info.w24q32flash_err==1)   //flash����
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
		adds = device_get_flahaddr_addr();  //��ȡ���һ����ʷ���ݣ������ϱ�������洢����һ����
		hist_read(&wl_hist_data, adds);
	}
	if(wl_hist_data.data_time.year == 255||	wl_hist_data.data_time.month == 255)
	{
		Trace_log("û�д洢���� ����ʵʱ����\r\n");
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
		Trace_log("��ʷ��洢���� ����ʵʱ����\r\n");
		wl_hist_data.data_time.year = device_info.data_time.year;
		wl_hist_data.data_time.month = device_info.data_time.month;
		wl_hist_data.data_time.day = device_info.data_time.day;
		wl_hist_data.data_time.hour = device_info.data_time.hour;
		wl_hist_data.data_time.min = device_info.data_time.min;
		wl_hist_data.data_time.sec = device_info.data_time.sec;
		wl_hist_data.data.ch1_value = MPAbuf.sensor2data;
		wl_hist_data.data.ch2_value = MPAbuf.sensor1data;
	}
		
	memset(transmit_cmd,0,output_len);
	i = 0;
	t_data.value = time_2_time_stamp(&wl_hist_data.data_time);   //����ʱ��
	t_data.value -= 28800;          //ԭ����Ϊ����ʱ�䣬ת����UTCʱ����Ҫ��ȥ8Сʱ
	
	transmit_cmd[i++] = 0x55;         //���ݰ�ͷ
	transmit_cmd[i++] = 0xaa;
  transmit_cmd[i++] = 0x01;         //���ݰ�����
  transmit_cmd[i++] = 0x00;	       //���ݰ��ܳ���
	transmit_cmd[i++] = t_data.data[3];      //ʵʱ����ʱ��
	transmit_cmd[i++] = t_data.data[2]; 
	transmit_cmd[i++] = t_data.data[1]; 
	transmit_cmd[i++] = t_data.data[0]; 
	s_data.value = wl_hist_data.data.ch1_value;    //����ֵ ѹ��
	transmit_cmd[i++] = s_data.data[3]; 
	transmit_cmd[i++] = s_data.data[2]; 
	transmit_cmd[i++] = s_data.data[1]; 
	transmit_cmd[i++] = s_data.data[0];
	s_data.value = wl_hist_data.data.ch2_value;    //����ֵ  �¶�
	transmit_cmd[i++] = s_data.data[3]; 
	transmit_cmd[i++] = s_data.data[2]; 
	transmit_cmd[i++] = s_data.data[1]; 
	transmit_cmd[i++] = s_data.data[0];
	transmit_cmd[i++] = (uint8_t)(device_info.vbat_bfb/1);    //��ص����ٷֱ�
	transmit_cmd[i++] = nb.nb_csq;    //�ź�ǿ��
	crc_word = 0x0000;
	if(device_info.alarm_flag_STATUS.sen2up == 1)    //sen2���������Ҵ���  ѹ��
	{
		device_info.alarm_flag_ID.sen2up = 0;
		crc_word |= 0x0001;
	}
	else
		crc_word &= 0xFFFE;
	if(device_info.alarm_flag_STATUS.sen2down == 1)    //sen2���������Ҵ���  ѹ��
	{
		device_info.alarm_flag_ID.sen2down = 0;
		crc_word |= 0x0002;
	}
	else
		crc_word &= 0xFFFD;
	if(device_info.alarm_flag_STATUS.sen2_f == 1)    //sen2��������  ѹ��
	{
		device_info.alarm_flag_ID.sen2_f = 0;
		crc_word |= 0x0004;
	}
	else
		crc_word &= 0xFFFB;
	if(device_info.alarm_flag_STATUS.sen1up == 1)    //sen1���������Ҵ���   �¶�
	{
		device_info.alarm_flag_ID.sen1up = 0;
		crc_word |= 0x0008;
	}
	else
		crc_word &= 0xFFF7;
	if(device_info.alarm_flag_STATUS.sen1down == 1)    //sen1���������Ҵ���  �¶�
	{
		device_info.alarm_flag_ID.sen1down = 0;
		crc_word |= 0x0010;
	}
	else
		crc_word &= 0xFFEF;
	if(device_info.alarm_flag_STATUS.sen1_f == 1)    //sen1��������  �¶�
	{
		device_info.alarm_flag_ID.sen1_f = 0;
		crc_word |= 0x0020;
	}
	else
		crc_word &= 0xFFDF;
	if(device_info.alarm_flag_STATUS.senerror == 1)    //��������ȡ����
	{
		device_info.alarm_flag_ID.senerror = 0;
		crc_word |= 0x0040;
	}
	else
		crc_word &= 0xFFBF;
	if(device_info.alarm_flag_STATUS.flasherror == 1)    //flash����
	{
		device_info.alarm_flag_ID.flasherror = 0;
		crc_word |= 0x0080;
	}
	else
		crc_word &= 0xFF7F;
	if(device_info.alarm_flag_STATUS.powerlow == 1)    //�͵�������
	{
		device_info.alarm_flag_ID.powerlow = 0;
		crc_word |= 0x0100;
	}
	else
		crc_word &= 0xFEFF;
	if(device_info.alarm_flag_STATUS.MPU_MOT == 1)    //�˶�����
	{
		device_info.alarm_flag_ID.MPU_MOT = 0;
		crc_word |= 0x0200;
	}
	else
		crc_word &= 0xFDFF;
	if(device_info.alarm_flag_STATUS.tab_shock == 1)    //�𶯱�������
	{
		device_info.alarm_flag_ID.tab_shock = 0;
		crc_word |= 0x0400;
	}
	else
		crc_word &= 0xFBFF;
	
	transmit_cmd[i++] = crc_word>>8;
	transmit_cmd[i++] = crc_word;
	
	transmit_cmd[3] = i+2;
	crc_word = crc_16((uint8_t *)transmit_cmd, i);
	transmit_cmd[i++] = crc_word>>8;
	transmit_cmd[i++] = crc_word;
	
//	memset(transmit_cmd,0,output_len1);
	memset(wireless_buf,0,sizeof(wireless_buf));
	str_to_hex16((uint8_t *)wireless_buf,(uint8_t *)transmit_cmd,i);
	sprintf((char *)transmit_cmd,"AT+DTRX=%s\r\n",wireless_buf);
	Trace_log(transmit_cmd);
	if(!uart_send_ack(&GPRS, transmit_cmd, wireless_buf ,"OK+SENT:", recive_len ,10000))
	{
		Trace_log("���ݷ��ͳɹ�\r\n");//����ģ���쳣
		return TRUE;
	}
	else
		Trace_log("���ݷ���ʧ��\r\n");//����ģ���쳣*/
		return FALSE;
}
/*********************LORAWAN��������***********************************************************/
void lorawan_recive_data(void)
{
	char *p;
	uint8_t i,len;
	uint16_t crc_word;
	uint8_t h,l;
	union flo_data   //����������
	{
		int8_t data[4];
		float value;
	};
//	union flo_data f_data;
/*	
	if(!uart_send_ack(&GPRS, "AT+DRX?", wireless_buf ,"+DRX:", recive_len ,500))
	{
		p = strstr(( char *)wireless_buf, "feedback=" );
		memset(transmit_cmd,0,sizeof(transmit_cmd));
		while(*p != '\0' && *p++ != ':');
		while(*p != '\0' && *p++ != ',');
		len = atoi(p);
		for(i=0;i<len*2;i++)
		{
			transmit_cmd[i] = p[i];
		}
		transmit_cmd[i++] = '\0';
		memset(wireless_buf,0,sizeof(wireless_buf));
		device_string_to_hex(transmit_cmd,wireless_buf,i,len);
		
		if(wireless_buf[0] == 0x55 && wireless_buf[1] == 0xAA && wireless_buf[2] == 0x03 && wireless_buf[4] == 0x2A)    //��ʼ����У��ͨ��
		{
			crc_word = crc_16((uint8_t *)wireless_buf, len-2);
			l = crc_word>>8;
			h = crc_word;
			if(wireless_buf[len-2] == l && wireless_buf[len-1] == h)    //crcУ��ͨ��
			{
				f_data.data[3] = wireless_buf[5];
				f_data.data[2] = wireless_buf[6];
				f_data.data[1] = wireless_buf[7];
				f_data.data[0] = wireless_buf[8];
				device_info.save.sensor2_up = f_data.value;    //����ֵ ѹ������
				f_data.data[3] = wireless_buf[9];
				f_data.data[2] = wireless_buf[10];
				f_data.data[1] = wireless_buf[11];
				f_data.data[0] = wireless_buf[12];
				device_info.save.sensor2_down = f_data.value;    //����ֵ ѹ������
				f_data.data[3] = wireless_buf[13];
				f_data.data[2] = wireless_buf[14];
				f_data.data[1] = wireless_buf[15];
				f_data.data[0] = wireless_buf[16];
				device_info.save.sensor2_f = f_data.value;   //����ֵ ѹ������
				f_data.data[3] = wireless_buf[17];
				f_data.data[2] = wireless_buf[18];
				f_data.data[1] = wireless_buf[19];
				f_data.data[0] = wireless_buf[20];
				device_info.save.sensor2_hc = f_data.value;    //����ֵ ѹ���ز�
				
				f_data.data[3] = wireless_buf[21];
				f_data.data[2] = wireless_buf[22];
				f_data.data[1] = wireless_buf[23];
				f_data.data[0] = wireless_buf[24];
				device_info.save.sensor1_up = f_data.value;    //����ֵ �¶�����
				f_data.data[3] = wireless_buf[25];
				f_data.data[2] = wireless_buf[26];
				f_data.data[1] = wireless_buf[27];
				f_data.data[0] = wireless_buf[28];
				device_info.save.sensor1_down = f_data.value;    //����ֵ �¶�����
				f_data.data[3] = wireless_buf[29];
				f_data.data[2] = wireless_buf[30];
				f_data.data[1] = wireless_buf[31];
				f_data.data[0] = wireless_buf[32];
				device_info.save.sensor1_f = f_data.value;      //����ֵ �¶Ȳ���
				f_data.data[3] = wireless_buf[33];
				f_data.data[2] = wireless_buf[34];
				f_data.data[1] = wireless_buf[35];
				f_data.data[0] = wireless_buf[36];
				device_info.save.sensor1_hc = f_data.value;     //����ֵ �¶Ȼز�
				
				h = wireless_buf[37];
				l = wireless_buf[38];
				device_info.save.srv_time = (h<<8)|l;
				device_info.save.SENSOR_SAM_TIME = wireless_buf[39];
				if(wireless_buf[40] == 0x01)     //��ظ�λ
					device_vbat_reset();
				device_param_write(&device_info.save);     //flash�洢����
				Trace_log("Զ�̸��Ĳ���\r\n");//����ģ���쳣
			}
		}
	}*/
}
/*********************LORAWAN��������***********************************************************/
void task_lorawan(void)
{	
	Trace_log("lorawan task\r\n");//����ģ���쳣
	uint8_t re_send = 0;
	begin:
	if(re_send >=3)    //�����ط�����
		goto end;
	if(device_info.lorawan_net_status == 0)
	{
		PCIE_PW_OPEN(); 	//ģ���ϵ�
		if(lorawan_init() == TRUE)
		{
			if(lorawan_send_setdata() == TRUE)
				device_info.lorawan_net_status = 1;
			else 
			{
				PCIE_PW_CLOSE();
				re_send++;
				goto begin;
			}
		}
		else
		{
			PCIE_PW_CLOSE();
			re_send++;
			goto begin;
		}
	}
	if(device_info.lorawan_net_status == 1)
	{
		if(lorawan_send_newdata() == FALSE)    //���ݷ���ʧ��
		{
			PCIE_PW_CLOSE();
	    re_send++;
			device_info.lorawan_net_status = 0;
			goto begin;
		}
		else
		{
			Trace_log("lorawan���ݷ��ͳɹ�\r\n");//lorawan�ɹ�
			lorawan_recive_data();
			lorawan_work_statues();
			return;
		}
	}
	
	end:
	PCIE_PW_CLOSE();
	device_info.lorawan_net_status = 0;
	lorawan_first = 0;
	Trace_log("lorawan���ݷ���ʧ��\r\n");//lorawanʧ��
}

