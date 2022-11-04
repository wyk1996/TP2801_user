#include "main.h"
#include "lora.h"


LORA_databuf lora_buf;

union valReg
{
char data[4];
float fval;
};

union valReg val;
union valReg2
{
char data2[4];
float fval2;
};

union valReg2 val2;
/**********************************************************************************/
uint8_t cs_Check( uint8_t *buf,uint8_t lensizeof)
{
   uint8_t  num = 0;
   for (int i = 0; i <=lensizeof; i++)
   {
     num = (num + buf[i]) ;//% 256;
   }
	 return num;	 
}
/**********************************************************************************/
void lora_rest(void)
{
//  LL_GPIO_ResetOutputPin(pwrio_gpio, pwrio_gpio_id);
//	delay_vtask(5);
//	LL_GPIO_SetOutputPin(pwrio_gpio, pwrio_gpio_id);
}
/**********************************************************************************/
void lora_ext(void)   //lora唤醒
{
//  lora_gpioext();	
	LL_GPIO_ResetOutputPin(vddext_gpio, vddext_gpio_id);
	delay_vtask(8);
	LL_GPIO_SetOutputPin(vddext_gpio, vddext_gpio_id);
}
	
/**********************************************************************************/
uint8_t lora_data(void)
{ 
//	uint16_t cre_ce;
//	uint8_t crc_hl[2];
//	uint8_t i=0;
//  uint8_t csnum=0;	
//	uint16_t len_i=0;
/*
	memset(wireless_buf,0,sizeof(wireless_buf));
	memset(transmit_cmd,0,sizeof(transmit_cmd));
	vTaskSuspendAll();  
	// if(MPAbuf.Pressure!=0 &MPAbuf.Temperature !=0)
 //  {
	
		 val.fval=MPAbuf.sensor2data;
     val2.fval2=MPAbuf.sensor1data;
	
	// }
	transmit_cmd[len_i++]=0x3C;
	transmit_cmd[len_i++]=0x17;
	transmit_cmd[len_i++]=0;//len  

	transmit_cmd[len_i++]=0x55;                           //数据头
	transmit_cmd[len_i++]=0xAA;
	transmit_cmd[len_i++]=atoi((char*)device_info.save.srv_UserName);  //地址
	if(device_info.save.tab_sensor == D032 || device_info.save.tab_sensor == TP2321_1 || device_info.save.tab_sensor == D033 || device_info.save.tab_sensor == D020  || device_info.save.tab_sensor == TP2313)
	   transmit_cmd[len_i++]=0x01;                                   //类型压力
	else
	if(device_info.save.tab_sensor == D035 ||device_info.save.tab_sensor == D036 || device_info.save.tab_sensor == TP2321 || device_info.save.tab_sensor == YW_ZDY || device_info.save.tab_sensor == TP2323)	
		 transmit_cmd[len_i++]=0x02;         //类型水位
	else
	if(device_info.save.tab_sensor == TP2305V2)
		transmit_cmd[len_i++]=0x03;         //温湿度
	else
	if(device_info.save.tab_sensor == TP2302V2 || device_info.save.tab_sensor == TP2303V1 ||device_info.save.tab_sensor == DS18B20)
		transmit_cmd[len_i++]=0x04;         //温度
  else
	if(device_info.save.tab_sensor == TP2312)
		transmit_cmd[len_i++]=0x05;         //TDS
	else
	if(device_info.save.tab_sensor == TP2319)
		transmit_cmd[len_i++]=0x06;         //可燃气体
	else
	if(device_info.save.tab_sensor == TP2315)
		transmit_cmd[len_i++]=0x07;         //水浸传感器
	else
		transmit_cmd[len_i++]=0x00;         //未定义
	transmit_cmd[len_i++]=val.data[0];      //压力
	transmit_cmd[len_i++]=val.data[1]; 
	transmit_cmd[len_i++]=val.data[2]; 
	transmit_cmd[len_i++]=val.data[3]; 

	transmit_cmd[len_i++]=val2.data2[0];     //温度
	transmit_cmd[len_i++]=val2.data2[1]; 
	transmit_cmd[len_i++]=val2.data2[2]; 
	transmit_cmd[len_i++]=val2.data2[3]; 

	transmit_cmd[len_i++]=(uint8_t)device_info.vbat_bfb;  //电量
	transmit_cmd[len_i++]= nb.nb_csq & 0xff;    //信号  14
	
	if(device_info.alarming_flag != 0)  //报警标志 
	{
		device_info.alarming_flag = 0;
	  transmit_cmd[len_i++]=0x01;
	}
	else 
	{
		transmit_cmd[len_i++]=0x00;
	}
		
	val.fval=device_info.save.sensor2_up;     //压力上限
  val2.fval2=	device_info.save.sensor2_down;   //压力下限

	transmit_cmd[len_i++]=val.data[0];   
	transmit_cmd[len_i++]=val.data[1]; 
	transmit_cmd[len_i++]=val.data[2]; 
	transmit_cmd[len_i++]=val.data[3]; 
	
	transmit_cmd[len_i++]=val2.data2[0]; 
	transmit_cmd[len_i++]=val2.data2[1]; 
	transmit_cmd[len_i++]=val2.data2[2]; 
	transmit_cmd[len_i++]=val2.data2[3]; 
	
	val.fval=device_info.save.sensor1_up;    //温度上限
  val2.fval2=	device_info.save.sensor1_down;    //温度下限

	transmit_cmd[len_i++]=val.data[0];
	transmit_cmd[len_i++]=val.data[1]; 
	transmit_cmd[len_i++]=val.data[2]; 
	transmit_cmd[len_i++]=val.data[3]; 
	
	transmit_cmd[len_i++]=val2.data2[0]; 
	transmit_cmd[len_i++]=val2.data2[1]; 
	transmit_cmd[len_i++]=val2.data2[2]; 
	transmit_cmd[len_i++]=val2.data2[3]; 
	
  for(i=0;i<len_i-3;i++)   //CRC
	{
	  wireless_buf[i]=transmit_cmd[3+i];
	}
	cre_ce=crc_16(wireless_buf,len_i-3);
	crc_hl[0]=cre_ce>>8; 
	crc_hl[1]=cre_ce &0xff;
		
	transmit_cmd[len_i++]=crc_hl[0];
	transmit_cmd[len_i++]=crc_hl[1];

	
	transmit_cmd[2]=len_i-3;//len  agian
	csnum=cs_Check(transmit_cmd,len_i);
		
	transmit_cmd[len_i++]=csnum; 
	transmit_cmd[len_i++]=0x0D;
	xTaskResumeAll();
	
//	Trace_log("发送数据：");
	if(device_info.debug_en) 
  uart_transmit_buf(&COM0,transmit_cmd,len_i);
	
	for(uint8_t i=0;i<3;i++)
	{
		lora_ext();			
		if(uart_send_buff_ack(&GPRS, transmit_cmd,len_i, wireless_buf,recive_len, "OK", 1000))
		{ 
			Trace_log("发送失败");
//			return 0;	
		}
		else
		{ 
			Trace_log("发送成功");	
			return 1;		
		}	
	}*/
	return 0;
}

