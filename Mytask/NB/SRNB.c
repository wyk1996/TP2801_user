#include "main.h"



/*********************************************************/
static uint8_t srnb_open(void)
{
//	TAB_RST();   ///  ������
//	LL_GPIO_ResetOutputPin(pwrio_gpio, pwrio_gpio_id);    
//	delay_vtask(500); 
//	LL_GPIO_SetOutputPin(pwrio_gpio, pwrio_gpio_id);
//	delay_vtask(1200);
//	LL_GPIO_ResetOutputPin(pwrio_gpio, pwrio_gpio_id);
//	delay_vtask(1000);
	if(TEST_AT(20) != 0)   //���AT
	{
			return FALSE;
	}
	Trace_log("ģ�鿪���ɹ�\r\n");
	if(TEST_CPIN())    //���SIM��
		return FALSE;
	SET_ATE(1);
	return TRUE;
}
/*********************************************************/
uint8_t srnb_init(void)    //ģ���ʼ��
{
	
	uart_send_ack(&GPRS, "AT+CPSMS=0\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500);    //�ر�PSMģʽ
	uart_send_ack(&GPRS, "AT+CEDRXS=0,5,\"0101\"\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500);   //�ر�eDRX
	uart_send_ack(&GPRS, "AT+CGMR\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500);     //��ȡģ����Ϣ
	
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
	
	uart_send_ack(&GPRS, "AT+CGPADDR=0\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500);  //��ʾ PDP ��ַ
	uart_send_ack(&GPRS, "AT+CSCON?\r\n", wireless_buf, "OK", sizeof(wireless_buf), 5000);  //��������״̬
	
	Trace_log("ģ���ʼ���ɹ�\r\n");
	return 0;	
}

/*********************************************************/
uint8_t SRNB_link_sever(void )   //TCP/UDPƽ̨����
{
	uint8_t rx_size_ack;
	char *ackp;
	uart_send_ack(&GPRS, "AT+ECSTATUS=PHY\r\n", wireless_buf, "OK", sizeof(wireless_buf), 500);  //��������״̬
	sprintf((char *)transmit_cmd, "AT+CTM2MSETPM=\"%s\",%s,86400\r\n", device_info.save.srv_addr,device_info.save.srv_port);    //set connect ip and port
	uart_send_ack(&GPRS, transmit_cmd , wireless_buf, "OK", sizeof(wireless_buf), 300);
	if(!uart_send_ack(&GPRS, "AT+CTM2MREG\r\n", wireless_buf, "+CTM2M: reg,0", sizeof(wireless_buf), 5000)) //����ע��
	{
		Trace_log("ע��ɹ�\r\n");
	}
	else
		return FALSE;
//	rx_size_ack = uart_recv_data(&GPRS, wireless_buf2, recive_len,5000);
	if(!uart_send_ack(&GPRS, "", wireless_buf, "+CTM2M: obsrv,0", sizeof(wireless_buf), 5000))
	{
		Trace_log("���ĳɹ�\r\n");
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
		Trace_log("����ʧ��\r\n");
		return FALSE;
	}
	Trace_log("���ӳɹ�\r\n");	
	return TRUE;
}
/*********************************************************/
uint8_t  srnb_sevebuf(void )   //NBIOTƽ̨��������
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
		Trace_log("��ʷ��洢���� ����ʵʱ����\r\n");
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
	transmit_cmd[i++] = 0x68;      //��ʼ��
	transmit_cmd[i++] = 0x00;      //������Ϣ
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x01;      //Э��汾��
	transmit_cmd[i++] = 0x01;      //���̴���
	transmit_cmd[i++] = 0x07;      //������
	transmit_cmd[i++] = 0x00;      //���ܱ�־
	for(j=0;j<20;j++)         //ICCID
		transmit_cmd[i++] = nb.nb_iccid[j];
	len_add = i;
	transmit_cmd[i++] = 0x00;    //���ݳ���
	transmit_cmd[i++] = 0x00;
	//������
	if(strlen((char *)device_info.save.srv_login)<16)     //���
		k = 16-strlen((char *)device_info.save.srv_login);
	for(j=0;j<k;j++)
		transmit_cmd[i++] = 0x20;
	for(j=0;j<strlen((char *)device_info.save.srv_login);j++)
		transmit_cmd[i++] = device_info.save.srv_login[j];
	
	transmit_cmd[i++] = 0x01;    //�ϱ�����   ����
	transmit_cmd[i++] = device_info.save.srv_time;       //����ֵ
	transmit_cmd[i++] = 0x00;   //�ϱ�ʱ��
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = hex_to_BCD(wl_hist_data.data_time.year);    //����ʱ�� 6�ֽ�  16����BCD���ʾ  2014-5-23 10:00:00 140523100000
	transmit_cmd[i++] = hex_to_BCD(wl_hist_data.data_time.month);
	transmit_cmd[i++] = hex_to_BCD(wl_hist_data.data_time.day);
	transmit_cmd[i++] = hex_to_BCD(wl_hist_data.data_time.hour);
	transmit_cmd[i++] = hex_to_BCD(wl_hist_data.data_time.min);
	transmit_cmd[i++] = hex_to_BCD(wl_hist_data.data_time.sec);
	transmit_cmd[i++] = 0x00;      //������
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;     //��������
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;     //����˲ʱ����
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	
	da = (uint16_t)(wl_hist_data.data.ch2_value*10);    //�¶�
	transmit_cmd[i++] = da;
	transmit_cmd[i++] = da>>8;
	da = (uint16_t)(wl_hist_data.data.ch1_value*10);     //ѹ��
//	da = 2345;
	transmit_cmd[i++] = da;
	transmit_cmd[i++] = da>>8;
	
	da = (113+nb.nb_csq)/2;        //�ź�ǿ��
	transmit_cmd[i++] = da;
	transmit_cmd[i++] = 0x06;      //�������
	transmit_cmd[i++] = 0x00;      //����״̬
	transmit_cmd[i++] = 0x02;      //��������   ﮵��
	da = (uint16_t)device_info.vbat_bfb;
	transmit_cmd[i++] = da;        //���1��ص����ٷֱ�
	transmit_cmd[i++] = 0x00;      //���2��ص����ٷֱ�
	
	transmit_cmd[i++] = 0x0f;    //IMSI����
	transmit_cmd[i++] = 0x00;     
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;     
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	
	transmit_cmd[i++] = 0x00;     //�������
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	
	transmit_cmd[i++] = 0x00;     //���½������
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	
	transmit_cmd[i++] = 0x00;     //���½���ʱ��
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	transmit_cmd[i++] = 0x00;
	
	transmit_cmd[i++] = 0x00;     //��
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
	Trace_log("�ȴ�����\r\n");
	uart_send_ack(&GPRS, "", wireless_buf, "CTM2M", sizeof(wireless_buf), 500);   //���һ�½��ջ�����
	if(!uart_send_ack(&GPRS, transmit_cmd, wireless_buf, "+CTM2MSEND:", sizeof(wireless_buf), 40000))
	{		
//			uart_send_ack(&GPRS, "", wireless_buf2, "OK", recive_len, 10000);
		if(strstr((const char *)wireless_buf, "+CTM2M: send,") != NULL)
		{
			Trace_log("�����������\r\n");
		}
		else 
		{
			Trace_log("��������ʧ��\r\n");
			return FALSE;
		}

		device_info.Params_sever=0;	
	}
	else
	{
		Trace_log("����ʧ��\r\n");
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
				Trace_log("��������֤��ɲ�����\r\n");
			}
			else
			{
				Trace_log("��������֤ʧ��\r\n");
				return FALSE;
			}
		}
		else
		{
			Trace_log("�յ�Ӧ����󣬷���ʧ��\r\n");
			return FALSE;
		}		
	}
	else
		Trace_log("û���յ�Ӧ��\r\n");
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
	PCIE_PW_OPEN(); 	//ģ���ϵ�	
	Trace_log("�豸��������\r\n");

	if(srnb_open() == TRUE)   //�豸������������ʧ�ܣ�ֱ�ӽ���
  {					
		if(!srnb_init())	  //����ע�ᣬ��ʼ���ɹ�
		{	
			if((nb.nb_imei[0]<'0' || nb.nb_imei[0]>'9')|| (nb.nb_iccid[0]<'0'||nb.nb_iccid[0]>'9')||(nb.nb_imei[0]<'0'||nb.nb_imei[0]>'9'))  //��ȡģ����Ϣ
			{
				Trace_log("��ȡģ����Ϣ\r\n");
				get_nb_message();				
			}
			
			delay_vtask(100);				 
		  if(SRNB_link_sever() == TRUE)   //���ӷ������ɹ�
		  {
				rets = srnb_sevebuf();
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
			again_tosever=0;	
     if(again_tosever==1)
		 {
//			 again_tosever=0;	
//			 nb_OFF();
			 PCIE_PW_CLOSE(); 	//ģ��ϵ�					 
			 goto begin_nb;   //����
		 }		
	}
   			  
//	nb_OFF(); 
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

