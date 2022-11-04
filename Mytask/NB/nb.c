#include "main.h"
#include "nb.h"

#define nb_vddext   LL_GPIO_IsInputPinSet(vddext_gpio, vddext_gpio_id)


nb_ack nb={0};
hist_t  wl_hist_data;
rtc_t *gnss_data_time;
rtc_t cloud_time;
uint8_t transmit_cmd[output_len]={0};  //��������1
uint8_t wireless_buf[recive_len]={0};   //��������
//uint8_t sevebuf[output_len]={0};    //��������2
uint16_t BUFSIZ10=0;         //
//uint8_t sevebuf2[output_len1]={0};   //��������3
//uint8_t dingzhi1 = 0;   //�������� +IMEI   1��   0�ر�
//uint32_t sen_num;     

union sen_num   //�豸��ˮ��
{
  uint8_t data[4];
  uint32_t value ;
};
union sen_num s_num;


uint8_t gnssdata[10];   //�洢��γ���Լ�������Ϣ
//float GPS_E,GPS_N;

/******************************************************************/
/******************************************************************************/
void clear_allalarm(void)     //������б�����־λ
{
	uint8_t i;
	device_info.alarming_flag = 0;
	for(i=0;i<16;i++)
	{
		device_info.alarm_flag_ID.ch_status[i] = alarm_ok;
	}
	device_info.alarm_flag_ID.flasherror = 0;

}
/******************************************************************************/
void nb_OFF(void)   //ģ������ػ�
{
//	char i=10;
	SET_CFUN(0);  //����Ϊ���ٹ���ģʽ
	uart_send_ack(&GPRS, "AT+QCSEARFCN\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300);  //���ģ���Ѵ洢��EARFCN�б�
  if(!SET_QPOWD())
	{
//		while(i--)
//		{
//			if(!nb_vddext)   //��ⲻ��    ģ��δ��������
//			{
//				Trace_log("ģ��ػ��ɹ�\r\n");
//				break ;
//			}
//		  delay_vtask(200);
//		}
	}
	else
		Trace_log("ģ��ػ�ʧ��\r\n");
}
/******************************************************************************/
uint8_t nb_psmstart(void)          //PSM���� 
{
	if(nb_vddext)
	{
		Trace_log("ģ���ڿ���״̬\r\n");
		return 0;
	}
	else 
	{		
	  Trace_log("ģ�黽��\r\n");	
		LL_GPIO_SetOutputPin(psmio_gpio, psmio_gpio_id);    //����ģ��   BC95�����Ų�֧�ֻ��ѣ�ͨ��������ͨѶ����
		delay_vtask(500);
		LL_GPIO_ResetOutputPin(psmio_gpio, psmio_gpio_id);
		delay_vtask(100);
		if(TEST_AT(20))   //δ��⵽AT����
        return 1;
		SET_CFUN(1);  //����ȫ����
		if(!TEST_CPIN())
			return 0;
	 }
	return 1;
}
/******************************************************************************/
static void NB_RST(void)     //ģ�鰴������
{
	//����500ms���� 
	Trace_log("NBģ�鰴������\r\n");
	
	LL_GPIO_ResetOutputPin(pwrio_gpio, pwrio_gpio_id);    
	delay_vtask(500); 
	LL_GPIO_SetOutputPin(pwrio_gpio, pwrio_gpio_id);
	delay_vtask(500);
	LL_GPIO_ResetOutputPin(pwrio_gpio, pwrio_gpio_id);
	delay_vtask(1000);
}
/******************************************************************************/
uint8_t nb_open(void)      
{
//	uint8_t rx_size[100];

//	PCIE_PW_OPEN(); 	//ģ���ϵ�
	
    NB_RST();   ///  ������
	
    if(TEST_AT(30) != 0)   //���AT
    {
        return 1;
    }
		SET_ATE(1);
		TEST_AT(1);
		TEST_ATI();
		delay_vtask(1000);
		
    SET_CFUN(0);  //�ر�ȫ����
		
		//APN
		if(device_info.save.tab_module == BC95)
		{
			if(strcmp((char *)device_info.save.srv_apn, "") != 0)  //��⵽�Ѿ�����APN
			{
				sprintf((char *)transmit_cmd, "AT+CGDCONT=1,\"IP\",\"%s\"\r\n", device_info.save.srv_apn);
				uart_send_ack(&GPRS, transmit_cmd , wireless_buf, "OK", sizeof(wireless_buf), 300);
			}
			
			if(strcmp((char *)device_info.save.srv_apn_user_name, "") != 0) //��⵽�Ѿ�����APN user name
			{
				sprintf((char *)transmit_cmd, "AT+CGAUTH=1,2,\"%s\"", device_info.save.srv_apn);
				uart_transmit_str(&GPRS,transmit_cmd);
				if(strcmp((char *)device_info.save.srv_apn_user_pass, "") != 0) //��⵽�Ѿ�����APN user name
				{
					sprintf((char *)transmit_cmd, ",\"%s\"", device_info.save.srv_apn_user_pass);   //set login number
					uart_transmit_str(&GPRS,transmit_cmd);
				}
				sprintf((char *)transmit_cmd, "\r\n");   //set login number
				uart_send_ack(&GPRS, transmit_cmd , wireless_buf, "OK", sizeof(wireless_buf), 300);
			}
			else     //ɾ���û���������
			{
				sprintf((char *)transmit_cmd, "AT+CGAUTH=1,0\r\n");
				uart_send_ack(&GPRS, transmit_cmd , wireless_buf, "OK", sizeof(wireless_buf), 300);
			}
		}
		else
		{
			if(strcmp((char *)device_info.save.srv_apn, "") != 0)  //��⵽�Ѿ�����APN
			{
				sprintf((char *)transmit_cmd, "AT+QCGDEFCONT=\"IP\",\"%s\"", device_info.save.srv_apn);   //set login number
				uart_transmit_str(&GPRS,transmit_cmd);
				if(strcmp((char *)device_info.save.srv_apn_user_name, "") != 0) //��⵽�Ѿ�����APN user name
				{
					sprintf((char *)transmit_cmd, ",\"%s\"", device_info.save.srv_apn_user_name);   //set login number
					uart_transmit_str(&GPRS,transmit_cmd);
				}
				if(strcmp((char *)device_info.save.srv_apn_user_pass, "") != 0) //��⵽�Ѿ�����APN user name
				{
					sprintf((char *)transmit_cmd, ",\"%s\"", device_info.save.srv_apn_user_pass);   //set login number
					uart_transmit_str(&GPRS,transmit_cmd);
				}
				sprintf((char *)transmit_cmd, "\r\n");   //set login number
				uart_send_ack(&GPRS, transmit_cmd , wireless_buf, "OK", sizeof(wireless_buf), 300);
				if(device_info.save.tab_module == BC26)
					SET_QRST();  //�豸����		AT+QRST	
				else if(device_info.save.tab_module == BC95)
					SET_NRB();
				
				delay_vtask(200);
				if(TEST_AT(20) != 0)   //���AT
				{
						return 1;
				}
			}
		}
						
		if(device_info.save.tab_module != BC95)
		{
			SET_IPR("115200");  //���ò�����
			TEST_AT(1);
			SET_ATW();   //��������
			SET_CFUN(1);  //����ȫ����
		}
			
		
		switch(device_info.save.device_operator)    //BC26,BC20����band������CFUNΪ1״̬��BC95���ñ�����CFUNΪ0״̬
		{
			case 1: SET_QBAND(5);break;
			case 2: SET_QBAND(8);break;
			case 3: SET_QBAND(3);break;
			default: SET_QBAND(0);break;
		}

		
//		if(TEST_CPIN())    //���SIM�� �쳣
//			return 1;
//		else
//		{
//		uart_send_ack(&GPRS, "AT+CEDRXS?\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300); 
		Trace_log("ģ�鿪���ɹ�\r\n");
		return 0;
//		}
}
/***************************************************************************************/
void time_calibration(void)      //��վУʱ
{
	uint8_t i;
	uint8_t atod = 0;
	char *ackp;
	if(device_info.time_calib == 1)    //�豸�Զ�Уʱ
	{
		device_info.time_calib = 0;
		i = 3;
		Trace_log("��ȡʱ��\r\n");
		while(i--)
		{
				uart_send_ack(&GPRS, "AT+QCCLK?\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500);
				if(strstr((char *)wireless_buf, "+QCCLK:") != NULL)
				{
					Trace_log("���ջ�վУ׼��Ϣ");
          ackp =strstr(( char *)wireless_buf, "+QCCLK:");
					memset(transmit_cmd, 0, sizeof(transmit_cmd));
				  strncpy((char *)transmit_cmd,ackp,200);
					ackp =strstr(( char *)transmit_cmd, "+QCCLK:" );
					
					while(*ackp++ != ':');
					atod = atoi((char *)ackp);
					if(atod != 0)
					 device_info.data_time.year = atod;
					while(*ackp++ != '/');
					atod = atoi((char *)ackp);
					if(atod != 0)
					  device_info.data_time.month = atod;
					while(*ackp++ != '/');
					atod = atoi((char *)ackp);
					if(atod != 0)
					  device_info.data_time.day = atod;
					while(*ackp++ != ',');
					device_info.data_time.hour = atoi((char *)ackp);
					while(*ackp++ != ':');
					device_info.data_time.min = atoi((char *)ackp);
					while(*ackp++ != ':');
					device_info.data_time.sec = atoi((char *)ackp);
								
					uint64_t timestamp = 0;
					timestamp = time_2_time_stamp(&device_info.data_time);
					stamp_2_time(timestamp + 28800, &device_info.data_time);
					RTC_WriteDate(&device_info.data_time);
					break;
				}
		}
	}
}
/******************************************************************/
/******************************************************************/
void get_nb_message(void)   //��ȡģ��̼���Ϣ
{
  GET_IMEI((char *)nb.nb_imei,"BC26");
	GET_ICCID((char *)nb.nb_iccid,"BC26");//iccid
	delay_vtask(50);
  GET_IMSI((char *)nb.nb_imsi);	
}
/******************************************************************/
uint8_t nb_init(void)    //ģ���ʼ��
{
	     //BC26ģ�鲻֧��AT+CREG����
//	uart_send_ack(&GPRS, "AT+QNBIOTEVENT=1,1\r\n", wireless_buf2, "OK", recive_len, 300);
//	uart_send_ack(&GPRS, "AT+CEREG=5\r\n", wireless_buf2, "OK", recive_len, 300);		
	SET_CFUN(1);  //����ȫ����
	uart_send_ack(&GPRS, "AT+CGATT=1\r\n", wireless_buf ,"OK", recive_len ,300);     //�������總��
	if(TEST_CPIN())    //���SIM�� �쳣
			return 1;
	if(device_info.save.tab_module == BC95)
		uart_send_ack(&GPRS, "AT+NNMI=2\r\n", wireless_buf ,"OK", recive_len ,300);
	if(TEST_CEREG(130))
	{
		Trace_log("ģ��ע������ʧ��\r\n");
		return 1;
	}
	
	if(TEST_CGATT(80))
	{
		Trace_log("���總��ʧ��\r\n");
		return 1;
	}	
   ///AT+CREG?   90s��ע�᲻�����缴�ɽ���
  	
	nb.nb_csq = GET_CSQ();  //CSQ     ��ȡ�ź�ǿ��
	if(nb.nb_csq == 0)
	{
		return 1;
	}	
	else  
		nb.nb_cesq = GET_CESQ();     //��ȡ�ź�����
//	nb.nb_cbc = GET_CBC();     //cbc     ��ȡģ�鹩���ѹ
  if(device_info.save.tab_protocol == NBIOT)
	{
	    uart_send_ack(&GPRS, "AT+CPSMS=1,,,\"00111000\",\"00000000\"\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500);     //TAU time 24H   active time 60s
		  TEST_CEREG(2);
	}
	Trace_log("ģ���ʼ���ɹ�\r\n");
	time_calibration();
	return 0;	
}
/******************************************************************/
uint8_t BC95_waitLWM2M_connect_status(void)
{
	uint8_t i,write_ack_num;
	write_ack_num = 8;
	while(1)
	{
//				uart_send_ack(&GPRS, "AT+QDTLSSTAT?\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300);AT+CEDRXS?
//		uart_send_ack(&GPRS, "AT+CEDRXS?\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300);
		uart_transmit_str(&GPRS,(uint8_t *)"AT+NMSTATUS?\r\n");
		i = check_recv3(&GPRS, wireless_buf, "REJECTED_BY_SERVER", "MO_DATA_ENABLED","UNINITIALISED",sizeof(wireless_buf), 300);
		if(i == 0)   //���Updata�Ƿ�ɹ�
		{
			Trace_log("�������ܾ�\r\n");
			return FALSE;
		}
		else if(i == 2)
		{
			Trace_log("δ��ʼ��\r\n");
			return FALSE;
		}
		else if(i == 1)
		{
			Trace_log("ʹ�����ݷ���\r\n");
			return TRUE;
		}
		write_ack_num--;
		if(write_ack_num==0)
		{
			Trace_log("ע��ʧ��\r\n");
			return FALSE;
		}
		delay_vtask(500);
	}	
}
/***************************************************************************************/
uint8_t BC95_wait_send_status(void)
{
	char *getp;
	uint8_t i,m;
	m = 30;
	while(m--)
	{
		if(!uart_send_ack(&GPRS, "AT+QLWULDATASTATUS?\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500))
		{
			getp = (char *)strstr(( char *)wireless_buf, "STATUS:");
			getp += 7;
			i = atoi((char *)getp);
			if(i == 0)
			{
				Trace_log("δ����\r\n");
//				return FALSE;
			}
			else if(i == 1)
			{
				Trace_log("�ѷ��ͣ��ȴ� LwM2M ������ƽ̨��Ӧ\r\n");
//				return TRUE;
			}
			else if(i == 2)
			{
				Trace_log("����ʧ��\r\n");
				return FALSE;
			}
			else if(i == 3)
			{
				Trace_log("��ʱ\r\n");
				return FALSE;
			}
			else if(i == 4)
			{
				Trace_log("�ɹ�\r\n");
				return TRUE;
			}
			else if(i == 5)
			{
				Trace_log("�յ�RESET��Ϣ\r\n");
				return FALSE;
			}
		}
		delay_vtask(500);
	}
	Trace_log("����ʧ��\r\n");
	return FALSE;
}
/***************************************************************************************/
/***************************************************************************************/
uint8_t getParam(void)
{
	uint8_t f_write=0;
	uint16_t rx_size,da;;
//	uint8_t i;
	uint8_t wait_cnt = 10; 
	uint8_t get_len;
	char *getp;
//	char SAM[5];
//	float f_da;
	getp=0;
	
	memset(wireless_buf, 0, sizeof(wireless_buf));
	memset(transmit_cmd, 0, sizeof(transmit_cmd));
	Trace_log("��ȡƽ̨������Ϣ\r\n");	
	if( device_info.save.sever_type == NB_tlink)
	{
		if(device_info.save.tab_module == BC95)
		{
			uart_send_ack(&GPRS, "AT+QLWULDATAEX=8,676574506172616d,0x0100\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300);  //��������get����
			if(BC95_wait_send_status() == FALSE)
				return 0;
		}
		else
			uart_send_ack(&GPRS, "AT+QLWDATASEND=19,0,0,8,676574506172616d,0x0100\r\n", wireless_buf, "SEND OK", sizeof(wireless_buf), 15000);  //��������get����
	}
	while(wait_cnt--)
	{
		if( device_info.save.sever_type == NB_tlink   || device_info.save.sever_type == NB_telecom   )
		{
//			uart_transmit_str(&GPRS, (uint8_t *)"AT+QLWRD=256\r\n");	
			if(device_info.save.tab_module == BC95)
				rx_size = uart_send_ack(&GPRS, "AT+NMGR\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500);
			else
				rx_size = uart_send_ack(&GPRS, "AT+QLWRD=256\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500);			
		}
		else if( device_info.save.sever_type == NB_onenet )
		{
//			uart_transmit_str(&GPRS, (uint8_t *)"AT+MIPLRD=256\r\n");
			rx_size = uart_send_ack(&GPRS, "AT+MIPLRD=256\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500);
		}
		getp=0;
//		rx_size = uart_recv_data(&GPRS, wireless_buf2, recive_len,300);
		if(!rx_size)
		{
//			Trace_log(wireless_buf2);
			if( device_info.save.sever_type == NB_tlink   || device_info.save.sever_type == NB_telecom   )
			{
				if(device_info.save.tab_module == BC95)
				{
					getp = (char *)strstr(( char *)wireless_buf, "NMGR");
				}
				getp = (char *)strstr(( char *)wireless_buf, "+QLWRD: ");
			}
			else if( device_info.save.sever_type == NB_onenet )
				getp = (char *)strstr(( char *)wireless_buf, "+MIPLRD:");
			if(getp!=NULL)
			{
				get_len = 0;
				if(device_info.save.tab_module == BC95)
				{
					getp += 4;
					get_len = atoi((char *)getp);
				}
				else
				{
					while(*getp++ != ' ');
					get_len = atoi((char *)getp);
				}
				
				if(get_len)
				{
					getp = (char *)strstr(( char *)wireless_buf, "7B22");
					if(getp!=NULL)
					{
						hex16_to_str((uint8_t *)getp,get_len*2,transmit_cmd);	
						Trace_log(transmit_cmd);
						Trace_log("\r\n");
						break;
					}
					else
						get_len = 0;
				}			
			}
		}
		delay_vtask(200);
	}
//	delay_vtask(100);
	if(get_len)
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
				Trace_log("�޸�AT\r\n");
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
				Trace_log("�޸�CT\r\n");
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
				Trace_log("��ص�����λ\r\n");
			}				
		}
		
   if(f_write == 1)
   {
	   f_write=0;  
		 device_info.Params_sever = 1;
     device_param_write(&device_info.save);    //���ݱ���
   }
	 
		for(uint8_t qinkong=0;qinkong<2;qinkong++)
	  {
			if( device_info.save.sever_type == NB_tlink   || device_info.save.sever_type == NB_telecom  )
			{
//				uart_transmit_str(&GPRS, (uint8_t *)"AT+QLWRD=256\r\n");
				if(device_info.save.tab_module == BC95)
					rx_size = uart_send_ack(&GPRS, "AT+NMGR\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500);
				else
				uart_send_ack(&GPRS, "AT+QLWRD=256\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300);
			}
			else if( device_info.save.sever_type == NB_onenet )
			{
//				uart_transmit_str(&GPRS, (uint8_t *)"AT+MIPLRD=256\r\n");
				uart_send_ack(&GPRS, "AT+MIPLRD=256\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300);
			}
			vTaskDelay(100);
	  }
		return 1;    //���յ�Ӧ��
  }
  else
		return 0;
}
/***************************************************************************************/
uint8_t nb_link_sever(void )   //NBIOTƽ̨����
{	
//uint8_t rx_size[40];
//uint8_t rx_size1[40];
	char *buf_cache;
  uint32_t mgid2;
//	uint8_t len;	
//	uint8_t upack=0;
	if( device_info.save.sever_type == NB_tlink  ||device_info.save.sever_type == NB_telecom)    
	{		
		Trace_log("NB_tlink or NB_telecom����\r\n");
		if(device_info.save.tab_module == BC95)
		{
			uart_send_ack(&GPRS, "AT+QREGSWT?\r\n", wireless_buf, "OK", recive_len, 300);
//			if( !uart_send_ack(&GPRS, "AT+QREGSWT?\r\n", wireless_buf, "OK", recive_len, 300))   //�޸�ע��ģʽ   Ĭ��1 �Զ�ע��
//			{
//				buf_cache = strstr((const char *)wireless_buf, "+QREGSWT:" );
//				buf_cache += 9;
//				len = atoi(buf_cache);
//				if(len == 0)
//				{
//					
//				}
//			}
			sprintf((char *)transmit_cmd, "AT+NCDP=%s,%s\r\n", device_info.save.srv_addr,device_info.save.srv_port);    //set connect ip and port   AT+NCDP=180.101.147.115,5683
			uart_send_ack(&GPRS, transmit_cmd , wireless_buf, "OK", sizeof(wireless_buf), 300);
			sprintf((char *)transmit_cmd,"AT+QCFG=\"LWM2M/EndpointName\",\"%s\"\r\n", device_info.save.srv_login);   //set login number
		  uart_send_ack(&GPRS, transmit_cmd , wireless_buf, "OK", sizeof(wireless_buf), 300);
			uart_send_ack(&GPRS, "AT+NRB\r\n", wireless_buf, "OK", recive_len, 300);   //����
			delay_vtask(2000);
			if(nb_init() == 1)
				return 1;
			if(BC95_waitLWM2M_connect_status() == TRUE)
				return 0;
		}
		else
		{
			uart_send_ack(&GPRS, "AT+QLWCFG=\"dataformat\",1,1\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300);   //set sending and received data into hex mode
		  sprintf((char *)transmit_cmd, "AT+QLWSERV=\"%s\",%s\r\n", device_info.save.srv_addr,device_info.save.srv_port);    //set connect ip and port
		  if(! uart_send_ack(&GPRS, transmit_cmd , wireless_buf, "OK", sizeof(wireless_buf), 300))    //�����ɹ�
		  {	
			    sprintf((char *)transmit_cmd, "AT+QLWCONF=\"%s\"\r\n", device_info.save.srv_login);   //set login number
		      uart_send_ack(&GPRS, transmit_cmd , wireless_buf, "OK", sizeof(wireless_buf), 300); 
		      uart_send_ack(&GPRS, "AT+QLWADDOBJ=19,0,1,\"0\"\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300);  	 // add a LwM2M object
					uart_transmit_str(&GPRS, (uint8_t *)"AT+QLWOPEN=1\r\n");     //��������
					if(!check_recv2(&GPRS, wireless_buf,"+QLWOBSERVE: 0,19,0", "ERROR",sizeof(wireless_buf),30000))
					{			
						return 0;
					}
		  }	
		  uart_send_ack(&GPRS, "AT+QLWCLOSE\r\n", wireless_buf, "CLOSE OK", sizeof(wireless_buf), 300);
		  uart_send_ack(&GPRS, "AT+QLWDEL\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300);	  
		}
	}
	else if( device_info.save.sever_type == NB_onenet)
	{	 
		Trace_log("onenet����\r\n");
		uart_send_ack(&GPRS,"AT+MIPLCONFIG=1,0,1,\"183.230.40.39\",5683\r\n",wireless_buf,"OK",sizeof(wireless_buf),300);//  ��������IP port
	
		uart_transmit_str(&GPRS,(uint8_t *)"AT+MIPLCREATE\r\n");   //Created the communication suite instance successfully    
		if(!check_recv2(&GPRS, wireless_buf, "OK", "+CIS ERROR: ",sizeof(wireless_buf), 300))
		{	
			uart_transmit_str(&GPRS,(uint8_t *)"AT+MIPLADDOBJ=0,3200,1,\"1\",4,2\r\n");//4,2      Add a LwM2M object
			if(	!check_recv2(&GPRS, wireless_buf, "OK", "+CIS ERROR: ",sizeof(wireless_buf), 300))
			{
				if( !uart_send_ack(&GPRS, "AT+MIPLOPEN=0,86400\r\n", wireless_buf, "+MIPLOBSERVE: 0", sizeof(wireless_buf), 15000))   //
				{
						buf_cache = strstr((const char *)wireless_buf, "+MIPLOBSERVE: 0" );
						{
							buf_cache += 16;
							nb.mgid = atoi(buf_cache);
						}						
						sprintf((char *)transmit_cmd,"AT+MIPLOBSERVERSP=0,%d,1\r\n",nb.mgid);
						buf_cache=0;
						if( !	uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "+MIPLDISCOVER: 0", sizeof(wireless_buf), 10000))
						{
								buf_cache = strstr((const char *)wireless_buf, "+MIPLDISCOVER: 0" );
								{
										buf_cache += 17;
										mgid2 = atoi(buf_cache);
								}						
								sprintf((char *)transmit_cmd,"AT+MIPLDISCOVERRSP=0,%d,1,4,\"5750\"\r\n",mgid2);		 
								uart_send_ack(&GPRS,transmit_cmd, wireless_buf, "OK", sizeof(wireless_buf), 300);  	   
								return 0 ;
						}						
				 }				
			}						  
		}
		uart_send_ack(&GPRS, "AT+MIPLDELOBJ=0,3200\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300);
		uart_send_ack(&GPRS, "AT+MIPLCLOSE=0\r\n", wireless_buf, "+MIPLEVENT: 0,15", sizeof(wireless_buf), 300) ;
		uart_send_ack(&GPRS, "AT+MIPLDELETE=0\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300) ;				
	}
	return 1;
}
/***************************************************************************************/
uint8_t TCP_UDP_link_sever(void )   //TCP/UDPƽ̨����
{
//	uint8_t wait_t = 60;
	if( device_info.save.tab_protocol == UDP)
	{
	  sprintf((char *)transmit_cmd, "AT+QIOPEN=1,0,\"UDP\",\"%s\",%s,0\r\n", device_info.save.srv_addr,device_info.save.srv_port);    //set connect ip and port
	}
	else if( device_info.save.tab_protocol == TCP)
	{
	  sprintf((char *)transmit_cmd, "AT+QIOPEN=1,0,\"TCP\",\"%s\",%s,0\r\n", device_info.save.srv_addr,device_info.save.srv_port);    //set connect ip and port
	}
	uart_transmit_str(&GPRS,transmit_cmd);
	if(check_recv2(&GPRS, wireless_buf, "+QIOPEN: 0,0", "ERROR",sizeof(wireless_buf), 10000)) //����ʧ��
	{
		uart_transmit_str(&GPRS,transmit_cmd);
		if(check_recv2(&GPRS, wireless_buf, "+QIOPEN: 0,0", "ERROR",sizeof(wireless_buf), 10000)) //����ʧ��
			return 1;
	}
	uart_send_ack(&GPRS, "AT+QISTATE=1,0\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300);	
	return 0;
}
/***************************************************************************************/
uint8_t GET_GNSS(void)                             //��ȡ��λ��Ϣ
{	
	uint8_t gnss = 60;
	char *buf_cache,addr,i,jiexi,gnss_err;
	float yushu;
	char gnss_time[6];
	union sen_data   //
	{
		int8_t data[4];
		float value;
	};
	union sen_data s_data;
	union hc_data   //��������
	{
		uint8_t data[4];
		uint32_t value;
	};
	union hc_data h_data;
	
	if(device_info.save.GPS_stu == OFF)
		device_info.gps_time = 0;	
	else if(device_info.gps_time >= 24)   //���豸��������ʱ��δ��
	{
		device_info.gps_satues = 0;
		device_info.gps_time = 0;
		uart_send_ack(&GPRS, "AT+QGNSSC=1\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300);   //����GPS��λ
		uart_send_ack(&GPRS, "AT+QGNSSAGPS=1\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300);
		uart_send_ack(&GPRS, "AT+QGNSSDB=1\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300);
		delay_vtask(5000);
	  jiexi = 0;	
		gnss_err = 0;
		while(gnss--)
		{
			if(!uart_send_ack(&GPRS, "AT+QGNSSRD=\"NMEA/RMC\"\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300))
			{
				gnss_err = 0;
				buf_cache = strstr((const char *)wireless_buf, "+QGNSSRD:" );
				addr = 0;
				i = 0;				
				while(*(buf_cache+i) != '\0')
				{
					if(*(buf_cache+i) == ',')
					  addr++;
					if(addr == 2)
					{
						i++;
						if(*(buf_cache+i) == 'A')  //��⵽��Ч��λ��Ϣ
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
			delay_vtask(5000);  //ms��ʱ
		}
//		jiexi = 1;
//		memset((char *)wireless_buf2,0,recive_len);
//		sprintf((char *)wireless_buf2 ,"+QGNSSRD: $GNRMC,093530.21,A,2236.7006,N,11350.3778,E,1.100,,200720,,,A,V*15");
//		buf_cache = strstr((const char *)wireless_buf2, "+QGNSSRD:" );
		
		if(jiexi == 1)
		{
			buf_cache += 16;
			while(*(buf_cache++) != ',');     //���ҵ�1��������
			for(i=0;i<6;i++)
			{
				gnss_time[i] = *(buf_cache++);
			}
			gnss_data_time->hour = ((gnss_time[0]-0x30)*10 + (gnss_time[1]-0x30)+8);    //hour
			gnss_data_time->min = ((gnss_time[2]-0x30)*10 + (gnss_time[3]-0x30));     //min
			gnss_data_time->sec = ((gnss_time[4]-0x30)*10 + (gnss_time[5]-0x30));     //sec
			if(gnss_data_time->hour >=24)
				gnss_data_time->hour -=24;
			while(*(buf_cache++) != ',');     //���ҵ�2��������
			while(*(buf_cache++) != ',');     //���ҵ�3��������
			
			s_data.value = atof(buf_cache);
			device_info.GPS_N = s_data.value;
			yushu = fmodf(s_data.value,100);
			yushu = yushu/60;
			s_data.value = (int16_t)s_data.value/100;
			s_data.value += yushu;
			h_data.value = (uint32_t)(s_data.value*1000000);
			gnssdata[1] = h_data.data[3];      //γ������
			gnssdata[2] = h_data.data[2];
			gnssdata[3] = h_data.data[1];
			gnssdata[4] = h_data.data[0];
      		
      while(*(buf_cache++) != ',');     //���ҵ�4��������		
      gnssdata[0] = *(buf_cache++);      //γ�ȷ���			
			while(*(buf_cache++) != ',');     //���ҵ�5��������

			s_data.value = atof(buf_cache);
			device_info.GPS_E = s_data.value;
			yushu = fmodf(s_data.value,100);
			yushu = yushu/60;
			s_data.value = (int16_t)s_data.value/100;
			s_data.value += yushu;
			h_data.value = (uint32_t)(s_data.value*1000000);
			gnssdata[6] = h_data.data[3];      //��������
			gnssdata[7] = h_data.data[2];
			gnssdata[8] = h_data.data[1];
			gnssdata[9] = h_data.data[0];
			while(*(buf_cache++) != ',');     //���ҵ�6��������
			gnssdata[5] = *(buf_cache++);      //���ȷ���
			while(*(buf_cache++) != ',');     //���ҵ�7��������
			while(*(buf_cache++) != ',');     //���ҵ�8��������
			while(*(buf_cache++) != ',');     //���ҵ�9��������
			for(i=0;i<6;i++)
			{
				gnss_time[i] = *(buf_cache++);
			}
			gnss_data_time->day = ((gnss_time[0]-0x30)*10 + (gnss_time[1]-0x30));    //day
			gnss_data_time->month = ((gnss_time[2]-0x30)*10 + (gnss_time[3]-0x30));     //month
			gnss_data_time->year = ((gnss_time[4]-0x30)*10 + (gnss_time[5]-0x30));     //year
			uart_send_ack(&GPRS, "AT+QGNSSC=0\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300);    //�ر�GPS
			device_info.gps_satues = 1;
			return 1;		
		}
	}
	return 0;
}

/***************************************************************************************/
/***************************************************************************************/
static uint8_t tcp_udp_send_data(void)
{
	uint8_t retr = FALSE;
	BUFSIZ10=strlen((char *)transmit_cmd);
	str_to_hex16((uint8_t *)wireless_buf,(uint8_t *)transmit_cmd,BUFSIZ10);

	sprintf((char *)transmit_cmd, "AT+QISEND=0,%d,%s\r\n",BUFSIZ10,wireless_buf);
	uart_send_ack(&GPRS, "AT+QICFG=\"dataformat\",1,0\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300);  //����HEXģʽ��������
	uart_transmit_str(&GPRS,transmit_cmd);
	if(check_recv3(&GPRS, wireless_buf, "SEND OK", "SEND FALL","ERROR",sizeof(wireless_buf), 15000) == 0)   //�յ�Ӧ��
	{
		retr = TRUE;
	}
	else
	{
		uart_transmit_str(&GPRS,transmit_cmd);
		if(check_recv3(&GPRS, wireless_buf, "SEND OK", "SEND FALL","ERROR",sizeof(wireless_buf), 15000) == 0)   //�յ�Ӧ��
			retr = TRUE;
		else
			retr = FALSE;
	}
	return retr;
}
/***************************************************************************************/
uint8_t TCP_UDP_sevebuf(void )   //TCP/UDPƽ̨��������
{
	uint8_t i=0;
//	uint8_t retr = FALSE;
	uint8_t shuju_ok = 0;
	uint32_t adds;
//	uint16_t crc_word;
//	uint8_t udp_login1[16],udp_login2[32];
//	memset(sevebuf,0,output_len);
//	memset(udp_login1,0x30,16);
	
	device_info.alarming_flag = 0;   //������б�����־	
	
	if(device_info.Params_sever==1 )  //��һ�η���ͬ������
	{	
		Trace_log("��һ�η���ͬ������\r\n");
		sprintf((char *)transmit_cmd,"{%s\"IMEI\":\"%s\",\"ICCID\":\"%s\",\"AT\":\"%d\",\"CT\":\"%d\",\"BR\":\"0\"}",
		device_info.save.srv_login,nb.nb_imei,nb.nb_iccid,device_info.save.srv_time,device_info.save.SENSOR_SAM_TIME);					
		//�༭ͬ���������
		if(tcp_udp_send_data() == FALSE)
			return FALSE;
		device_info.Params_sever=0;
	  Trace_log("��һ�η���ͬ���������\r\n");
	}
	
	if(device_info.alarm_flag_STATUS.flasherror==0)   //flashû�й���
	{		
		adds = device_get_flahaddr_addr();  //��ȡ���һ����ʷ���ݣ������ϱ�������洢����һ����
		hist_read(&wl_hist_data, adds);
	
		if(wl_hist_data.data_time.year == 255||	wl_hist_data.data_time.month == 255)
		{
			Trace_log("û�д洢���� ����ʵʱ����\r\n");
			shuju_ok = 0;
		}
		else if(wl_hist_data.data_time.year != device_info.data_time.year ||	wl_hist_data.data_time.month != device_info.data_time.month || wl_hist_data.data_time.day !=device_info.data_time.day)
		{
			Trace_log("��ʷ��洢���� ����ʵʱ����\r\n");
			shuju_ok = 0;
		}
		else
			shuju_ok = 1;
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
	
//	if( device_info.save.tab_protocol == UDP)
//	{					
		memset(transmit_cmd,0,sizeof(transmit_cmd));
		sprintf((char *)transmit_cmd,"{\"t\":\"20%02d%02d%02d%02d%02d%02d\",\"CH0\":%.2f,\"CH1\":%.2f,\"CH2\":%.2f,\"CH3\":%.2f,\"CH4\":%.4f,\"CH5\":%.4f,\"CH6\":%.4f,\"CH7\":%.4f,\"CH8\":%.4f,\"CH9\":%.4f,\"Q\":%d,\"R\":%d}",
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
		
		Trace_log(transmit_cmd);	
		Trace_log("\r\n");
		if(tcp_udp_send_data() == FALSE)
			return FALSE;			
//	}
		
	delay_vtask(1000);
	uart_send_ack(&GPRS, "AT+QICLOSE=0\r\n", wireless_buf, "CLOSE OK", sizeof(wireless_buf), 300);
		
	return TRUE;
}
/***************************************************************************************/
/***************************************************************************************/
uint8_t nbiot_send_data(void)
{
	if( device_info.save.sever_type == NB_tlink || device_info.save.sever_type == NB_telecom)
	{	
		Trace_log(transmit_cmd);	
		Trace_log("\r\n");					
		str_to_hex16(wireless_buf,transmit_cmd,BUFSIZ10);		
		
		if(device_info.save.tab_module == BC95)
		{
			if(device_info.save.sever_type == NB_tlink)
			{
				sprintf((char *)transmit_cmd, "AT+QLWULDATAEX=%d,%s,0x0100\r\n",BUFSIZ10 ,wireless_buf);
			}
			else //if(device_info.save.sever_type == Telecom)
			{					
				sprintf((char *)transmit_cmd, "AT+QLWULDATAEX=%d,02%x%s,0x0100\r\n",(BUFSIZ10+2),BUFSIZ10,wireless_buf);
			}
			uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "OK", sizeof(wireless_buf),300);
			if(BC95_wait_send_status() == FALSE)    
				return FALSE;
			else
				device_info.Params_sever=0;
		}
		else
		{
			if(device_info.save.sever_type == NB_tlink)
			{
				sprintf((char *)transmit_cmd, "AT+QLWDATASEND=19,0,0,%d,%s,0x0100\r\n",BUFSIZ10 ,wireless_buf);
			}
			else //if(device_info.save.sever_type == Telecom)
			{					
				sprintf((char *)transmit_cmd, "AT+QLWDATASEND=19,0,0,%d,02%x%s,0x0100\r\n",(BUFSIZ10+2),BUFSIZ10,wireless_buf);
			}
			uart_send_ack(&GPRS, "AT+QLWCFG=\"dataformat\",1,1\r\n", wireless_buf, "OK", sizeof(wireless_buf), 300);
			if(!uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "SEND OK", sizeof(wireless_buf),30000))
			{  	
				device_info.Params_sever=0;
			}
			else 
			{
				uart_send_ack(&GPRS, "AT+QLWCLOSE\r\n", wireless_buf, "CLOSE OK", sizeof(wireless_buf), 300);
				uart_transmit_str(&GPRS, (uint8_t *)"AT+QLWOPEN=1\r\n");						
				if(!check_recv2(&GPRS, wireless_buf,"+QLWOBSERVE: 0,19,0", "ERROR",sizeof(wireless_buf), 20000))
				{			
					if(!uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "SEND OK", sizeof(wireless_buf), 6000))
					{			
						device_info.Params_sever=0;						
					}
					else
						return FALSE;
				}
				else
					return FALSE;
			}
		}
	}		
	else if(device_info.save.sever_type == NB_onenet)
	{		   
		 if(nb.mgid==0)
		 {
			 return FALSE; 
		 }
		 memcpy(wireless_buf,transmit_cmd,BUFSIZ10);
		 sprintf((char *)transmit_cmd, "AT+MIPLNOTIFY=0,%d,3200,0,5750,1,%d,\"%s\",0,0,255\r\n",nb.mgid,BUFSIZ10,wireless_buf);
		 if(!uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "+MIPLEVENT: 0,26", sizeof(wireless_buf), 8000))
		 {							 
			 device_info.Params_sever=0;	
		 }
		 else
			 return FALSE;
	}
	
	return TRUE;
}
/***************************************************************************************/
uint8_t  nb_sevebuf(void )   //NBIOTƽ̨��������
{			
	uint8_t i=0;
  uint8_t baoj_id,baoj_status;
	float baoj_limit_h,baoj_limit_f,baoj_limit_l,baoj_data;
//	uint8_t write_ack_num=8;
//	uint16_t rx_size_ack=0;
	uint32_t adds;
//	char *ackp;
	uint8_t shuju_ok = 0;
//	uint8_t nb_seveok = 0;
//	uint32_t ackid=0;
//	memset(sevebuf,0,output_len);
//	memset(sevebuf2,0,output_len1);
	memset(transmit_cmd,0,sizeof(transmit_cmd));
	
	if( device_info.save.sever_type == NB_tlink || device_info.save.sever_type == NB_telecom)  //updata ���
	{
		if(device_info.save.tab_module == BC95)
		{
			if(BC95_waitLWM2M_connect_status() == FALSE)
				return 1;
		}
		else
		{
			uart_transmit_str(&GPRS,(uint8_t *)"AT+QLWUPDATE\r\n");
			if(check_recv3(&GPRS, wireless_buf, "UPDATE OK", "ERROR","UPDATE FAIL",sizeof(wireless_buf), 30000))   //���Updata�Ƿ�ɹ�
			{	
				return 1;
			}
		}
	}
	else if( device_info.save.sever_type == NB_onenet )
	{
		uart_transmit_str(&GPRS,(uint8_t *)"AT+MIPLUPDATE=0,86400,0\r\n");
		if(check_recv2(&GPRS, wireless_buf, "OK", "ERROR",sizeof(wireless_buf), 300))   //���Updata�Ƿ�ɹ�
		{	
				return 1;
		}
		else
		{
			if(uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "+MIPLEVENT: 0,11", sizeof(wireless_buf),30000))   //�ϱ��ɹ�
				return 1;
		}
	}
	
	getParam();   //���ݻ�ȡ
	
	if(device_info.Params_sever==1 )  //��һ�η���ͬ������
	{	
		Trace_log("��һ�η���ͬ������\r\n");
		if( device_info.save.sever_type == NB_tlink)
			sprintf((char *)transmit_cmd,"{\"params\":{\"AT\":%d,\"CT\":%d,\"BR\":0}}",device_info.save.srv_time,device_info.save.SENSOR_SAM_TIME);
		else
			sprintf((char *)transmit_cmd,"{\"AT\":%d,\"CT\":%d,\"BR\":0}",device_info.save.srv_time,device_info.save.SENSOR_SAM_TIME);					
		BUFSIZ10=strlen((char *)transmit_cmd);				
		//�༭ͬ���������
		if(nbiot_send_data() == FALSE)
			return FALSE;
		
		sprintf((char *)transmit_cmd,"{\"t\":\"20%02d%02d%02d%02d%02d%02d\",\"IMEI\":\"%s\",\"ICCID\":\"%s\"}",
		device_info.data_time.year,device_info.data_time.month,device_info.data_time.day,device_info.data_time.hour,device_info.data_time.min,device_info.data_time.sec,nb.nb_imei,nb.nb_iccid);	
				
		BUFSIZ10=strlen((char *)transmit_cmd);				
		//�༭ͬ���������
		if(nbiot_send_data() == FALSE)
			return FALSE;
		
		device_info.Params_sever=0;
	  Trace_log("��һ�η���ͬ���������\r\n");
	}
								
	if(device_info.alarming_flag != 0 )      //����
	{ 
		Trace_log("�����ϱ�");
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
			//////////////////���ݷ��Ͳ���/////////////////////////////////////////////
			sprintf((char *)transmit_cmd,"{\"WRID\":%d,\"ACTION\":%d,\"WRDATA\":%.4f,\"LIMIT_H\":%.4f,\"LIMIT_L\":%.4f,\"LIMIT_F\":%.4f}",
			baoj_id,baoj_status,baoj_data,baoj_limit_h,baoj_limit_l,baoj_limit_f);	
	
			BUFSIZ10=strlen((char *)transmit_cmd);
			if(nbiot_send_data() == FALSE)
				return FALSE;
			device_info.alarming_flag--;
			device_info.alarm_flag_ID.ch_status[i] = 0;
			if(device_info.alarming_flag<=0 || device_info.alarming_flag>30)
				device_info.alarming_flag = 0;				
		}
	}		
//	else   //device_info.alarming_flag=0   ������������ ���ݰ�׼��
	//�����ж�
	if(device_info.alarm_flag_STATUS.flasherror==0)   //flashû�й���
	{
		adds = device_get_flahaddr_addr();  //��ȡ���һ����ʷ���ݣ������ϱ�������洢����һ����
		hist_read(&wl_hist_data, adds);
		if(wl_hist_data.data_time.year == 255||	wl_hist_data.data_time.month == 255)
		{
			Trace_log("û�д洢���� ����ʵʱ����\r\n");
			shuju_ok = 0;
		}
		else if(wl_hist_data.data_time.year != device_info.data_time.year ||	wl_hist_data.data_time.month != device_info.data_time.month || wl_hist_data.data_time.day !=device_info.data_time.day)
		{
			Trace_log("��ʷ��洢���� ����ʵʱ����\r\n");
			shuju_ok = 0;
		}
		else
			shuju_ok = 1;
	}
					
	////��ʷ����
	if(shuju_ok == 1)   //flash������������ʷ�洢    ���Ͷϵ���������
	{
	  i=0;
		device_info.pend_num=device_pending_num_get();
		device_info.pend_addr=device_perv_addr_get();
		while(device_info.pend_num)
		{
			Trace_log("��ʷ����");
			hist_read(&wl_hist_data, device_info.pend_addr);    //��ȡ��ʷ
			if(wl_hist_data.data_time.year==0 && wl_hist_data.data_time.month==0)
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
			
			if(device_info.pend_num>1 )  //
	    {				
				memset(transmit_cmd,0,sizeof(transmit_cmd));
				sprintf((char *)transmit_cmd,"{\"t\":\"20%02d%02d%02d%02d%02d%02d\",\"CH0\":%.2f,\"CH1\":%.2f,\"CH2\":%.2f,\"CH3\":%.2f,\"CH4\":%.4f,\"CH5\":%.4f,\"CH6\":%.4f,\"CH7\":%.4f,\"CH8\":%.4f,\"CH9\":%.4f,\"Q\":%d,\"R\":%d}",
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
	    								
        BUFSIZ10=strlen((char *)transmit_cmd);
				if(nbiot_send_data() == FALSE)
					return FALSE;
		  }
				// ���õ�ַ�������� /
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
		adds = device_get_flahaddr_addr();  //��ȡ���һ����ʷ���ݣ������ϱ�������洢����һ����
		hist_read(&wl_hist_data, adds);
	}
	
	Trace_log("ʵʱ���ݷ���\r\n");		
	sprintf((char *)transmit_cmd,"{\"t\":\"20%02d%02d%02d%02d%02d%02d\",\"Q\":%d,\"R\":%d,\"CH0\":%.2f,\"CH1\":%.2f,\"CH2\":%.2f,\"CH3\":%.2f,\"CH4\":%.4f,\"CH5\":%.4f,\"CH6\":%.4f,\"CH7\":%.4f,\"CH8\":%.4f,\"CH9\":%.4f}",
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
	BUFSIZ10=strlen((char *)transmit_cmd);
	if(nbiot_send_data() == FALSE)
			return FALSE;			
	Trace_log("�����ݳɹ�\r\n");	
	//uart_send_ack(&GPRS, "AT+QNBIOTRAI=1\r\n", wireless_buf2, "OK", 256, 300);
//  Trace_log("NB��������\r\n");		
//	SET_CFUN(0);
//	delay_vtask(2000);
//	uart_send_ack(&GPRS, "AT+CPSMS=1,,,\"00111000\",\"00000000\"\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500);   //TAU time 24H   active time 0s		
//	uart_send_ack(&GPRS, "AT+QNBIOTRAI=1\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500);
	SET_CFUN(0);
	SET_QRELLOCK();	  //����AT�������ʱ������ģ����ٽ�������		
	return TRUE;
}
/***************************************************************************************/
uint8_t task_nb(void)
{		
  uint8_t again_tosever = 0;
	begin_nb:
	PCIE_PW_OPEN(); 	//ģ���ϵ�	
	delay_vtask(200);
	if(again_tosever == 0 && device_info.event_again == 0) //���߻�������
	{
		Trace_log("NB��ʼ����\r\n");
   	if(nb_psmstart())   //�豸����ʧ�ܣ��豸���¿���
		{	 
			nb.device_restart_num++;  //��¼��������
			device_restart_num_set(nb.device_restart_num);
			if(nb_open())   //�豸������������ʧ�ܣ�ֱ�ӽ���
				goto end_nb;
		}
	}
	else
		if(again_tosever==1 || device_info.event_again == 1)   //�豸����
		{
//			device_info.event_again = 0;
			Trace_log("�豸��������\r\n");
//			delay_vtask(1000);           //
			nb.device_restart_num++;  //��¼��������
			device_restart_num_set(nb.device_restart_num);
			if(nb_open())   //�豸������������ʧ�ܣ�ֱ�ӽ���
				goto end_nb;
		}
			

		if(!nb_init())	  //����ע�ᣬ��ʼ���ɹ�
		{	
			if((nb.nb_imei[0]<'0' || nb.nb_imei[0]>'9')|| (nb.nb_iccid[0]<'0'||nb.nb_iccid[0]>'9')||(nb.nb_imei[0]<'0'||nb.nb_imei[0]>'9'))  //��ȡģ����Ϣ
			{
				Trace_log("��ȡģ����Ϣ\r\n");
				get_nb_message();	
			}			

			if(device_info.event_again == 1)
			{
				if(!nb_link_sever())
					device_info.event_again = 0;
				else
					if(again_tosever==0)
					{
						 again_tosever=1;	
						 nb_OFF();
						 PCIE_PW_CLOSE(); 	//ģ��ϵ�
						 delay_vtask(5000);
						 goto begin_nb;   //����
					}
					else
						goto end_nb;
			}
			
			if(!nb_sevebuf())  //���ݷ��ͳɹ�
			{	
				again_tosever=0;
				 return 0;
			}
			else 	 
			 if(!nb_link_sever())   //�������ӷ�����
			 {
				 if(!nb_sevebuf())	
				 {
					 again_tosever=0;
					 return 0;
				 }
         else
          if(again_tosever==0)
				 {
					 again_tosever=1;	
           nb_OFF();
		       PCIE_PW_CLOSE(); 	//ģ��ϵ�					 
				   goto begin_nb;   //����
				 }					 
			 }
			 else
			 {
				 if(again_tosever==0)
				 {
					 again_tosever=1;	
           nb_OFF();
		       PCIE_PW_CLOSE(); 	//ģ��ϵ�					 
				   goto begin_nb;   //����
				 }
			 }			 
	  } 		

	
	end_nb:
		nb_OFF(); 
		PCIE_PW_CLOSE();	//ģ��ϵ�
	  device_info.event_again = 1;
		if(device_info.save.device_dabao == OFF)
		{
			Trace_log("����ʧ�ܣ����ݱ���\r\n");
			if(	device_info.pend_num ==0)//���ݱ������ϵ�����
			{	
				device_info.pend_addr=device_get_flahaddr_addr();///////////////*//////////
				device_perv_addr_set();
			}
			device_info.pend_num =device_info.pend_num + 1;
			device_pending_num_set();
		}
		else
			Trace_log("����ʧ��\r\n");
		nb.nb_csq = 0;
		clear_allalarm();
	return 1;
}
/***************************************************************************************/
uint8_t task_udpDZDG(void)
{
	uint8_t again_tosever = 0;
	uint8_t rets = FALSE;
//	uint8_t sev_ret = FALSE;
//	uint8_t GNSS_WORK = 0;
	begin_nb:
	PCIE_PW_OPEN(); 	//ģ���ϵ�	
	Trace_log("�豸��������\r\n");

	if(!nb_open())   //�豸������������ʧ�ܣ�ֱ�ӽ���
  {					
		if(!nb_init())	  //����ע�ᣬ��ʼ���ɹ�
		{	
			if((nb.nb_imei[0]<'0' || nb.nb_imei[0]>'9')|| (nb.nb_iccid[0]<'0'||nb.nb_iccid[0]>'9')||(nb.nb_imei[0]<'0'||nb.nb_imei[0]>'9'))  //��ȡģ����Ϣ
			{
				Trace_log("��ȡģ����Ϣ\r\n");
				get_nb_message();					
			}
			
			delay_vtask(100);				 
		  if(!TCP_UDP_link_sever())   //���ӷ������ɹ�
		  {
				rets = TCP_UDP_sevebuf();  //��������   ��׼���� 
			  if(rets == FALSE)	  //��������ʧ��   
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
			again_tosever = 0;
     if(again_tosever==1)
		 {
//			 again_tosever=0;	
			 nb_OFF();
			 PCIE_PW_CLOSE(); 	//ģ��ϵ�					 
			 goto begin_nb;   //����
		 }		
	}
   			  
	nb_OFF(); 
	PCIE_PW_CLOSE();	//ģ��ϵ�
	
	if(rets == FALSE)
	{
	  Trace_log("����ʧ��\r\n");
		nb.nb_csq = 0;
		clear_allalarm();
  }
	else
		Trace_log("���ͳɹ�\r\n");
	
	return rets;
}
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
uint16_t str_to_hex16(uint8_t *buf, uint8_t * format, uint16_t us_length)
{
	uint8_t tempcode;
	uint16_t uslength = us_length * 2,us_count = 0,us_data;

	for(us_data = 0;us_count < uslength; us_count += 2,us_data++)
	{
		tempcode = format[us_data] >> 4;

		if(tempcode <= 9)
		{
			buf[us_count] = tempcode + '0';
		}
		else
		{
			buf[us_count] = tempcode - 10 + 'A';
		}
		tempcode = format[us_data] & 0X0F;
		if(tempcode <= 9)
		{
			buf[us_count+1] = tempcode + '0';
		}
		else
		{
			buf[us_count+1] = tempcode - 0X0A + 'A';
		}
	}
	buf[uslength] = '\0';
	return uslength;
}
/*
*********************************************************************************************************
*	�� �� ��: BcdToChar
*	����˵��: ��BCD��תΪASCII�ַ��� ���� 0x0A ==> 'A'
*	��    ��: _bcd   ������Ķ�������������С��16
*	�� �� ֵ: ת�����
*********************************************************************************************************/
/***************************************************************************************/
void hex16_to_str(uint8_t  *s_inchar, uint16_t s_len, uint8_t  *a_outtxt)
{

	uint16_t size1=0,i=0;
		 for( size1=0;size1<s_len; size1++)
		{
			
			if(	s_inchar[size1]>='0' && s_inchar[size1] <= '9')
			{
				a_outtxt[size1] = s_inchar[size1] - 0x30;
			}
			else if(	s_inchar[size1]>='A' && s_inchar[size1] <= 'F')
			{
				a_outtxt[size1] = s_inchar[size1] - 'A'+10;
			}
			else if(	s_inchar[size1]>='a' && s_inchar[size1] <= 'f')
			{
				a_outtxt[size1] = s_inchar[size1] - 'a'+10;
			}

		}
	 for( size1=0,i=0;size1<s_len; size1++,i++)
	 {
			a_outtxt[i]=(a_outtxt[size1]<<4)|a_outtxt[++size1];			
	 }
	  a_outtxt[i] = '\0';
	    
}
/***************************************************************************************/