/***************************************************************************************/
uint16_t lora_get_NSID(void)  //读取lora网络号
{
//发送:3C 07 00 43 0D 
//接收:3C 87 02 00 01 C6 0D 

	uint16_t	NSIDbuf[2]={0};
	uint16_t NSID=0; 
	uint8_t lorasevebuf[6]={0};
	lorasevebuf[0]=0x3C;
	lorasevebuf[1]=0x07;
	lorasevebuf[2]=0x00;
	lorasevebuf[3]=0x43;
	lorasevebuf[4]=0x0D;
	for(uint8_t i=0;i<3;i++)
	{
		lora_ext();
		if( !uart_send_buff_ack(&GPRS, lorasevebuf,5, wireless_buf, recive_len, "<", 1000))
		{
			if(wireless_buf[1]==0x87 && wireless_buf[2]==0x02)
			{
			NSIDbuf[0]=wireless_buf[3];	
			NSIDbuf[1]=wireless_buf[4];
			NSID=NSIDbuf[0]<<8 | NSIDbuf[1];
			}   
			//uart_transmit_buf(&COM0,wireless_buf2,20);
		}	
	}
	return NSID;
}
/**********************************************************************************/
static uint32_t lora_get_BAND(uint8_t *sudu)   //uint8_t *sudu    读取lora的频率和速度
{
//发送:3C 03 00 3F 0D 
//接收:3C 83 09 55 AA 07 19 CF 0E 40 3C 81 C1 0D 
	uint32_t Bandbuf[4];
	uint32_t Band=0; 
	uint8_t lorasevebuf[8]={0};
	
	lorasevebuf[0]=0x3C;
	lorasevebuf[1]=0x03;
	lorasevebuf[2]=0x00;
	lorasevebuf[3]=0x3F;
	lorasevebuf[4]=0x0D;
	
	for(uint8_t i=0;i<3;i++)
	{
		lora_ext();
		if( !uart_send_buff_ack(&GPRS, lorasevebuf,5, wireless_buf,  recive_len,"<", 255))
		{
			if(wireless_buf[1]==0x83 && wireless_buf[2]==0x09)
			{	
				*sudu=wireless_buf[5];//////////////////
				Bandbuf[0]=wireless_buf[6];	
				Bandbuf[1]=wireless_buf[7];	
				Bandbuf[2]=wireless_buf[8];	
				Bandbuf[3]=wireless_buf[9];		
				Band=	Bandbuf[0]<<24|Bandbuf[1]<<16|Bandbuf[2]<<8|Bandbuf[3];
			}    
		}
  }
	return  Band;
}
/**********************************************************************************/
uint8_t lora_get_Txpower(void)   //读取发射功率
{
//发送:3C 0D 00 49 0D 
//接收:3C 8D 01 14 DE 0D 
uint8_t Tx_power = 0; 
uint8_t lorasevebuf[10]={0};

	lorasevebuf[0]=0x3C;
	lorasevebuf[1]=0x0D;
	lorasevebuf[2]=0x00;
	lorasevebuf[3]=0x49;
	lorasevebuf[4]=0x0D;	  
  for(uint8_t i=0;i<3;i++)
	{
		lora_ext();
		if( !uart_send_buff_ack(&GPRS, lorasevebuf,5, wireless_buf, recive_len,  "<",255))
		{
			if(wireless_buf[1]==0x8D && wireless_buf[2]==0x01)
			{
				Tx_power=wireless_buf[3];	
			}    
			//uart_transmit_buf(&COM0,wireless_buf2,20);
		}
	}
	return Tx_power;
}
/**********************************************************************************/
void lora_set_NSID(uint16_t set_Nsid)
{
//发送:3C 06 02 00 01 45 0D 
//接收:3C 86 03 4F 4B 00 5F 0D 
//写入:OK

  uint8_t lorasevebuf[10]={0};
 
//uint8_t cre_cebuf[10]={0};

	uint8_t csnum=0; 
	lorasevebuf[0]=0x3C;
	lorasevebuf[1]=0x06;
	lorasevebuf[2]=0x02;
	lorasevebuf[3]=set_Nsid>>8;
	lorasevebuf[4]=set_Nsid;
	csnum=cs_Check(lorasevebuf,4);
	lorasevebuf[5]=csnum;
	lorasevebuf[6]=0x0D;

	for(uint8_t i = 0;i<3;i++)
	{
		lora_ext();	
 //   memset(wireless_buf2,0,256);		
		if(!uart_send_buff_ack(&GPRS, lorasevebuf, 7,wireless_buf,recive_len, "OK",  500))
		{
			uart_transmit_buf(&COM0,wireless_buf,20);
			break;
		}
	}
}
/**********************************************************************************/
void lora_set_BAND(uint32_t set_Band ,uint8_t set_shudu)
{
//发送:3C 02 09 55 AA 07 19 CF 0E 40 3C 81 40 0D 
//接收:3C 82 03 4F 4B 00 5B 0D 

	uint8_t csnum=0;
	uint8_t lorasevebuf[20]={0};

	uint16_t cre_ce;

	lorasevebuf[0]=0x55;
	lorasevebuf[1]=0xAA;
	lorasevebuf[2]=set_shudu;
	lorasevebuf[3]=set_Band>>24;
	lorasevebuf[4]=set_Band>>16;
	lorasevebuf[5]=set_Band>>8;
	lorasevebuf[6]=set_Band;
	
	cre_ce=crc_16(lorasevebuf,7);
	
	lorasevebuf[0]=0x3C;
	lorasevebuf[1]=0x02;
	lorasevebuf[2]=0x09;
	lorasevebuf[3]=0x55;
	lorasevebuf[4]=0xAA;
	lorasevebuf[5]=set_shudu;
	lorasevebuf[6]=set_Band>>24;
	lorasevebuf[7]=set_Band>>16;
	lorasevebuf[8]=set_Band>>8;
	lorasevebuf[9]=set_Band;
	lorasevebuf[11]=cre_ce>>8;
	lorasevebuf[10]=cre_ce;
	csnum=cs_Check(lorasevebuf,11);
	
	lorasevebuf[12]=csnum;
	lorasevebuf[13]=0x0D;	
//	lora_ext();
	for(uint8_t i = 0;i<3;i++)
	{
		lora_ext();
		if(!uart_send_buff_ack(&GPRS, lorasevebuf, 14,wireless_buf, recive_len,  "OK",255))
		{
			 break;
		}
	}
}
/**********************************************************************************/
void lora_set_Txpower(uint8_t Tx_Power)
{
//发送:3C 0C 01 14 5D 0D 
//接收:3C 8C 03 4F 4B 00 65 0D 
//写入:OK
	uint8_t csnum=0;
	uint8_t lorasevebuf[10]={0};

	lorasevebuf[0]=0x3C;
	lorasevebuf[1]=0x0C;
	lorasevebuf[2]=0x01;
	lorasevebuf[3]=Tx_Power;
	csnum=cs_Check(lorasevebuf,3);
	lorasevebuf[4]=csnum;
	lorasevebuf[5]=0x0D;	
//	lora_ext();
	for(uint8_t i = 0;i<3;i++)
	{
		lora_ext();
		if(!uart_send_buff_ack(&GPRS, lorasevebuf,6, wireless_buf,recive_len, "OK",  255))
		{
       break;
		}
	}
}
/**********************************************************************************/
uint8_t lora_get_base(void)
{	
	uint8_t lorasevebuf[10]={0};
	uint32_t  tobuf16[4]={0};
	lorasevebuf[0]=0x3C;
	lorasevebuf[1]=0x05;
	lorasevebuf[2]=0x00;
	lorasevebuf[3]=0x41;
	lorasevebuf[4]=0x0D;
  lora_ext();
  for(uint8_t i = 0;i<3;i++)
	{
		if( !uart_send_buff_ack(&GPRS, lorasevebuf,5, wireless_buf,recive_len,  "<", 255))
		{	
			if(wireless_buf[1]==0x85 && wireless_buf[2]==0x12)
			{	
				lora_buf.lora_once=wireless_buf[5];
				lora_buf.lora_retry=wireless_buf[6];
				
				lora_buf.lora_updata=wireless_buf[7];
				lora_buf.lora_upack=wireless_buf[8];
				
				lora_buf.lora_num=wireless_buf[9];	
				lora_buf.lora_num=lora_buf.lora_num<<8|wireless_buf[10];	
			
				lora_buf.lora_slot=wireless_buf[11];
				lora_buf.lora_slot=lora_buf.lora_slot<<8|wireless_buf[12];
				

				tobuf16[0]=wireless_buf[13];
				tobuf16[1]=wireless_buf[14];
				tobuf16[2]=wireless_buf[15];
				tobuf16[3]=wireless_buf[16];
				lora_buf.lora_time=tobuf16[0]<<24|tobuf16[1]<<16|tobuf16[2]<<8|tobuf16[3];
				
				tobuf16[0]=wireless_buf[17];
				tobuf16[1]=wireless_buf[18];
				tobuf16[2]=wireless_buf[19];
				tobuf16[3]=wireless_buf[20];
				lora_buf.lora_pwrtime=tobuf16[0]<<24|tobuf16[1]<<16|tobuf16[2]<<8|tobuf16[3];
			}    
			return  1;		
		}
  }
	return 0;
}

/**********************************************************************************/
int16_t lora_get_signal(void)
{
	int16_t signalbuf[4];
	int16_t lora_Signal=0; 
	uint8_t lorasevebuf[8]={0};
//	char buf[10];

	lorasevebuf[0]=0x3C;
	lorasevebuf[1]=0x16;
	lorasevebuf[2]=0x00;
	lorasevebuf[3]=0x52;
	lorasevebuf[4]=0x0D;

	lora_ext();
  for(uint8_t i = 0;i<3;i++)
	{
		if( !uart_send_buff_ack(&GPRS, lorasevebuf,5, wireless_buf, recive_len, "<", 255))
		{
			if(wireless_buf[1]==0x96 && wireless_buf[2]==0x02)
			{
				signalbuf[0]=wireless_buf[3];	
				signalbuf[1]=wireless_buf[4];	
				lora_Signal=	signalbuf[0]<<8|signalbuf[1];
			if(lora_Signal!=0)
			lora_Signal=	lora_Signal+200;
			}    
		}
	}
	if(lora_Signal == 0) 
		{
			Trace_log("Signal is err\r\n");
		}
	return  lora_Signal;
}

/**********************************************************************************/
uint16_t lora_read(void)
{
//	lora_ext();
    lora_buf.LoRa_NETid_set=lora_get_NSID();							
		sprintf((char *)device_info.save.srv_addr,"%d",	lora_buf.LoRa_NETid_set);
	
		lora_buf.LoRa_Band_set =lora_get_BAND(&lora_buf.LoRa_sudu_set);
		if(lora_buf.LoRa_Band_set !=0)
		lora_buf.LoRa_Band_set= lora_buf.LoRa_Band_set/1000000;//mhz		
		sprintf((char *)device_info.save.srv_login,"%d",lora_buf.LoRa_Band_set);		
		sprintf((char *)device_info.save.srv_Password,"%d",lora_buf.LoRa_sudu_set);
		
		lora_buf.LoRa_Txpower_set=lora_get_Txpower();
		sprintf((char *)device_info.save.srv_port,"%d",	lora_buf.LoRa_Txpower_set);
		lora_get_base();                                  //  get lora the base parameters
		nb.nb_csq=lora_get_signal();//+200                //  signal  +200   =  new_value  and this may have bug ,it is get signal=0,its err ,it is me?or lora ?					
//		lora_buf.LoRa_NETid_set =	lora_buf.LoRa_NETid;
//		lora_buf.LoRa_Band_set = lora_buf.LoRa_Band;				
//		lora_buf.LoRa_sudu_set =lora_buf.LoRa_sudu;
//		lora_buf.LoRa_Txpower_set =lora_buf.LoRa_Txpower;
		return 0;
}
/**********************************************************************************/

