#include "main.h"
#include "device.h"

char *Version = "TP280xVx.0_220606_ZY";
device_t device_info;
/********************CRC********************************/
static const uint8_t s_CRCH[] = 
{
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,  
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,  
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,  
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,  
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,  
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,  
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,  
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,  
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,  
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,  
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,  
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,  
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,  
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,  
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,  
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,  
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,  
0x40  
};

/**********************CRC******************************/
static uint8_t s_CRCLo[] = 
{
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,  
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,  
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,  
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,  
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,  
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,  
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,  
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,  
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,  
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,  
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,  
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,  
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,  
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,  
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,  
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,  
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,  
0x40
};



/********************CRC********************************/
uint16_t crc_16(uint8_t *p_buf, uint16_t length)   //返回的数据高低位反了   查表法
{
  uint8_t crc_h = 0XFF, crc_l = 0XFF;
  int index;
  
  while(length--)
  {
    index = crc_h ^ *(p_buf++);  /*计算CRC*/
    crc_h = (uint8_t)(crc_l ^ s_CRCH[index]);
    crc_l = s_CRCLo[index];
  }
  return ((uint16_t)crc_h << 8 | crc_l);
}
/*uint16_t crc_16(uint8_t *p_buf, uint16_t length)    //计算法
{
  uint16_t wCrc = 0xffff;
	uint8_t i,j;
	for(i=0; i<length; i++)
	{
		wCrc ^= (unsigned int)p_buf[i];
		for(j=0; j<8; j++)
		{
			if(wCrc & 1)
			{
				wCrc >>= 1; 
				wCrc ^= 0xA001; 
			}
			else
			{
				wCrc >>= 1; 
			}
		}
	}
	return wCrc;
}
*/
/****************************************************/
/*****************清除flash数据***********************************/
void device_clear_hist(void)
{
	w25qxx_erase_chip();
	
	device_info.hist_num = 0;
  	device_info.save_addr = 0;
	device_info.pend_num = 0;
	device_info.pend_addr = 0;
	device_set_flahaddr_addr(0);
	device_pending_num_set();
	device_perv_addr_set();
}
/****************************************************/
/* 设备应答 带上crc */  
//
char tx_buf[150];
static void device_ack_crc(void *front, void *back)
{
  uint8_t tx_size = 0;
  uint16_t crc;
	
  tx_size = strlen(front) + strlen(back) + 5; 
	
  memset(tx_buf, 0, 150);
  tx_buf[0] = 0XFF;
  tx_buf[1] = 0X03;
  tx_buf[2] = tx_size - 5;
  
  strcat(tx_buf, front);
  strcat(tx_buf, back);
  
  crc = crc_16((uint8_t *)tx_buf, tx_size - 2);
  tx_buf[tx_size - 2] = crc >> 8;
  tx_buf[tx_size - 1] = crc;
  
  uart_transmit_buf(&COM0, (uint8_t *)tx_buf, tx_size);   //DMA

}
/****************************************************/
/* 写入缓存区 */
static uint8_t  device_set_strparam(void *params, uint8_t *set_buf, uint16_t params_size, uint16_t set_size)
{
	uint16_t write_size = set_size;
	if(params_size-1 <= set_size)   /*防止写入溢出*/
	 write_size = params_size-1;	
	
	if(strcmp(params,(char *)set_buf) == 0 )
	{
		return 0;
	}
	memset(params, 0, params_size);

	strncpy(params, (char *)set_buf, write_size);
	return 2;
}

/****************************************************/
void device_param_write(save_param_t *p_pram)
{  	
	device_info.save.device_crc= crc_16((uint8_t *)&device_info.save,  sizeof(save_param_t)- 2); 
	device_info.save.save_len = sizeof(save_param_t);
	/* 存储flash */
	flash_write(DEVICE_INFO, (uint32_t *)&device_info.save, sizeof(save_param_t)/4);	
	if(device_info.device_en==0)  //配置参数有修改
	{
		device_info.Params_sever = 1;
		device_info.device_en = 1;
	}
}
//****************************************************/
void device_param_default(save_param_t *p_pram)    //数据初始化
{
	uint8_t i;
	p_pram->first_data[0] = 0x0F;
	p_pram->first_data[1] = 0xF0;
	strcpy( (char *)p_pram->srv_addr,"117.60.157.137");                  //开放 "47.106.59.102");//4G//1883.dtuip.com//ct电信 117.60.157.137
	strcpy( (char *)p_pram->srv_login, "1234567890");
	strcpy( (char *)p_pram->srv_UserName, "MQTT");
	strcpy( (char *)p_pram->srv_Password, "MQTTPW");
	strcpy( (char *)p_pram->srv_Topic, "0");
	strcpy( (char *)p_pram->srv_Subacription, "0/+");

	strcpy( (char *)p_pram->srv_port, "5683");  ///1883
	
	strcpy( (char *)p_pram->srv_apn, "");
	strcpy( (char *)p_pram->srv_apn_user_name, "");
	strcpy( (char *)p_pram->srv_apn_user_pass, "");
	
	strcpy( (char *)p_pram->srv_sim_pin, "");
	strcpy( (char *)p_pram->srv_sim_puk, "");
	
	p_pram->channel_up[0] = 80;     //电压通道初始化
	p_pram->channel_down[0] = 20;
	p_pram->channel_f[0] = 10;
	p_pram->channel_hc[0] = 2;
	p_pram->channel_c[0] = 0;
	p_pram->channel_range_l[0] = 0;
	p_pram->channel_range_h[0] = 100;
	p_pram->channel_unit_p[0] = UNIT_0;
	p_pram->channel_unit[0] = UNIT_0;
		
	for(i=1;i<4;i++)     //测温通道初始化
	{
		p_pram->channel_up[i] = 600;
		p_pram->channel_down[i] = -200;
		p_pram->channel_f[i] = 100;
		p_pram->channel_hc[i] = 20;
		p_pram->channel_c[i] = 0;
		p_pram->channel_range_l[i] = -200;
		p_pram->channel_range_h[i] = 600;
		p_pram->channel_unit_p[i] = UNIT_C;
		p_pram->channel_unit[i] = UNIT_C;
	}
	
	for(i=4;i<16;i++)     //iic通道初始化
	{
		p_pram->channel_up[i] = 1600;
		p_pram->channel_down[i] = 0;
		p_pram->channel_f[i] = 1600;
		p_pram->channel_hc[i] = 20;
		p_pram->channel_c[i] = 0;
		p_pram->channel_range_l[i] = 0;
		p_pram->channel_range_h[i] = 1600;
		p_pram->channel_unit_p[i] = UNIT_KPa;
		p_pram->channel_unit[i] = UNIT_MPa;
		
		p_pram->channel_up[i+1] = 70;
		p_pram->channel_down[i+1] = -40;
		p_pram->channel_f[i+1] = 10;
		p_pram->channel_hc[i+1] = 2;
		p_pram->channel_c[i+1] = 0;
		p_pram->channel_range_l[i+1] = -40;
		p_pram->channel_range_h[i+1] = 70;
		p_pram->channel_unit_p[i+1] = UNIT_C;
		p_pram->channel_unit[i+1] = UNIT_C;
		i++;
	}
	for(i=0;i<6;i++)
	{
		p_pram->port_sensor_type[i] = 0;
		p_pram->port_one_wire_type[i] = 0;
	}
	p_pram-> battery_cap = 17000;   //默认17000mAh容量电池
	p_pram-> srv_time = 60;        //上报时间默认60分钟
	p_pram-> BJ_pinlv_time = 60;        //上报时间默认60分钟
	
	p_pram-> device_operator = ATO_Search;	
	p_pram->zhengdian = OFF;
	p_pram->BJ_pinlv_stu = OFF;
	p_pram->port_num = 3;
	
	p_pram-> lora_id = 1 ;
	p_pram->sever_type = NB_tlink;
	
	p_pram->eliminate_signal = ON;
	p_pram->device_dabao = OFF;
	p_pram->tab_model = 0;
	p_pram-> SENSOR_SAM_TIME = 6;      //默认6*10=60秒
	
	p_pram->tab_module = 0;
	p_pram->tab_protocol = 0;
	p_pram-> modbus_addr = 1;
	p_pram->dip_num = 4;
	
	p_pram-> baud = 9600;
	p_pram-> parity = LL_UART_PARITY_NONE; 
	p_pram-> stop_bits = LL_UART_STOPBITS_1;
	p_pram-> data_bits = LL_UART_DATAWIDTH_8B;
	 				 	 
	p_pram-> DZ_NUM = 0;	 
	p_pram-> sensorerr_num = 5;
	#if 0
	for(i=0;i<3;i++)
	{
		p_pram-> pt100_100[i] = 100.5;
		p_pram-> pt100_402[i] = 200.5;
	}
	#endif
}
//****************************************************/
void device_param_read(save_param_t *p_pram)    //数据读取
{
	uint16_t device_crcnew=0;
	uint16_t read_lan;
	uint8_t re_write = 0;
//	uint8_t i;
	
	memset(&device_info.save, 0, sizeof(save_param_t)); 
	read_lan = sizeof(save_param_t);
	flash_read(DEVICE_INFO, (uint32_t *)&device_info.save, read_lan/4);
	device_crcnew = crc_16((uint8_t *)&device_info.save,  read_lan- 2);   //-2  CRC标志位 uint16_t device_info.save.device_crc
	
	if(p_pram->first_data[0] != 0x0F && p_pram->first_data[1] != 0xF0)   //判断该FLASH存储数据是否为该程序数据
	{
		device_param_default(p_pram);
		re_write = 1;
	}
//	else if(read_lan == device_info.save.save_len && device_crcnew != device_info.save.device_crc)   //FLASH空间大小没有发生变化但是CRC不对  说明数据出现错误  需要复位参数
//	{
//		device_param_default(p_pram);
//		re_write = 1;
//	}
			
	if(p_pram->srv_time <= 0 || p_pram->srv_time > 1440)
	{
		p_pram-> srv_time = 60;
		re_write = 1;
	}
	
	if(p_pram->SENSOR_SAM_TIME < 1 || p_pram->SENSOR_SAM_TIME > 250)
	{
		p_pram-> SENSOR_SAM_TIME = 6; 
		re_write = 1;
	}
	
	if(p_pram->sensorerr_num>250)
	{
		p_pram->sensorerr_num = 5;
		re_write = 1;
	}

	if(re_write == 1)
	  device_param_write(p_pram);   //初始化后的数据重新写入flash
}
/****************************************************/

/*******************信号值读取*********************************/
static void device_get_QSignal(void)
{
	char buf[4];	
	sprintf(buf, "%d", nb.nb_csq);
	if( device_info.save.tab_module == TP307V1_1_A || device_info.save.tab_module == LORAWAN)
	{
		device_ack_crc("信号值Signal[70-190dbm]:", buf);
	}
	else
	{	
		device_ack_crc("信号值Signal[0-31dbm]:", buf);
	}
}
/*******************电池电量获取*********************************/
static void device_get_VBAT(void )
{
    char buf[11];
   if(device_info.channel_data[0] > 100)
		 device_info.channel_data[0] = 100;
	 else if(device_info.channel_data[0] < 0)
		 device_info.channel_data[0] = 0;
   sprintf(buf, "%0.2f", (device_info.channel_data[0]));
    device_ack_crc("电池电量VBAT[0-100%]:", buf);
}
/**********************连接服务器 读取******************************/
void device_get_sever_type(void)
{
	if(device_info.save.tab_protocol == NBIOT)
	{
		if(device_info.save.sever_type == NB_tlink)
		{
			device_ack_crc("服务器Sever_type(NB_tlink/NB_onenet/NB_telecom):","NB_tlink");
		}
		else if(device_info.save.sever_type == NB_onenet)
		{
			device_ack_crc("服务器Sever_type(NB_tlink/NB_onenet/NB_telecom):","NB_onenet");
		}
		else if(device_info.save.sever_type == NB_telecom)
		{
			device_ack_crc("服务器Sever_type(NB_tlink/NB_onenet/NB_telecom):","NB_telecom");
		}
		else
			device_ack_crc("服务器Sever_type(NB_tlink/NB_onenet/NB_telecom):","NB_tlink");
	}
	else if(device_info.save.tab_protocol == UDP)
	{
		if(device_info.save.sever_type == tlink_open)
			device_ack_crc("服务器Sever_type(OPEN/CLOSE):","OPEN");
		else if(device_info.save.sever_type == tlink_close)
			device_ack_crc("服务器Sever_type(OPEN/CLOSE):","CLOSE");
		else
		{
			device_info.save.sever_type = tlink_close;
			device_ack_crc("服务器Sever_type(OPEN/CLOSE):","CLOSE");
		}
	}
	else
		device_ack_crc("服务器Sever_type:","OPEN");
}
/*******************连接服务器设置*********************************/
void device_set_sever_type(uint8_t *params)   //NB 服务器
{
	if(strstr((char *)params, "NB_tlink"))
	{
		device_info.save.sever_type = NB_tlink;
	}
	else if(strstr((char *)params, "NB_onenet"))
	{
		device_info.save.sever_type = NB_onenet;
	}
	else if(strstr((char *)params, "NB_telecom"))
	{
		device_info.save.sever_type = NB_telecom;
	}
	else if(strstr((char *)params, "OPEN"))
	{
		device_info.save.sever_type = tlink_open;
	}
	else if(strstr((char *)params, "CLOSE"))
	{
		device_info.save.sever_type = tlink_close;
	}
  else
    device_info.save.sever_type = NB_tlink;		
}
/***************设备表头型号获取*************************************/
void device_get_model(void)
{
	switch(device_info.save.tab_model)
	{
		case TP280xVx_0: device_ack_crc("设备型号[model]:","TP280xVx.0");break;   //低功耗表头
		case TP280xVx_0_P: device_ack_crc("设备型号[model]:","TP280xVx.0-P");break;    //外供电表头
		default :device_ack_crc("设备型号[model](TP280xVx.0/TP280xVx.0-P):","0"); break;
	}
}
/****************设备名称设置************************************/
void device_set_model(uint8_t *params)    //版本号----传感器不同，存储不同
{
	uint8_t *tempbuf = params;
	tempbuf += 7;
	if(strstr((char *)tempbuf, "TP280xVx.0"))     //
	{
		device_info.save.tab_model = TP280xVx_0;
	}
	else if(strstr((char *)tempbuf, "TP280xVx.0-P"))       //   
	{
		device_info.save.tab_model = TP280xVx_0_P;
	}
  else if(strstr((char *)tempbuf, "0"))       //  
	{
		device_info.save.tab_model = 0;
	}		 
}
/*******************单位设置*********************************/
void device_set_channel_unit(uint8_t *params,uint8_t *senss)  //设置显示单位
{
	if(strstr((char *)params, "MPa"))
		*senss = UNIT_MPa;
	else if(strstr((char *)params, "KPa"))
		*senss = UNIT_KPa;
	else if(strstr((char *)params, "℃"))
	  *senss =UNIT_C;
	else if(strstr((char *)params, "F"))
	  *senss =UNIT_F;
	else
		*senss = UNIT_0;
}
/******************单位获取1**********************************/
void device_get_channel_unit(uint8_t ch)   //读取单位
{
	char unit1[5];
	char unit2[40];
	switch(device_info.save.channel_unit[ch])
	{
		case UNIT_MPa:		strcpy(unit1,"MPa");break ;
		case UNIT_KPa:		strcpy(unit1,"KPa");break ;		
		case UNIT_C:      strcpy(unit1,"℃");break ;
		case UNIT_F:      strcpy(unit1,"F");break ;
//		case UNIT_0:      strcpy(unit,"0");break ;
	  default : strcpy(unit1,"0");break ;
	}
	switch(ch)
	{
		case 1: 
		case 2:
		case 3: 
		case 5:
		case 7:
		case 9: 
		case 11:
		case 13: 
		case 15: sprintf(unit2,"通道%d量程单位[CH%d unit](℃/F/0):",ch,ch);break;
		case 4:
		case 6:
		case 8:
		case 10: 
		case 12:
		case 14: sprintf(unit2,"通道%d量程单位[CH%d unit](MPa/KPa/0):",ch,ch);break;
		default: sprintf(unit2,"NONE:");break;		
	}
	device_ack_crc(unit2,unit1);
}
/******************单位获取1**********************************/
/******************小信号开关获取**********************************/
void device_get_eliminate_signal(void) //小信号开关读取
{
	if(device_info.save.eliminate_signal == ON)
	{
		device_ack_crc("小信号消除Eliminate Small-Signal[<0.5%rdg.](on/off):","on");
	}
	else if(device_info.save.eliminate_signal == OFF)
	{
		device_ack_crc("小信号消除Eliminate Small-Signal[<0.5%rdg.](on/off):","off");
	}
	else 
	{ device_info.save.eliminate_signal = ON;
		device_ack_crc("小信号消除Eliminate Small-Signal[<0.5%rdg.](on/off):","on");
	}
}
/******************小信号开关设置**********************************/
void device_set_eliminate_signal(uint8_t *signal)  //小信号开关设置
{
	if(strstr((char *)signal, "on"))
	{
	device_info.save.eliminate_signal = ON;
	}
	else if(strstr((char *)signal, "off"))
	{
	 device_info.save.eliminate_signal = OFF;
	}
}
/****************************************************/
/****************************************************/
/****************************************************/
void device_get_lora_once(void)
{
	  char buf[8];
    sprintf(buf, "%d", lora_buf.lora_once);
    device_ack_crc("单次上报字节:", buf);
}
/****************************************************/
void device_get_lora_retry(void)
{	  char buf[8];
    sprintf(buf, "%d", lora_buf.lora_retry);
    device_ack_crc("重发次数:", buf);
}	
/****************************************************/
void device_get_lora_updata(void)
{	  char buf[8];
    sprintf(buf, "%d", lora_buf.lora_updata);
    device_ack_crc("唤醒数据的长度:", buf);
}	
/****************************************************/
void device_get_lora_upack(void)
{	  char buf[8];
    sprintf(buf, "%d", lora_buf.lora_upack);
    device_ack_crc("唤醒应答数据长度:", buf);
}	
/****************************************************/
void device_get_lora_num(void)
{	  char buf[8];
    sprintf(buf, "%d", lora_buf.lora_num);
    device_ack_crc("终端节点数:", buf);
}	
/****************************************************/
void device_get_lora_slot(void)
{  char buf[8];
    sprintf(buf, "%d", lora_buf.lora_slot);
    device_ack_crc("上报SLOT长度:", buf);
}
/****************************************************/
void device_get_lora_time(void)
{  char buf[8];
    sprintf(buf, "%d", lora_buf.lora_time);
    device_ack_crc("上报周期[ms]:", buf);
}
/****************************************************/
void device_get_lora_pwrtime(void)
{
  	char buf[8];
    sprintf(buf, "%d", lora_buf.lora_pwrtime);
    device_ack_crc("唤醒间隔:", buf);
}
/****************************************************/
void device_set_lora_sudu(uint8_t *params)   
{
	
	if(strstr((char *)params, "距离远-低速"))
	{
		strcpy( (char *)device_info.save.srv_Password, "4");
	}
	else if(strstr((char *)params, "距离中-中速"))
	{	
		strcpy( (char *)device_info.save.srv_Password, "7");	
	}
	else if(strstr((char *)params, "距离近-高速"))
	{	
		strcpy( (char *)device_info.save.srv_Password, "10");
	}	
}
/****************************************************/
void device_get_lora_sudu(void)
{
  
	if(atoi((char *)device_info.save.srv_Password)  == 4 )
	{
		device_ack_crc("速率(距离远-低速/距离中-中速/距离近-高速):","距离远-低速");
	}
	else 	if(atoi((char *)device_info.save.srv_Password)  == 7 )
	{
		device_ack_crc("速率(距离远-低速/距离中-中速/距离近-高速):","距离中-中速");
	}	
	else 	if(atoi((char *)device_info.save.srv_Password)  == 10)
	{
		device_ack_crc("速率(距离远-低速/距离中-中速/距离近-高速):","距离近-高速");
	}
	else
	{	strcpy( (char *)device_info.save.srv_Password, "10");
  	device_ack_crc("速率(距离远-低速/距离中-中速/距离近-高速):","距离近-高速");
	}
}
/****************************************************/
void device_get_modbus_addr(void)    //485地址
{
	char mb_addr[8];
  sprintf(mb_addr, "%d",device_info.save.modbus_addr);
	device_ack_crc("modbus addrs:", mb_addr);
}
/****************************************************/
void device_get_channel_data(uint8_t ch)    //数据2
{
	char unit[5],unit1[40];
	if(ch>3)
	{
		if(ch>=(device_info.save.port_num*2+4))
		{
			device_ack_crc("通道未启用","");
			return;
		}
	}
	switch(device_info.save.channel_unit[ch])
	{
		case UNIT_MPa:		strcpy(unit,"MPa");break ;
		case UNIT_KPa:		strcpy(unit,"KPa");break ;
		case UNIT_C:      	strcpy(unit,"℃");break ;
		case UNIT_F:      	strcpy(unit,"F");break ;
	  	default : break ;
	}
	sprintf(unit1,"通道%d数据[%s]:%.4f",ch,unit,device_info.channel_data[ch]);	
	device_ack_crc(unit1,"");
}
/****************************************************/
/****************************************************/
/****************************************************/
void device_get_sensor_timers(void)    //当前时间
{
	char mb_addr[40];
  sprintf(mb_addr, "20%02d/%02d/%02d  %02d.%02d.%02d",
	device_info.data_time.year,
	device_info.data_time.month,
	device_info.data_time.day,
	device_info.data_time.hour,
	device_info.data_time.min,
	device_info.data_time.sec
  );
	device_ack_crc("采样时间Sampling time:", mb_addr);
}
/****************************************************/
void device_set_modbus_addr(uint8_t  *addrs)  //设置modbus地址
{ 
	device_info.save.modbus_addr= atoi((char *)addrs);
}
/****************************************************/
void device_get_baud( void)   //485串口波特率
{
	switch(device_info.save.baud)
	{
		case 4800:		device_ack_crc("RS485串口波特率Baud(4800/9600/14400/19200/38400/56000/57600):","4800");break ;
		case 9600:		device_ack_crc("RS485串口波特率Baud(4800/9600/14400/19200/38400/56000/57600):","9600");break ;
		case 14400:	  device_ack_crc("RS485串口波特率Baud(4800/9600/14400/19200/38400/56000/57600):","14400");break ;
		case 19200:  	device_ack_crc("RS485串口波特率Baud(4800/9600/14400/19200/38400/56000/57600):","19200");break ;
		case 38400:  	device_ack_crc("RS485串口波特率Baud(4800/9600/14400/19200/38400/56000/57600):","38400");break ;
		case 56000:		device_ack_crc("RS485串口波特率BaudBaud(4800/9600/14400/19200/38400/56000/57600):","56000");break ;
		case 57600:   device_ack_crc("RS485串口波特率(4800/9600/14400/19200/38400/56000/57600):","57600");break ;
//		case 115200:  device_ack_crc("RS485串口波特率Baud(4800/9600/14400/19200/38400/56000/57600/115200):","115200");break ;
	  default :      device_ack_crc("RS485串口波特率Baud(4800/9600/14400/19200/38400/56000/57600):","9600");device_info.save.baud = 9600;break ;
	}
}
/****************************************************/
void device_get_parity(void)    //485串口有无校验 
{
  if(device_info.save.parity == LL_UART_PARITY_NONE)
	{	
		device_ack_crc("RS485串口校验Check(无校验/奇校验/偶校验):","无校验");
	}
	else if (device_info.save.parity == LL_UART_PARITY_ODD)
	{
		device_ack_crc("RS485串口校验Check(无校验/奇校验/偶校验):","奇校验");
	}
	else  
	{
		device_ack_crc("RS485串口校验Check(无校验/奇校验/偶校验):","偶校验");
	}
}
/****************************************************/
void device_get_data_bits(void)  //485串口数据位 8/9
{  
	if(device_info.save.data_bits == LL_UART_DATAWIDTH_8B)
	{	
		 device_ack_crc("RS485数据位DATA(8/9):","8");
	}
	else 
	{
		if(device_info.save.data_bits != LL_UART_DATAWIDTH_9B)
		{
		  device_ack_crc("RS485数据位DATA(8/9):","9");
		}
		else 
		{
		  device_ack_crc("RS485数据位DATA(8/9):","8");
		}			
	}
}
/****************************************************/
void device_get_stop_bits(void)  //485串口停止位
{
  if(device_info.save.stop_bits==  LL_UART_STOPBITS_1)
	{
	   device_ack_crc("RS485停止位Stop(1/2):","1");
	}
	else if (device_info.save.stop_bits==  LL_UART_STOPBITS_2)
	{
	  device_ack_crc("RS485停止位Stop(1/2):","2");
	}
}
/****************************************************/
void device_set_baud( uint8_t *baud)  //波特率
{
	 device_info.save.baud =atoi((char *)baud);
	if(device_info.save.baud >65535)
		device_info.save.baud = 9600;
}
/****************************************************/
void device_set_parity(uint8_t *parity) //有无校验 
{
 	if(strstr((char *)parity, "无校验"))
	{
		device_info.save.parity = LL_UART_PARITY_NONE;

	}
	else 	if(strstr((char *)parity, "奇校验"))
	{
		device_info.save.parity = LL_UART_PARITY_ODD;
		device_info.save.data_bits = LL_UART_DATAWIDTH_9B;
	}
	else 	if(strstr((char *)parity, "偶校验"))
	{
		device_info.save.parity = LL_UART_PARITY_EVEN;
		device_info.save.data_bits = LL_UART_DATAWIDTH_9B;
	}		
}
/****************************************************/
void device_set_data_bits(uint8_t *data_bits) //数据位 8/9
{
  if(strstr((char *)data_bits, "8"))
	{
	  device_info.save.data_bits = LL_UART_DATAWIDTH_8B;
	}
	else if(strstr((char *)data_bits, "9"))
	{
	  device_info.save.data_bits = LL_UART_DATAWIDTH_9B;
	}
}
/****************************************************/
void device_set_stop_bits(uint8_t *stop_bits) //停止位
{
  if(strstr((char *)stop_bits, "1"))
	{
	  device_info.save.stop_bits = LL_UART_STOPBITS_1;
	}
	else if(strstr((char *)stop_bits, "2"))
	{
	  device_info.save.stop_bits = LL_UART_STOPBITS_2;
	}
}
/****************************************************/
uint32_t system_get_baud(void)
{
	return device_info.save.baud;
}
/****************************************************/
uint32_t system_get_parity(void)
{
	return device_info.save.parity;
}
/****************************************************/
uint32_t system_get_data_bits(void)
{
	return device_info.save.data_bits;
}
/****************************************************/
uint32_t system_get_stop_bits(void)
{
	return device_info.save.stop_bits;
}
/****************************************************/
uint16_t sys_get_modbus_addr( void )
{
  return device_info.save.modbus_addr;
}
/****************************************************/
void device_get_dabao(void)  //获取打包状态
{
	if(device_info.save.device_dabao == ON)
    device_ack_crc("打包上传功能Package upload function(on/off):", "on");
	else if(device_info.save.device_dabao == OFF)
		device_ack_crc("打包上传功能Package upload function(on/off):", "off");
	else 
	{ 
		device_info.save.device_dabao = OFF;    //默认
	  device_ack_crc("打包上传功能Package upload function(on/off):", "off");
	}
}
/****************************************************/
void device_get_zhengdian(void)  //获取整点上传状态
{
	if(device_info.save.zhengdian == ON)
    device_ack_crc("整点上传功能Whole point upload function(on/off):", "on");
	else if(device_info.save.zhengdian == OFF)
		device_ack_crc("整点上传功能Whole point upload function(on/off):", "off");
	else 
	{ 
		device_info.save.zhengdian = OFF;    //默认
	  device_ack_crc("整点上传功能Whole point upload function(on/off):", "off");
	}
}
/****************************************************/
/****************************************************/
void device_set_dabao(uint8_t *dabao)    //设置打包状态
{
	if(strstr((char *)dabao, "on"))
	{
		device_info.save.device_dabao = ON;
	}
	else if(strstr((char *)dabao, "off"))
	{
	  device_info.save.device_dabao = OFF;
	}
	else
		device_info.save.device_dabao = OFF;   //默认
}
/****************************************************/
void device_set_zhengdian(uint8_t *zhengdian)    //设置整点上传状态
{
	if(strstr((char *)zhengdian, "on"))
	{
		device_info.save.zhengdian = ON;
	}
	else if(strstr((char *)zhengdian, "off"))
	{
	  device_info.save.zhengdian = OFF;
	}
	else
		device_info.save.zhengdian = OFF;   //默认
}
/****************************************************/
/****************************************************/
void device_get_BJ_pinlv_stu(void)  //获取报警触发上报频率调整开启状态
{
	if(device_info.save.BJ_pinlv_stu == ON)
    device_ack_crc("报警触发上报频率调整Alarm repor statue(on/off):", "on");
	else if(device_info.save.BJ_pinlv_stu == OFF)
		device_ack_crc("报警触发上报频率调整Alarm repor statue(on/off):", "off");
	else 
	{ 
		device_info.save.BJ_pinlv_stu = OFF;    //默认
	  device_ack_crc("报警触发上报频率调整Alarm repor statue(on/off):", "off");
	}
}
/****************************************************/
void device_set_BJ_pinlv_stu(uint8_t *alarm)    //设置报警触发上报频率调整状态
{
	if(strstr((char *)alarm, "on"))
	{
		device_info.save.BJ_pinlv_stu = ON;
	}
	else if(strstr((char *)alarm, "off"))
	{
		device_info.save.BJ_pinlv_stu = OFF;
	}
	else
		device_info.save.BJ_pinlv_stu = OFF;   //默认
}
/****************************************************/
void device_get_BJ_pinlv_time(void)  //获取报警触发上报频率时间间隔
{
	char alarm[8];
	if(device_info.save.BJ_pinlv_time<1 || device_info.save.BJ_pinlv_time>1440)
		device_info.save.BJ_pinlv_time = 5;
	sprintf(alarm,"%d",device_info.save.BJ_pinlv_time);
	device_ack_crc("报警触发上报间隔Alarm repor time[1-1440分钟min]:", alarm);
}
/****************************************************/
void device_set_BJ_pinlv_time(uint8_t *alarm,uint8_t p_size)    //设置报警触发上报频率时间间隔
{
	char SAM[5];
  device_set_strparam(SAM,alarm,sizeof(SAM),p_size);	   
	device_info.save.BJ_pinlv_time = atoi(SAM);
	if(device_info.save.BJ_pinlv_time<1 || device_info.save.BJ_pinlv_time>1440)
		device_info.save.BJ_pinlv_time = 5;
}
/****************************************************/
void device_get_restart_num(void)    //获取重启次数
{
	char buf[10];
  sprintf(buf, "%d",nb.device_restart_num);
	device_ack_crc("设备重启次数Device restart times:", buf);
}
/****************************************************/
static void device_get_cesq(void)    //读取信号质量
{
	char buf[4];
  sprintf(buf, "%d",nb.nb_cesq);
	device_ack_crc("信号质量Signal quality[0-63]:", buf);
}
/****************************************************/
/****************************************************/
static void get_sensor_one_wire_type(char *buf,uint8_t port)
{
	switch(device_info.save.port_one_wire_type[port])
	{
		case TP2305V2: device_ack_crc(buf,"TP2305V2");break;
		case TP2302V2: device_ack_crc(buf,"TP2302V2");break;
		case TP2360: device_ack_crc(buf,"TP2360");break;
		case TP2305V3: device_ack_crc(buf,"TP2305V3");break;
		case TP2308: device_ack_crc(buf,"TP2308");break;
		case TP2303V1: device_ack_crc(buf,"TP2303V1");break;
		case TP2310: device_ack_crc(buf,"TP2310");break;
		case TP2311: device_ack_crc(buf,"TP2311");break;
		case TP2312: device_ack_crc(buf,"TP2312");break;
		case TP2313: device_ack_crc(buf,"TP2313");break;
		case TP2314: device_ack_crc(buf,"TP2314");break;
		case TP2315: device_ack_crc(buf,"TP2315");break;
		case TP2316: device_ack_crc(buf,"TP2316");break;
		case TP2317: device_ack_crc(buf,"TP2317");break;
		case TP2318: device_ack_crc(buf,"TP2318");break;
		case TP2319: device_ack_crc(buf,"TP2319");break;
		case TP2321_1: device_ack_crc(buf,"TP2321_1");break;
		case TP2321: device_ack_crc(buf,"TP2321");break;
		case TP2322: device_ack_crc(buf,"TP2322");break;
		default : device_ack_crc(buf, "One_wire");break;
	}
}
/****************************************************/
static void device_get_port_type(uint8_t port)    //传感器类型读取   设备有6个可变类型端口 0-5
{
	char buf[40];
	if(port>=device_info.save.port_num)
	{
		device_ack_crc("","");
		return;
	}
	sprintf(buf,"接口%d传感器类型",port+1);
	switch(device_info.save.port_sensor_type[port])
	{
		case One_wire: get_sensor_one_wire_type(buf,port);break;
		case D032: device_ack_crc(buf,":D032");break;
		case D033: device_ack_crc(buf,":D033");break;
		case D020: device_ack_crc(buf,":D020");break;
		case DS18B20: device_ack_crc(buf,":DS18B20");break;
		default : device_ack_crc(buf, "(One_wire/D032/D033/D020/DS18B20):0");break;
	} 
}
/****************************************************/
/****************************************************/
/********************************************************************************************************/
static void device_set_port_type(uint8_t *params,uint8_t port)    //传感器类型设置
{
	if(strstr((char *)params, "One_wire"))           //
	{
		device_info.save.port_sensor_type[port] = One_wire;
		device_info.save.channel_unit[port*2+4] = UNIT_0;
		device_info.save.channel_unit[port*2+5] = UNIT_0;
		device_info.save.channel_unit_p[port*2+4] = UNIT_0;
		device_info.save.channel_unit_p[port*2+5] = UNIT_0;
		device_info.save.channel_range_h[port*2+4] = 1600;
		device_info.save.channel_range_h[port*2+5] = 70;
		device_info.save.channel_range_l[port*2+4] = 0;
		device_info.save.channel_range_l[port*2+5] = -40;
	}
  else if(strstr((char *)params, "D032"))           //压力
	{
		device_info.save.port_sensor_type[port] = D032;
		device_info.save.channel_unit[port*2+4] = UNIT_MPa;
		device_info.save.channel_unit[port*2+5] = UNIT_C;
		device_info.save.channel_unit_p[port*2+4] = UNIT_KPa;
		device_info.save.channel_unit_p[port*2+5] = UNIT_C;
		device_info.save.channel_range_h[port*2+4] = 1600;
		device_info.save.channel_range_h[port*2+5] = 70;
		device_info.save.channel_range_l[port*2+4] = 0;
		device_info.save.channel_range_l[port*2+5] = -40;
	}
	else if(strstr((char *)params, "D033"))           //压力
	{
		device_info.save.port_sensor_type[port] = D033;
		device_info.save.channel_unit[port*2+4] = UNIT_MPa;
		device_info.save.channel_unit[port*2+5] = UNIT_C;
		device_info.save.channel_unit_p[port*2+4] = UNIT_KPa;
		device_info.save.channel_unit_p[port*2+5] = UNIT_C;
		device_info.save.channel_range_h[port*2+4] = 1600;
		device_info.save.channel_range_h[port*2+5] = 70;
		device_info.save.channel_range_l[port*2+4] = 0;
		device_info.save.channel_range_l[port*2+5] = -40;
	}
	else if(strstr((char *)params, "D020"))           //压力
	{
		device_info.save.port_sensor_type[port] = D020;	
		device_info.save.channel_unit[port*2+4] = UNIT_MPa;
		device_info.save.channel_unit[port*2+5] = UNIT_C;
		device_info.save.channel_unit_p[port*2+4] = UNIT_KPa;
		device_info.save.channel_unit_p[port*2+5] = UNIT_C;
		device_info.save.channel_range_h[port*2+4] = 40000;
		device_info.save.channel_range_h[port*2+5] = 70;
		device_info.save.channel_range_l[port*2+4] = 0;
		device_info.save.channel_range_l[port*2+5] = -40;
	}
	else if(strstr((char *)params, "0"))
		device_info.save.port_sensor_type[port] = 0;
}
/********************************************************************************************************/
void device_get_channel_range(uint8_t ch)    //传感器量程读取
{
	char unit[4],unit1[50];
	switch(device_info.save.channel_unit_p[ch])
	{
		case UNIT_MPa:		strcpy(unit,"MPa");break ;
		case UNIT_KPa:		strcpy(unit,"KPa");break ;
		case UNIT_C:      strcpy(unit,"℃");break ;
		case UNIT_F:      strcpy(unit,"F");break ;
		case UNIT_0:      strcpy(unit,"0");break ;
	  default : break ;
	}
	sprintf(unit1,"通道%d量程[%s]:%d~%d",ch,unit,device_info.save.channel_range_l[ch],device_info.save.channel_range_h[ch]);
	device_ack_crc(unit1,"");
}
/****************************************************/
void device_get_channel_range_l(uint8_t ch)    //传感器量程下限读取
{
	char unit[4],unit1[30];
	switch(device_info.save.channel_unit_p[ch])
	{
		case UNIT_MPa:		strcpy(unit,"MPa");break ;
		case UNIT_KPa:		strcpy(unit,"KPa");break ;
		case UNIT_C:      strcpy(unit,"℃");break ;
		case UNIT_F:      strcpy(unit,"F");break ;
		case UNIT_0:      strcpy(unit,"0");break ;
	  default : break ;
	}
	sprintf(unit1,"通道%d量程下限[%s]:%d",ch,unit,device_info.save.channel_range_l[ch]);
	device_ack_crc(unit1,"");
}
/****************************************************/
void device_get_channel_range_h(uint8_t ch)    //传感器量程上限读取
{
	char unit[4],unit1[30];
	switch(device_info.save.channel_unit_p[ch])
	{
		case UNIT_MPa:		strcpy(unit,"MPa");break ;
		case UNIT_KPa:		strcpy(unit,"KPa");break ;
		case UNIT_C:      strcpy(unit,"℃");break ;
		case UNIT_F:      strcpy(unit,"F");break ;
		case UNIT_0:      strcpy(unit,"0");break ;
	  default : break ;
	}
	sprintf(unit1,"通道%d量程上限[%s]:%d",ch,unit,device_info.save.channel_range_h[ch]);
	device_ack_crc(unit1,"");
}
/****************************************************/
/****************************************************/
/****************************************************/
void device_get_module(void)   //内置模块读取
{
	char get_module[15];	
	switch(device_info.save.tab_module)
	{
		case NULL1: strcpy(get_module,"NULL1");break;    //没有模块
		case BC26: strcpy(get_module,"BC26");break;  //NB模块
		case BC95: strcpy(get_module,"BC95");break;  //NB模块
		case BC20: strcpy(get_module,"BC20");break;  //NB模块
		case M101: strcpy(get_module,"M101");break;  //NB模块
		case EC25: strcpy(get_module,"EC25");break;
		case EC25_G: strcpy(get_module,"EC25_G");break;
		case TP307V1_1_A: strcpy(get_module,"TP307V1.1-A");break;
		case LORAWAN: strcpy(get_module,"LORAWAN");break;
		default : device_ack_crc("通讯模块module(NULL1/BC26/BC20/BC95/M101/EC25/EC25_G/TP307V1.1-A/LORAWAN):","0");return;
	}
	device_ack_crc("通讯模块module:",get_module);	
}
/****************************************************/
void device_set_module(uint8_t *tempbuf)    //内置模块设置
{
	uint8_t *params = tempbuf;
	params += 7;
  if(strstr((char *)params, "NULL1"))           //
	{
		device_info.save.tab_module = NULL1;
		device_info.save.tab_protocol = MODBUS;     //模块为空时  只支持MODBUS协议
	}
	else if(strstr((char *)params, "BC26"))      //
	{
		device_info.save.tab_module = BC26;
		device_info.save.tab_protocol = NBIOT;
		device_info.save.device_operator = CHN_Telecom;
	}
	else if(strstr((char *)params, "BC20"))    //
	{
		device_info.save.tab_module = BC20;
		device_info.save.tab_protocol = UDP;
		device_info.save.device_operator = ATO_Search;
		device_info.save.sever_type = tlink_close;
	}
	else if(strstr((char *)params, "BC95"))      //
	{
		device_info.save.tab_module = BC95;
		device_info.save.tab_protocol = NBIOT;
		device_info.save.device_operator = CHN_Telecom;
	}
	else if(strstr((char *)params, "M101"))      //
	{
		device_info.save.tab_module = M101;
		device_info.save.tab_protocol = NBIOT;
		device_info.save.device_operator = CHN_Telecom;
	}
	else if(strstr((char *)params, "EC25"))    //
	{
		device_info.save.tab_module = EC25;
		device_info.save.tab_protocol = MQTT;
		device_info.save.device_operator = ATO_Search;
	}
	else if(strstr((char *)params, "EC25_G"))    //
	{
		device_info.save.tab_module = EC25_G;
		device_info.save.tab_protocol = MQTT;
		device_info.save.device_operator = ATO_Search;
	}
	else if(strstr((char *)params, "TP307V1.1-A"))    //
	{
		device_info.save.tab_module = TP307V1_1_A;
		device_info.save.tab_protocol = LORA;     //模块为lora时  只支持LORA协议
	}
	else if(strstr((char *)params, "LORAWAN"))    //
	{
		device_info.save.tab_module = LORAWAN;
		device_info.save.tab_protocol = LORA_WAN;     //
	}
	else if(strstr((char *)params, "0"))
	{
		device_info.save.tab_module = 0;
		device_info.save.tab_protocol = 0;
	}
}
/****************************************************/
void device_get_protocol(void)    //通讯方式读取
{
	switch(device_info.save.tab_protocol)
	{
		case NBIOT: device_ack_crc("通讯方式protocol(NBIOT/UDP):","NBIOT");break;
		case MQTT: device_ack_crc("通讯方式protocol(MQTT/TCP):","MQTT");break;
		case TCP: device_ack_crc("通讯方式protocol(MQTT/TCP):","TCP");break;
		case LORA: device_ack_crc("通讯方式protocol:","LORA");break;
		case MODBUS: device_ack_crc("通讯方式protocol:","MODBUS");break;
		case UDP: device_ack_crc("通讯方式protocol(NBIOT/UDP):","UDP");break;
		case LORA_WAN: device_ack_crc("通讯方式protocol:","LORA_WAN");break;
		default : device_ack_crc("通讯方式protocol:","0");break;
	}
}
/****************************************************/
void device_set_protocol(uint8_t *params)    //通讯方式设置
{
  if(strstr((char *)params, "NBIOT"))           //
	{
		device_info.save.tab_protocol = NBIOT;
	}
	else if(strstr((char *)params, "MQTT"))      //
	{
		device_info.save.tab_protocol = MQTT;
	}
	else if(strstr((char *)params, "TCP"))    //
	{
		device_info.save.tab_protocol = TCP;
	}
	else if(strstr((char *)params, "MODBUS"))    //
	{
		device_info.save.tab_protocol = MODBUS;
	}
	else if(strstr((char *)params, "UDP"))    //
	{
		device_info.save.tab_protocol = UDP;
	}
}
/****************************************************/
void device_get_srv_timr(void)    //读取上报时间
{
	char SAM[5];
	sprintf(SAM,"%d",device_info.save.srv_time);
//	if(device_info.NOSLEEP_MODE == 1)
//	{
//		device_ack_crc("上报时间Upload time[1~21600 秒sec]:", SAM);
//	}
//	else
		device_ack_crc("上报时间Upload time[1~1440 分钟min]:", SAM);
	
}
/****************************************************/
void device_set_srv_time(uint8_t *params,uint8_t p_size)    //设置上报时间
{
	char SAM[5];
  device_set_strparam(SAM,params,sizeof(SAM),p_size);	   
	device_info.save.srv_time = atoi(SAM);
//	if(device_info.NOSLEEP_MODE == 1)
//	{
//		if(device_info.save.srv_time<1 || device_info.save.srv_time>21600) 
//			device_info.save.srv_time = 3600;
//	}
//	else
	  if(device_info.save.srv_time<1 || device_info.save.srv_time>1440) 
			device_info.save.srv_time = 60;	
}
/****************************************************/ 
void device_get_sam_time(void)    //读取采样时间   n*10秒
{
	char SAM[4];
	if( device_info.save.SENSOR_SAM_TIME>250 || device_info.save.SENSOR_SAM_TIME<1) 
		device_info.save.SENSOR_SAM_TIME = 6;
	sprintf(SAM, "%d",device_info.save.SENSOR_SAM_TIME);
	device_ack_crc("采样时间Sample time[1~250 n*10秒sec]:", SAM);	
}
/****************************************************/
void device_set_sam_time(uint8_t *params,uint8_t p_size)    //设置采样时间  n*10秒
{
	char SAM[4];
  	device_set_strparam(SAM,params,sizeof(SAM),p_size);	   
	device_info.save.SENSOR_SAM_TIME = atoi(SAM);

	if(device_info.save.SENSOR_SAM_TIME<1 || device_info.save.SENSOR_SAM_TIME>250) 
	{
		device_info.save.SENSOR_SAM_TIME = 6; 
	}		
}
/****************************************************/ 
void device_get_dip_num(void)    //读取小数点位数
{
	char SAM[2];
	if( device_info.save.dip_num>4) 
		device_info.save.dip_num = 4;
	sprintf(SAM, "%d",device_info.save.dip_num);
	device_ack_crc("显示小数点位数Display decimal places(0/1/2/3/4):", SAM);	
}
/****************************************************/
void device_set_dip_num(uint8_t *params,uint8_t p_size)    //设置显示小数点位数
{
	char SAM[4];
	uint8_t dip_da;
  device_set_strparam(SAM,params,sizeof(SAM),p_size);	   
	dip_da = atoi(SAM);

	if(dip_da>4) 
		device_info.save.dip_num = 4;
	else
	  device_info.save.dip_num = dip_da;
}
/****************************************************/
/****************************************************/
/****************************************************/
/****************************************************/
void device_get_DZ_NUM(void)         ////读取定制版本类型
{
	char SAM[4];
	sprintf((char *)SAM,"%d",device_info.save.DZ_NUM);
//	switch(device_info.save.DZ_NUM)
//	{
//		case 0:		strcpy(SAM,"0");break ;  //默认
//		case 1:		strcpy(SAM,"1");break ;
//		case 2:		strcpy(SAM,"2");break ;
//		case 3:		strcpy(SAM,"3");break ;
//		case 4:		strcpy(SAM,"4");break ;
//		case 5:		strcpy(SAM,"5");break ;
//		case 6:		strcpy(SAM,"6");break ;
//		case 7:		strcpy(SAM,"7");break ;
//		case 8:		strcpy(SAM,"8");break ;
//		
//		default :  strcpy(SAM,"0");device_info.save.DZ_NUM=0;break ;    //
//	}
  device_ack_crc("定制版本类型Custom version type:", SAM);
}
/****************************************************/
void device_set_DZ_NUM(uint8_t *params,uint8_t p_size)    //设置定制版本类型
{	
	char SAM[5];
  device_set_strparam(SAM,params,sizeof(SAM),p_size);	   
	device_info.save.DZ_NUM = atoi(SAM);
	
	DZ_init();
}
/****************************************************/
void device_get_channel_correct(uint8_t ch)    //sensor1数据修正
{
	uint8_t unit[30];
	sprintf((char *)unit,"通道%d修正值:%.4f",ch,device_info.save.channel_c[ch]);
	device_ack_crc(unit, "");
}
/****************************************************/
/****************************************************/
/****************************************************/
void device_get_channel_UP(uint8_t ch)     //读取通道上限
{
	uint8_t unit[30];
	sprintf((char *)unit,"通道%d上限:%.4f",ch,device_info.save.channel_up[ch]);
	device_ack_crc(unit, "");	
}
/****************************************************/
void device_get_channel_DOWN(uint8_t ch)    //读取通道下限
{
	uint8_t unit[30];
	sprintf((char *)unit,"通道%d下限:%.4f",ch,device_info.save.channel_down[ch]);
	device_ack_crc(unit, "");	
}
/****************************************************/
/****************************************************/
void device_get_channel_fluctuate(uint8_t ch)   //读取通道波动阈值
{
	uint8_t unit[40];
	sprintf((char *)unit,"通道%d波动阈值:%.4f",ch,device_info.save.channel_f[ch]);
	device_ack_crc(unit, "");	
}
/****************************************************/
/****************************************************/
void device_get_channel_hc(uint8_t ch)  //读取通道回差
{
	uint8_t unit[50];
	sprintf((char *)unit,"通道%d报警恢复差值:%.4f",ch,device_info.save.channel_hc[ch]);
	device_ack_crc(unit, "");
}
/****************************************************/
/****************************************************/
void device_get_operator(void)      //获取网络运营商信息
{
	switch(device_info.save.device_operator)
	{
		case CHN_Telecom: device_ack_crc("网络运营商[Operator](Telecom/Mobile/Unicom/AUTO):","Telecom");break;
		case CHN_Mobile: device_ack_crc("网络运营商[Operator](Telecom/Mobile/Unicom/AUTO):","Mobile");break;
		case CHN_Unicom: device_ack_crc("网络运营商[Operator](Telecom/Mobile/Unicom/AUTO):","Unicom");break;
		case ATO_Search: device_ack_crc("网络运营商[Operator](Telecom/Mobile/Unicom/AUTO):","AUTO");break;
	}
}
/****************************************************/
void device_set_operator(uint8_t *params)    //设置网络运营商信息
{
	if(strstr((char *)params, "Telecom"))
	{
		device_info.save.device_operator = CHN_Telecom;
	}
	else if(strstr((char *)params, "Mobile"))
	{
		device_info.save.device_operator = CHN_Mobile;
	}
	else if(strstr((char *)params, "Unicom"))
	{
		device_info.save.device_operator = CHN_Unicom;
	}
	else if(strstr((char *)params, "AUTO"))
	{
		device_info.save.device_operator = ATO_Search;
	}
  else
    device_info.save.device_operator = ATO_Search;
}
/****************************************************/
/****************************************************/
/****************************************************/
/****************************************************/
/****************************************************/
void device_vbat_reset(void)   //电池电量复位
{

		nb.Vbat_data = 0;	    //0x4684d000
		device_nbvbat_set(nb.Vbat_data);
		device_info.channel_data[0] = 100.0;

	nb.device_restart_num = 0;
	device_restart_num_set(nb.device_restart_num);
}
/****************************************************/
void device_vbat_reset_get(void)   //电池电量复位
{
	if(device_info.save.tab_model == TP280xVx_0)
	{
		device_ack_crc("复位电池电量Reset battery level","");
	}
	else
		device_ack_crc("NONE:","---");
}
/***************lorawan_satatus*************************************/
void device_get_lorawan_satatus(void)    //读取lorawan 网络ID
{
	char unit[30];
	switch(lorawan.STATUS)
	{
		case 0: strcpy(unit,"无操作");break;
		case 1: strcpy(unit,"正在发送数据");break;
		case 2: strcpy(unit,"发送失败");break;
		case 3: strcpy(unit,"发送成功");break;
		case 4: strcpy(unit,"入网成功");break;
		case 5: strcpy(unit,"入网失败");break;
		case 6: strcpy(unit,"网络异常");break;
		case 7: strcpy(unit,"无下行数据");break;
		case 8: strcpy(unit,"有下行数据");break;
		default: strcpy(unit,"---");break;
	}
	device_ack_crc("lorawan状态statue:",unit);
}
/*************lorawan_CHGROUP***************************************/
void device_get_lorawan_CHGROUP(void)
{
	char unit[4];
	sprintf(unit,"%d",lorawan.CHGROUP);
	device_ack_crc("信道组CH Group:",unit);
}
/***************hex_to_string*************************************/
void device_hex_to_string(uint8_t *hex,uint8_t *str,uint8_t hex_len,uint8_t str_len)
{
	uint8_t i,j,h,l;
	j = 0;
	if(hex_len-1>str_len)/*防止写入溢出*/
		hex_len = str_len;
	for(i=0;i<hex_len;i++)
	{
		if(hex[i]!='\0')
		{
			l = hex[i]&0x0f;
			h = hex[i]&0xf0;
			h >>= 4;
			if(h<10)
				str[j++] = 0x30+h;
			else
				str[j++] = 0x41+h-10;
			
			if(l<10)
				str[j++] = 0x30+l;
			else
				str[j++] = 0x41+l-10;
		}
		else
		{
			str[j++] = '\0';
			return;
		}
	}
	str[j++] = '\0';
}
/***************string_to_hex*************************************/
void device_string_to_hex(uint8_t *str,uint8_t *hex,uint8_t str_len,uint8_t hex_len)
{
	uint8_t i,j,h,l;
	j = 0;
	if(str_len <= hex_len)   /*防止写入溢出*/
	 str_len = hex_len;
	for(i=0;i<str_len;i++)
	{
		if(str[i] != '\0')
		{
			h = 0;
			l = 0;
			if(str[i]>='0' && str[i]<='9')
				h = str[i++]-'0';
			else if(str[i]>='a' && str[i]<='f')
				h = str[i++]-'a'+10;
			else if(str[i]>='A' && str[i]<='F')
				h = str[i++]-'A'+10;
			
			if(str[i]>='0' && str[i]<='9')
				l = str[i]-'0';
			else if(str[i]>='a' && str[i]<='f')
				l = str[i]-'a'+10;
			else if(str[i]>='A' && str[i]<='F')
				l = str[i]-'A'+10;
			hex[j++] = (h<<4)+l;
		}
	}
	hex[j] = '\0';
}
/**************lorawan_CHGROUP**************************************/
/*每8通道为一组
1:CH0~CH7
2:CH0~CH7
......
12:CH88~CH95
*/
void device_set_lorawan_CHGROUP(uint8_t *params,uint8_t p_size)
{
	char SAM[4];
  device_set_strparam(SAM,params,sizeof(SAM),p_size);	   
	lorawan.CHGROUP = atoi(SAM);
	if(lorawan.CHGROUP<1 || lorawan.CHGROUP>12) 
		lorawan.CHGROUP = 1;   //默认为1
}
/***************lorawan_ULDLMODE*************************************/
void device_get_lorawan_ULDLMODE(void)
{
	char unit[10];
	switch(lorawan.ULDLMODE)
	{
		case 0: strcpy(unit,"异频");break;
		case 1: strcpy(unit,"同频");break;
		default: break;
	}
	device_ack_crc("上下行同异频ULDL MODE(异频/同频):",unit);
}
/***************lorawan_ULDLMODE*************************************/
void device_set_lorawan_ULDLMODE(uint8_t *params)
{
	if(strstr((char *)params, "异频"))
	{
		lorawan.ULDLMODE = 0;
	}
	else if(strstr((char *)params, "同频"))
	{
		lorawan.ULDLMODE = 1;
	}
  else
    lorawan.ULDLMODE = 0;    //默认异频
}
/****************lorawan_JOINMODE************************************/
void device_get_lorawan_JOINMODE(void)
{
	char unit[10];
	switch(lorawan.JOINMODE)
	{
		case 0: strcpy(unit,"OTAA");break;
		case 1: strcpy(unit,"ABP");break;
		default: break;
	}
	device_ack_crc("激活模式Join Mode(OTAA/ABP):",unit);
}
/***************lorawan_JOINMODE*************************************/
void device_set_lorawan_JOINMODE(uint8_t *params)
{
	if(strstr((char *)params, "OTAA"))
	{
		lorawan.JOINMODE = 0;
	}
	else if(strstr((char *)params, "ABP"))
	{
		lorawan.JOINMODE = 1;
	}
  else
    lorawan.JOINMODE = 0;    //默认OTAA
}
/*****************lorawan_CLASS***********************************/
void device_get_lorawan_CLASS(void)
{
	char unit[10];
	switch(lorawan.CLASS)
	{
		case 0: strcpy(unit,"CLASS_A");break;
//		case 1: sprintf(unit,"CLASS_B");break;
		case 2: strcpy(unit,"CLASS_C");break;
		default: break;
	}
	device_ack_crc("网络类型Network type(CLASS_A/CLASS_C):",unit);
}
/***************lorawan_CLASS*************************************/
void device_set_lorawan_CLASS(uint8_t *params)
{
	if(strstr((char *)params, "CLASS_A"))
	{
		lorawan.CLASS = 0;
	}
//	else if(strstr((char *)params, "CLASS_B"))
//	{
//		lorawan.CLASS = 1;
//	}
	else if(strstr((char *)params, "CLASS_C"))
	{
		lorawan.CLASS = 2;
	}
  else
    lorawan.CLASS = 0;    //默认CLASS_A
}
/****************lorawan_BAND************************************/
void device_get_lorawan_BAND(void)
{
	char unit[10];
	switch(lorawan.BAND)
	{
		case 0: strcpy(unit,"EU433");break;
		case 1: strcpy(unit,"CN470");break;
		case 2: strcpy(unit,"EU868");break;
		case 3: strcpy(unit,"US915");break;
		case 5: strcpy(unit,"AU915");break;
		case 8: strcpy(unit,"IN865");break;
		default: break;
	}
	device_ack_crc("频段BAND(EU433/CN470/EU868/US915/AU915/IN865):",unit);
}
/***************lorawan_BAND*************************************/
void device_set_lorawan_BAND(uint8_t *params)
{
	if(strstr((char *)params, "EU433"))
	{
		lorawan.BAND = 0;
	}
	else if(strstr((char *)params, "CN470"))
	{
		lorawan.BAND = 1;
	}
	else if(strstr((char *)params, "EU868"))
	{
		lorawan.BAND = 2;
	}
	else if(strstr((char *)params, "US915"))
	{
		lorawan.BAND = 3;
	}
	else if(strstr((char *)params, "AU915"))
	{
		lorawan.BAND = 5;
	}
	else if(strstr((char *)params, "IN865"))
	{
		lorawan.BAND = 8;
	}
  else
    lorawan.BAND = 1;    //默认CN470
}
/***************lorawan_APPPORT*************************************/
void device_get_lorawan_APPPORT(void)
{
	char unit[10];
	sprintf(unit,"%d",lorawan.APPPORT);
	device_ack_crc("应用端口APP PORT(1~223):",unit);
}
/***************lorawan_APPPORT*************************************/
void device_set_lorawan_APPPORT(uint8_t *params,uint8_t p_size)
{
	char SAM[4];
  device_set_strparam(SAM,params,sizeof(SAM),p_size);	   
	lorawan.APPPORT = atoi(SAM);
	if(lorawan.APPPORT<1 || lorawan.APPPORT>223) 
		lorawan.APPPORT = 10;   //默认为10
}
/*******************lorawan_DATARATE*********************************/
void device_get_lorawan_DATARATE(void)
{
	char unit[10];
	sprintf(unit,"%d",lorawan.DATARATE);
	if(lorawan.ADR == 0)
		device_ack_crc("通讯速率DATA RATE(0~5):",unit);
	else
	  device_ack_crc("通讯速率DATA RATE:","自适应");
}
/********************lorawan_DATARATE********************************/
void device_set_lorawan_DATARATE(uint8_t *params,uint8_t p_size)
{
	char SAM[4];
  device_set_strparam(SAM,params,sizeof(SAM),p_size);
	lorawan.DATARATE = atoi(SAM);
	if(lorawan.DATARATE>5) 
		lorawan.DATARATE = 0;   //默认为0
}
/******************lorawan_TXPWR**********************************/
void device_get_lorawan_TXPWR(void)
{
	char unit[5];
	switch(lorawan.TXPWR)
	{
		case 0: strcpy(unit,"20");break;
		case 1: strcpy(unit,"18");break;
		case 2: strcpy(unit,"16");break;
		case 3: strcpy(unit,"14");break;
		case 4: strcpy(unit,"12");break;
		case 5: strcpy(unit,"10");break;
		case 6: strcpy(unit,"8");break;
		case 7: strcpy(unit,"6");break;
		default: sprintf(unit,"20");lorawan.TXPWR=0;break;
	}
	device_ack_crc("发射功率TX PWR(≥6):",unit);
}
/******************lorawan_TXPWR**********************************/
void device_set_lorawan_TXPWR(uint8_t *params,uint8_t p_size)
{
	char SAM[4];
  device_set_strparam(SAM,params,sizeof(SAM),p_size);
	lorawan.TXPWR = atoi(SAM);
	if(lorawan.TXPWR>5)
	{
		switch(lorawan.TXPWR)
		{
			case 6: lorawan.TXPWR = 7;break;
			case 7: lorawan.TXPWR = 7;break;
			case 8: lorawan.TXPWR = 6;break;
			case 9: lorawan.TXPWR = 6;break;
			case 10: lorawan.TXPWR = 5;break;
			case 11: lorawan.TXPWR = 5;break;
			case 12: lorawan.TXPWR = 4;break;
			case 13: lorawan.TXPWR = 4;break;
			case 14: lorawan.TXPWR = 3;break;
			case 15: lorawan.TXPWR = 3;break;
			case 16: lorawan.TXPWR = 2;break;
			case 17: lorawan.TXPWR = 2;break;
			case 18: lorawan.TXPWR = 1;break;
			case 19: lorawan.TXPWR = 1;break;
			case 20: lorawan.TXPWR = 0;break;
			default: break;
		}
	}
	else
		lorawan.TXPWR = 0;   //默认0 20dBM 
}
/****************lorawan_ADR************************************/
void device_get_lorawan_ADR(void)
{
	char unit[5];
	switch(lorawan.ADR)
	{
		case 0: strcpy(unit,"OFF");break;
		case 1: strcpy(unit,"ON");break;
		default: strcpy(unit,"ON");lorawan.ADR = 1;break;
	}
	device_ack_crc("速率自适应Rate Adaptive(ON/OFF):",unit);
}
/***************lorawan_ADR*************************************/
void device_set_lorawan_ADR(uint8_t *params)
{
	if(strstr((char *)params, "OFF"))
	{
		lorawan.ADR = 0;
	}
	else if(strstr((char *)params, "ON"))
	{
		lorawan.ADR = 1;
	}
  else
    lorawan.ADR = 1;    //默认开启
}
/****************lorawan_APPKEY************************************/
void device_get_lorawan_APPKEY(void)
{
	uint8_t unit[50];
	device_hex_to_string(device_info.save.srv_apn,unit,sizeof(device_info.save.srv_apn),50);
	device_ack_crc("应用根秘钥APP Key:",unit);
}
/****************lorawan_NWKSKEY************************************/
void device_get_lorawan_NWKSKEY(void)
{
	uint8_t unit[50];
	device_hex_to_string(device_info.save.srv_apn_user_name,unit,sizeof(device_info.save.srv_apn_user_name),50);
	device_ack_crc("网络会话秘钥NWKS Key:",unit);
}
/****************lorawan_APPSKEY************************************/
void device_get_lorawan_APPSKEY(void)
{
	uint8_t unit[50];
	device_hex_to_string(device_info.save.srv_apn_user_pass,unit,sizeof(device_info.save.srv_apn_user_pass),50);
	device_ack_crc("应用会话秘钥APPS Key:",unit);
}
/****************************************************/
static void device_get_battery_cap(void)    //
{
	char mb_addr[10];
	if(device_info.save.battery_cap <1000)
		device_info.save.battery_cap = 19000;
	sprintf(mb_addr, "%d", device_info.save.battery_cap);
	device_ack_crc("电池容量Battery capacity[mAh]:", mb_addr);
}
/****************************************************/
static void device_get_sensor_err_num(void)
{
	char mb_addr[10];
	if(device_info.save.sensorerr_num>250)
		device_info.save.sensorerr_num = 5;
	sprintf(mb_addr, "%d", device_info.save.sensorerr_num);
	device_ack_crc("传感器掉线延时Sensor drop delay[0-250]:", mb_addr);
}
/****************************************************/
static void device_get_port_num(void)
{
	char sam[4];
	if(device_info.save.port_num>6)
		device_info.save.port_num = 3;
	sprintf(sam,"%d",device_info.save.port_num);
	device_ack_crc("数字传感器开启通道数[1-6]:", sam);
}
/****************************************************/
static void get_mqtt_username(void)
{
	if(device_info.save.tab_protocol == MQTT)
		device_ack_crc("MQTT用户名User Name:",device_info.save.srv_UserName);
	else
		device_ack_crc("MQTT用户名User Name:","------");
}
/****************************************************/
static void get_mqtt_password(void)
{
	if(device_info.save.tab_protocol == MQTT)
		device_ack_crc("MQTT密码Password:",device_info.save.srv_Password);
	else
		device_ack_crc("MQTT密码Password:","------");
}
/****************************************************/
static void get_mqtt_topic(void)
{
	if(device_info.save.tab_protocol == MQTT)
		device_ack_crc("MQTT发布主题Topic:",device_info.save.srv_Topic);
	else
		device_ack_crc("MQTT发布主题Topic:","------");
}
/****************************************************/
static void get_mqtt_subscription(void)
{
	if(device_info.save.tab_protocol == MQTT)
		device_ack_crc("MQTT订阅主题Subscription:",device_info.save.srv_Subacription);
	else
		device_ack_crc("MQTT订阅主题Subscription:","------");
}
/****************************************************/
static void get_gps_status(void)
{
	if(device_info.save.tab_module == BC20 || device_info.save.tab_module == EC25_G)
	{
		if(device_info.save.GPS_stu == ON)
			device_ack_crc("GPS开启状态GPS Status(on/off):","on");
		else if(device_info.save.GPS_stu == OFF)
			device_ack_crc("GPS开启状态GPS Status(on/off):","off");
		else
		{
			device_info.save.GPS_stu = OFF;
			device_ack_crc("GPS开启状态GPS Status(on/off):","off");
		}
	}
	else
		device_ack_crc("GPS开启状态GPS Status:","------");
}
/****************************************************/
void set_gps_status(uint8_t *dabao)    //设置GPS状态
{
	if(strstr((char *)dabao, "on"))
	{
		device_info.save.GPS_stu = ON;
	}
	else if(strstr((char *)dabao, "off"))
	{
	  device_info.save.GPS_stu = OFF;
	}
	else
		device_info.save.GPS_stu = OFF;   //默认
}
/****************************************************/
static void get_gps_data_E(void)
{
	char buf[10];
	sprintf(buf,"%.4f",device_info.GPS_E);
	if(device_info.save.tab_module == BC20 || device_info.save.tab_module == EC25_G)
		device_ack_crc("GPS经度GPS_E:",buf);
	else
		device_ack_crc("GPS经度GPS_E:","------");
}
/****************************************************/
static void get_gps_data_N(void)
{
	char buf[10];
	sprintf(buf,"%.4f",device_info.GPS_N);
	if(device_info.save.tab_module == BC20 || device_info.save.tab_module == EC25_G)
		device_ack_crc("GPS纬度GPS_N:",buf);
	else
		device_ack_crc("GPS纬度GPS_N:","------");
}
/****************************************************/
static void device_set_next(uint8_t *next,uint8_t end);
/****************************************************/
void device_get_channel_clb_data(uint8_t clb,uint8_t ch)    //数据2
{
	char unit[5],unit1[40];
	if(clb == 0)
		sprintf(unit1,"通道%d零点数据校准值[Ω]:%.4f",ch,device_info.save.pt100_100[ch-1]);	
	else
		sprintf(unit1,"通道%d满点数据校准值[Ω]:%.4f",ch,device_info.save.pt100_402[ch-1]);	
	device_ack_crc(unit1,"");
}
/****************************************************/
static void device_get_new(uint8_t get_m_id)   //初始模式串口获取数据
{
	switch(get_m_id)
	{						
		case new_local_set: device_ack_crc("------初始化信息设置Initial Information Setting------","");break;			
		case new_DZ: device_get_DZ_NUM();break;	
		case new_battery:device_get_battery_cap();break;
		case new_sensor_err_num: device_get_sensor_err_num();break;								
		case new_PORT_NUM: device_get_port_num();break;
		case new_reset: device_ack_crc("恢复出厂设置Restore factory settings","");break;
		
		case new_CHANNEL_CLB: device_ack_crc("PT100通道校准[PT100 CLB]","");break;    //通道1校准
		case new_CHANNEL1_CLB0: device_get_channel_clb_data(0,1);break;    //通道1校准零点值
		case new_CHANNEL1_CLB1: device_get_channel_clb_data(1,1);break;   //通道1校准满点值
//		case new_CHANNEL2_CLB,    //通道2校准
		case new_CHANNEL2_CLB0: device_get_channel_clb_data(0,2);break;   //通道2校准零点值
		case new_CHANNEL2_CLB1: device_get_channel_clb_data(1,2);break;   //通道2校准满点值
//		case new_CHANNEL3_CLB,    //通道3校准
		case new_CHANNEL3_CLB0: device_get_channel_clb_data(0,3);break;   //通道3校准零点值
		case new_CHANNEL3_CLB1: device_get_channel_clb_data(1,3);break;   //通道3校准满点值
		
		case new_PORT0_TYPE: device_get_port_type(0);break;
		case new_CHANNEL4_UNIT_P: device_get_channel_unit(4);break;
		case new_CHANNEL4_RANGE_H: device_get_channel_range_h(4);break;
		case new_CHANNEL4_RANGE_L: device_get_channel_range_l(4);break;
		case new_CHANNEL5_UNIT_P: device_get_channel_unit(5);break;
		case new_CHANNEL5_RANGE_H: device_get_channel_range_h(5);break;
		case new_CHANNEL5_RANGE_L: device_get_channel_range_l(5);break;
		
		case new_PORT1_TYPE: device_get_port_type(1);break;
		case new_CHANNEL6_UNIT_P: device_get_channel_unit(6);break;
		case new_CHANNEL6_RANGE_H: device_get_channel_range_h(6);break;
		case new_CHANNEL6_RANGE_L: device_get_channel_range_l(6);break;
		case new_CHANNEL7_UNIT_P: device_get_channel_unit(7);break;
		case new_CHANNEL7_RANGE_H: device_get_channel_range_h(7);break;
		case new_CHANNEL7_RANGE_L: device_get_channel_range_l(7);break;

		case new_PORT2_TYPE: device_get_port_type(2);break;
		case new_CHANNEL8_UNIT_P: device_get_channel_unit(8);break;
		case new_CHANNEL8_RANGE_H: device_get_channel_range_h(8);break;
		case new_CHANNEL8_RANGE_L: device_get_channel_range_l(8);break;
		case new_CHANNEL9_UNIT_P: device_get_channel_unit(9);break;
		case new_CHANNEL9_RANGE_H: device_get_channel_range_h(9);break;
		case new_CHANNEL9_RANGE_L: device_get_channel_range_l(9);break;
		
		case new_PORT3_TYPE: device_get_port_type(3);break;
		case new_CHANNEL10_UNIT_P: device_get_channel_unit(10);break;
		case new_CHANNEL10_RANGE_H: device_get_channel_range_h(10);break;
		case new_CHANNEL10_RANGE_L: device_get_channel_range_l(10);break;
		case new_CHANNEL11_UNIT_P: device_get_channel_unit(11);break;
		case new_CHANNEL11_RANGE_H: device_get_channel_range_h(11);break;
		case new_CHANNEL11_RANGE_L: device_get_channel_range_l(11);break;
		
		case new_PORT4_TYPE: device_get_port_type(4);break;
		case new_CHANNEL12_UNIT_P: device_get_channel_unit(12);break;
		case new_CHANNEL12_RANGE_H: device_get_channel_range_h(12);break;
		case new_CHANNEL12_RANGE_L: device_get_channel_range_l(12);break;
		case new_CHANNEL13_UNIT_P: device_get_channel_unit(13);break;
		case new_CHANNEL13_RANGE_H: device_get_channel_range_h(13);break;
		case new_CHANNEL13_RANGE_L: device_get_channel_range_l(13);break;
		
		case new_PORT5_TYPE: device_get_port_type(5);break;
		case new_CHANNEL14_UNIT_P: device_get_channel_unit(14);break;
		case new_CHANNEL14_RANGE_H: device_get_channel_range_h(14);break;
		case new_CHANNEL14_RANGE_L: device_get_channel_range_l(14);break;
		case new_CHANNEL15_UNIT_P: device_get_channel_unit(15);break;
		case new_CHANNEL15_RANGE_H: device_get_channel_range_h(15);break;
		case new_CHANNEL15_RANGE_L: device_get_channel_range_l(15);break;
		
		default:{device_ack_crc("",""); }break;
	}
}
/****************************************************/
void device_set_new(uint8_t *set_m_id,uint8_t end)   //初始模式串口设置数据
{	
	uint8_t set = 0;
	uint8_t *temp_buf = set_m_id,param_size = temp_buf[6] - 1;
	temp_buf += 7;
	
	switch(set_m_id[3]-end)
	{	
		case new_reset:	device_param_default(&device_info.save);break;
		case new_DZ: device_set_DZ_NUM(temp_buf,param_size);break;	
		case new_battery: device_info.save.battery_cap = atoi((char *)temp_buf);break;
		case new_sensor_err_num: device_info.save.sensorerr_num = atoi((char *)temp_buf);break;				
		
		case new_CHANNEL_CLB: device_info.pt100_clb_sta = 1;break;
		
		case new_PORT_NUM: device_info.save.port_num = atoi((char *)temp_buf);break;
		case new_PORT0_TYPE: device_set_port_type(temp_buf,0);break;
		case new_CHANNEL4_UNIT_P: device_set_channel_unit(temp_buf,&device_info.save.channel_unit_p[4]);break;
		case new_CHANNEL4_RANGE_H: device_info.save.channel_range_h[4] = atoi((char *)temp_buf);break;
		case new_CHANNEL4_RANGE_L: device_info.save.channel_range_l[4] = atoi((char *)temp_buf);break;
		case new_CHANNEL5_UNIT_P: device_set_channel_unit(temp_buf,&device_info.save.channel_unit_p[5]);break;
		case new_CHANNEL5_RANGE_H: device_info.save.channel_range_h[5] = atoi((char *)temp_buf);break;
		case new_CHANNEL5_RANGE_L: device_info.save.channel_range_l[5] = atoi((char *)temp_buf);break;
		
		case new_PORT1_TYPE: device_set_port_type(temp_buf,1);break;
		case new_CHANNEL6_UNIT_P: device_set_channel_unit(temp_buf,&device_info.save.channel_unit_p[6]);break;
		case new_CHANNEL6_RANGE_H: device_info.save.channel_range_h[6] = atoi((char *)temp_buf);break;
		case new_CHANNEL6_RANGE_L: device_info.save.channel_range_l[6] = atoi((char *)temp_buf);break;
		case new_CHANNEL7_UNIT_P: device_set_channel_unit(temp_buf,&device_info.save.channel_unit_p[7]);break;
		case new_CHANNEL7_RANGE_H: device_info.save.channel_range_h[7] = atoi((char *)temp_buf);break;
		case new_CHANNEL7_RANGE_L: device_info.save.channel_range_l[7] = atoi((char *)temp_buf);break;
		
		case new_PORT2_TYPE: device_set_port_type(temp_buf,2);break;
		case new_CHANNEL8_UNIT_P: device_set_channel_unit(temp_buf,&device_info.save.channel_unit_p[8]);break;
		case new_CHANNEL8_RANGE_H: device_info.save.channel_range_h[8] = atoi((char *)temp_buf);break;
		case new_CHANNEL8_RANGE_L: device_info.save.channel_range_l[8] = atoi((char *)temp_buf);break;
		case new_CHANNEL9_UNIT_P: device_set_channel_unit(temp_buf,&device_info.save.channel_unit_p[9]);break;
		case new_CHANNEL9_RANGE_H: device_info.save.channel_range_h[9] = atoi((char *)temp_buf);break;
		case new_CHANNEL9_RANGE_L: device_info.save.channel_range_l[9] = atoi((char *)temp_buf);break;

		case new_PORT3_TYPE: device_set_port_type(temp_buf,3);break;
		case new_CHANNEL10_UNIT_P: device_set_channel_unit(temp_buf,&device_info.save.channel_unit_p[10]);break;
		case new_CHANNEL10_RANGE_H: device_info.save.channel_range_h[10] = atoi((char *)temp_buf);break;
		case new_CHANNEL10_RANGE_L: device_info.save.channel_range_l[10] = atoi((char *)temp_buf);break;
		case new_CHANNEL11_UNIT_P: device_set_channel_unit(temp_buf,&device_info.save.channel_unit_p[11]);break;
		case new_CHANNEL11_RANGE_H: device_info.save.channel_range_h[11] = atoi((char *)temp_buf);break;
		case new_CHANNEL11_RANGE_L: device_info.save.channel_range_l[11] = atoi((char *)temp_buf);break;
		
		case new_PORT4_TYPE: device_set_port_type(temp_buf,4);break;
		case new_CHANNEL12_UNIT_P: device_set_channel_unit(temp_buf,&device_info.save.channel_unit_p[12]);break;
		case new_CHANNEL12_RANGE_H: device_info.save.channel_range_h[12] = atoi((char *)temp_buf);break;
		case new_CHANNEL12_RANGE_L: device_info.save.channel_range_l[12] = atoi((char *)temp_buf);break;
		case new_CHANNEL13_UNIT_P: device_set_channel_unit(temp_buf,&device_info.save.channel_unit_p[13]);break;
		case new_CHANNEL13_RANGE_H: device_info.save.channel_range_h[13] = atoi((char *)temp_buf);break;
		case new_CHANNEL13_RANGE_L: device_info.save.channel_range_l[13] = atoi((char *)temp_buf);break;
		
		case new_PORT5_TYPE: device_set_port_type(temp_buf,5);break;
		case new_CHANNEL14_UNIT_P: device_set_channel_unit(temp_buf,&device_info.save.channel_unit_p[14]);break;
		case new_CHANNEL14_RANGE_H: device_info.save.channel_range_h[14] = atoi((char *)temp_buf);break;
		case new_CHANNEL14_RANGE_L: device_info.save.channel_range_l[14] = atoi((char *)temp_buf);break;
		case new_CHANNEL15_UNIT_P: device_set_channel_unit(temp_buf,&device_info.save.channel_unit_p[15]);break;
		case new_CHANNEL15_RANGE_H: device_info.save.channel_range_h[15] = atoi((char *)temp_buf);break;
		case new_CHANNEL15_RANGE_L: device_info.save.channel_range_l[15] = atoi((char *)temp_buf);break;

		default: set = *temp_buf - 0X30;break;														
	}
	if(set == 1)
	{
		device_info.debug_en = 0;
		device_param_get(set_m_id[3]);
		delay_vtask(1000);
		device_info.debug_en = 1;
	}
	else 
	{
		device_info.debug_en = 0;
		device_param_get(set_m_id[3]);
	}	
}
/****************************************************/
static void device_get_null(uint8_t get_m_id)   //初始模式串口获取数据
{
	switch(get_m_id)
	{						
		case NULL_CHANNEL_DATA: device_ack_crc("------设备基本参数读取 Read basic parameters------","");break;
		case NULL_TIME: device_get_sensor_timers() ;break;
		case NULL_CHANNEL0_DATA: device_get_VBAT();break;
		case NULL_CHANNEL1_DATA: device_get_channel_data(1);break;
		case NULL_CHANNEL2_DATA: device_get_channel_data(2);break;
		case NULL_CHANNEL3_DATA: device_get_channel_data(3);break;
		case NULL_CHANNEL4_DATA: device_get_channel_data(4);break;    //
		case NULL_CHANNEL5_DATA: device_get_channel_data(5);break;
		case NULL_CHANNEL6_DATA: device_get_channel_data(6);break;
		case NULL_CHANNEL7_DATA: device_get_channel_data(7);break;
		case NULL_CHANNEL8_DATA: device_get_channel_data(8);break;
		case NULL_CHANNEL9_DATA: device_get_channel_data(9);break;     //
		case NULL_CHANNEL10_DATA: device_get_channel_data(10);break;
		case NULL_CHANNEL11_DATA: device_get_channel_data(11);break;
		case NULL_CHANNEL12_DATA: device_get_channel_data(12);break;
		case NULL_CHANNEL13_DATA: device_get_channel_data(13);break;
		case NULL_CHANNEL14_DATA: device_get_channel_data(14);break;    //
		case NULL_CHANNEL15_DATA: device_get_channel_data(15);break;
		
		case NULL_CHANNEL_SET: device_ack_crc("------参数设置Channel setting------","");break;
		case NULL_SAM_TIME: device_get_sam_time();break;		
		case NULL_DIP_NUM: device_get_dip_num();break;		
		case NULL_eliminate_signal: device_get_eliminate_signal();break;
		case NULL_device_restart_num: device_get_restart_num();break;
		case NULL_vbat_reset: device_vbat_reset_get();break;
		
		case NULL_CHANNEL1_SENSOR_NAME:  device_ack_crc("通道1传感器类型:","PT100");break;    //通道传感器名称
		case NULL_CHANNEL1_UP: device_get_channel_UP(1);break;          //通道上限
		case NULL_CHANNEL1_DOWN: device_get_channel_DOWN(1);break;       //通道下限     //
		case NULL_CHANNEL1_F: device_get_channel_fluctuate(1);break;        //通道波动阈值
		case NULL_CHANNEL1_HC: device_get_channel_hc(1);break;        //通道报警回差
		case NULL_CHANNEL1_C: device_get_channel_correct(1);break;
		
		case NULL_CHANNEL2_SENSOR_NAME:  device_ack_crc("通道2传感器类型:","PT100");break;    //通道传感器名称
		case NULL_CHANNEL2_UP: device_get_channel_UP(2);break;          //通道上限
		case NULL_CHANNEL2_DOWN: device_get_channel_DOWN(2);break;       //通道下限     //
		case NULL_CHANNEL2_F: device_get_channel_fluctuate(2);break;        //通道波动阈值
		case NULL_CHANNEL2_HC: device_get_channel_hc(2);break;        //通道报警回差
		case NULL_CHANNEL2_C: device_get_channel_correct(2);break;
		
		case NULL_CHANNEL3_SENSOR_NAME:  device_ack_crc("通道3传感器类型:","PT100");break;    //通道传感器名称
		case NULL_CHANNEL3_UP: device_get_channel_UP(3);break;          //通道上限
		case NULL_CHANNEL3_DOWN: device_get_channel_DOWN(3);break;       //通道下限     //
		case NULL_CHANNEL3_F: device_get_channel_fluctuate(3);break;        //通道波动阈值
		case NULL_CHANNEL3_HC: device_get_channel_hc(3);break;        //通道报警回差
		case NULL_CHANNEL3_C: device_get_channel_correct(3);break;
		
		case NULL_PORT0_SENSOR_NAME: device_get_port_type(0);break;    //通道传感器名称
		case NULL_CHANNEL4_UP: device_get_channel_UP(4);break;          //通道上限
		case NULL_CHANNEL4_DOWN: device_get_channel_DOWN(4);break;       //通道下限     //20
		case NULL_CHANNEL4_F: device_get_channel_fluctuate(4);break;        //通道波动阈值
		case NULL_CHANNEL4_HC: device_get_channel_hc(4);break;        //通道报警回差
		case NULL_CHANNEL4_C: device_get_channel_correct(4);break;
		case NULL_CHANNEL5_UP: device_get_channel_UP(5);break;          //通道上限
		case NULL_CHANNEL5_DOWN: device_get_channel_DOWN(5);break;       //通道下限     //20
		case NULL_CHANNEL5_F: device_get_channel_fluctuate(5);break;        //通道波动阈值
		case NULL_CHANNEL5_HC: device_get_channel_hc(5);break;        //通道报警回差
		case NULL_CHANNEL5_C: device_get_channel_correct(5);break;
		
		case NULL_PORT1_SENSOR_NAME: device_get_port_type(1);break;    //通道传感器名称
		case NULL_CHANNEL6_UP: device_get_channel_UP(6);break;          //通道上限
		case NULL_CHANNEL6_DOWN: device_get_channel_DOWN(6);break;       //通道下限     //20
		case NULL_CHANNEL6_F: device_get_channel_fluctuate(6);break;        //通道波动阈值
		case NULL_CHANNEL6_HC: device_get_channel_hc(6);break;        //通道报警回差
		case NULL_CHANNEL6_C: device_get_channel_correct(6);break;
		case NULL_CHANNEL7_UP: device_get_channel_UP(7);break;          //通道上限
		case NULL_CHANNEL7_DOWN: device_get_channel_DOWN(7);break;       //通道下限     //20
		case NULL_CHANNEL7_F: device_get_channel_fluctuate(7);break;        //通道波动阈值
		case NULL_CHANNEL7_HC: device_get_channel_hc(7);break;        //通道报警回差
		case NULL_CHANNEL7_C: device_get_channel_correct(7);break;
		
		case NULL_PORT2_SENSOR_NAME: device_get_port_type(2);break;    //通道传感器名称
		case NULL_CHANNEL8_UP: device_get_channel_UP(8);break;          //通道上限
		case NULL_CHANNEL8_DOWN: device_get_channel_DOWN(8);break;       //通道下限     //20
		case NULL_CHANNEL8_F: device_get_channel_fluctuate(8);break;        //通道波动阈值
		case NULL_CHANNEL8_HC: device_get_channel_hc(8);break;        //通道报警回差
		case NULL_CHANNEL8_C: device_get_channel_correct(8);break;
		case NULL_CHANNEL9_UP: device_get_channel_UP(9);break;          //通道上限
		case NULL_CHANNEL9_DOWN: device_get_channel_DOWN(9);break;       //通道下限     //20
		case NULL_CHANNEL9_F: device_get_channel_fluctuate(9);break;        //通道波动阈值
		case NULL_CHANNEL9_HC: device_get_channel_hc(9);break;        //通道报警回差
		case NULL_CHANNEL9_C: device_get_channel_correct(9);break;
		
		case NULL_PORT3_SENSOR_NAME: device_get_port_type(3);break;    //通道传感器名称
		case NULL_CHANNEL10_UP: device_get_channel_UP(10);break;          //通道上限
		case NULL_CHANNEL10_DOWN: device_get_channel_DOWN(10);break;       //通道下限     //20
		case NULL_CHANNEL10_F: device_get_channel_fluctuate(10);break;        //通道波动阈值
		case NULL_CHANNEL10_HC: device_get_channel_hc(10);break;        //通道报警回差
		case NULL_CHANNEL10_C: device_get_channel_correct(10);break;
		case NULL_CHANNEL11_UP: device_get_channel_UP(11);break;          //通道上限
		case NULL_CHANNEL11_DOWN: device_get_channel_DOWN(11);break;       //通道下限     //20
		case NULL_CHANNEL11_F: device_get_channel_fluctuate(11);break;        //通道波动阈值
		case NULL_CHANNEL11_HC: device_get_channel_hc(11);break;        //通道报警回差
		case NULL_CHANNEL11_C: device_get_channel_correct(11);break;
		
		case NULL_PORT4_SENSOR_NAME: device_get_port_type(4);break;    //通道传感器名称
		case NULL_CHANNEL12_UP: device_get_channel_UP(12);break;          //通道上限
		case NULL_CHANNEL12_DOWN: device_get_channel_DOWN(12);break;       //通道下限     //20
		case NULL_CHANNEL12_F: device_get_channel_fluctuate(12);break;        //通道波动阈值
		case NULL_CHANNEL12_HC: device_get_channel_hc(12);break;        //通道报警回差
		case NULL_CHANNEL12_C: device_get_channel_correct(12);break;
		case NULL_CHANNEL13_UP: device_get_channel_UP(13);break;          //通道上限
		case NULL_CHANNEL13_DOWN: device_get_channel_DOWN(13);break;       //通道下限     //20
		case NULL_CHANNEL13_F: device_get_channel_fluctuate(13);break;        //通道波动阈值
		case NULL_CHANNEL13_HC: device_get_channel_hc(13);break;        //通道报警回差
		case NULL_CHANNEL13_C: device_get_channel_correct(13);break;
		
		case NULL_PORT5_SENSOR_NAME: device_get_port_type(5);break;    //通道传感器名称
		case NULL_CHANNEL14_UP: device_get_channel_UP(14);break;          //通道上限
		case NULL_CHANNEL14_DOWN: device_get_channel_DOWN(14);break;       //通道下限     //20
		case NULL_CHANNEL14_F: device_get_channel_fluctuate(14);break;        //通道波动阈值
		case NULL_CHANNEL14_HC: device_get_channel_hc(14);break;        //通道报警回差
		case NULL_CHANNEL14_C: device_get_channel_correct(14);break;
		case NULL_CHANNEL15_UP: device_get_channel_UP(15);break;          //通道上限
		case NULL_CHANNEL15_DOWN: device_get_channel_DOWN(15);break;       //通道下限     //20
		case NULL_CHANNEL15_F: device_get_channel_fluctuate(15);break;        //通道波动阈值
		case NULL_CHANNEL15_HC: device_get_channel_hc(15);break;        //通道报警回差
		case NULL_CHANNEL15_C: device_get_channel_correct(15);break;
		
		default:{device_ack_crc("",""); }break;
	}
}
/****************************************************/
//uint8_t id;
static void device_set_null(uint8_t *set_m_id,uint8_t end)   //初始模式串口获取数据
{
	uint8_t set = 0;
	uint8_t *temp_buf = set_m_id,param_size = temp_buf[6] - 1;
	temp_buf += 7;
	switch(set_m_id[3]-end)
	{				
		case NULL_SAM_TIME: device_set_sam_time(temp_buf,param_size);break;		
		case NULL_DIP_NUM: device_set_dip_num(temp_buf,param_size);break;
		case NULL_eliminate_signal: device_set_eliminate_signal(temp_buf);break;		
		case NULL_vbat_reset: device_vbat_reset();break;
		
		case NULL_CHANNEL1_UP: device_info.save.channel_up[1] = atof((char *)temp_buf);break;          //通道上限
		case NULL_CHANNEL1_DOWN: device_info.save.channel_down[1] = atof((char *)temp_buf);break;       //通道下限     //
		case NULL_CHANNEL1_F: device_info.save.channel_f[1] = atof((char *)temp_buf);break;        //通道波动阈值
		case NULL_CHANNEL1_HC: device_info.save.channel_hc[1] = atof((char *)temp_buf);break;        //通道报警回差
		case NULL_CHANNEL1_C: device_info.save.channel_c[1] = atof((char *)temp_buf);break;
		
		case NULL_CHANNEL2_UP: device_info.save.channel_up[2] = atof((char *)temp_buf);break;          //通道上限
		case NULL_CHANNEL2_DOWN: device_info.save.channel_down[2] = atof((char *)temp_buf);break;       //通道下限     //
		case NULL_CHANNEL2_F: device_info.save.channel_f[2] = atof((char *)temp_buf);break;        //通道波动阈值
		case NULL_CHANNEL2_HC: device_info.save.channel_hc[2] = atof((char *)temp_buf);break;        //通道报警回差
		case NULL_CHANNEL2_C: device_info.save.channel_c[2] = atof((char *)temp_buf);break;
		
		case NULL_CHANNEL3_UP: device_info.save.channel_up[3] = atof((char *)temp_buf);break;          //通道上限
		case NULL_CHANNEL3_DOWN: device_info.save.channel_down[3] = atof((char *)temp_buf);break;       //通道下限     //
		case NULL_CHANNEL3_F: device_info.save.channel_f[3] = atof((char *)temp_buf);break;        //通道波动阈值
		case NULL_CHANNEL3_HC: device_info.save.channel_hc[3] = atof((char *)temp_buf);break;        //通道报警回差
		case NULL_CHANNEL3_C: device_info.save.channel_c[3] = atof((char *)temp_buf);break;
		
		case NULL_CHANNEL4_UP: device_info.save.channel_up[4] = atof((char *)temp_buf);break;          //通道上限
		case NULL_CHANNEL4_DOWN: device_info.save.channel_down[4] = atof((char *)temp_buf);break;       //通道下限     //
		case NULL_CHANNEL4_F: device_info.save.channel_f[4] = atof((char *)temp_buf);break;        //通道波动阈值
		case NULL_CHANNEL4_HC: device_info.save.channel_hc[4] = atof((char *)temp_buf);break;        //通道报警回差
		case NULL_CHANNEL4_C: device_info.save.channel_c[4] = atof((char *)temp_buf);break;
		case NULL_CHANNEL5_UP: device_info.save.channel_up[5] = atof((char *)temp_buf);break;          //通道上限
		case NULL_CHANNEL5_DOWN: device_info.save.channel_down[5] = atof((char *)temp_buf);break;       //通道下限     //
		case NULL_CHANNEL5_F: device_info.save.channel_f[5] = atof((char *)temp_buf);break;        //通道波动阈值
		case NULL_CHANNEL5_HC: device_info.save.channel_hc[5] = atof((char *)temp_buf);break;        //通道报警回差
		case NULL_CHANNEL5_C: device_info.save.channel_c[5] = atof((char *)temp_buf);break;

		case NULL_CHANNEL6_UP: device_info.save.channel_up[6] = atof((char *)temp_buf);break;          //通道上限
		case NULL_CHANNEL6_DOWN: device_info.save.channel_down[6] = atof((char *)temp_buf);break;       //通道下限     //
		case NULL_CHANNEL6_F: device_info.save.channel_f[6] = atof((char *)temp_buf);break;        //通道波动阈值
		case NULL_CHANNEL6_HC: device_info.save.channel_hc[6] = atof((char *)temp_buf);break;        //通道报警回差
		case NULL_CHANNEL6_C: device_info.save.channel_c[6] = atof((char *)temp_buf);break;
		case NULL_CHANNEL7_UP: device_info.save.channel_up[7] = atof((char *)temp_buf);break;          //通道上限
		case NULL_CHANNEL7_DOWN: device_info.save.channel_down[7] = atof((char *)temp_buf);break;       //通道下限     //
		case NULL_CHANNEL7_F: device_info.save.channel_f[7] = atof((char *)temp_buf);break;        //通道波动阈值
		case NULL_CHANNEL7_HC: device_info.save.channel_hc[7] = atof((char *)temp_buf);break;        //通道报警回差
		case NULL_CHANNEL7_C: device_info.save.channel_c[7] = atof((char *)temp_buf);break;
		
		case NULL_CHANNEL8_UP: device_info.save.channel_up[8] = atof((char *)temp_buf);break;          //通道上限
		case NULL_CHANNEL8_DOWN: device_info.save.channel_down[8] = atof((char *)temp_buf);break;       //通道下限     //
		case NULL_CHANNEL8_F: device_info.save.channel_f[8] = atof((char *)temp_buf);break;        //通道波动阈值
		case NULL_CHANNEL8_HC: device_info.save.channel_hc[8] = atof((char *)temp_buf);break;        //通道报警回差
		case NULL_CHANNEL8_C: device_info.save.channel_c[8] = atof((char *)temp_buf);break;
		case NULL_CHANNEL9_UP: device_info.save.channel_up[9] = atof((char *)temp_buf);break;          //通道上限
		case NULL_CHANNEL9_DOWN: device_info.save.channel_down[9] = atof((char *)temp_buf);break;       //通道下限     //
		case NULL_CHANNEL9_F: device_info.save.channel_f[9] = atof((char *)temp_buf);break;        //通道波动阈值
		case NULL_CHANNEL9_HC: device_info.save.channel_hc[9] = atof((char *)temp_buf);break;        //通道报警回差
		case NULL_CHANNEL9_C: device_info.save.channel_c[9] = atof((char *)temp_buf);break;
		
		case NULL_CHANNEL10_UP: device_info.save.channel_up[10] = atof((char *)temp_buf);break;          //通道上限
		case NULL_CHANNEL10_DOWN: device_info.save.channel_down[10] = atof((char *)temp_buf);break;       //通道下限     //
		case NULL_CHANNEL10_F: device_info.save.channel_f[10] = atof((char *)temp_buf);break;        //通道波动阈值
		case NULL_CHANNEL10_HC: device_info.save.channel_hc[10] = atof((char *)temp_buf);break;        //通道报警回差
		case NULL_CHANNEL10_C: device_info.save.channel_c[10] = atof((char *)temp_buf);break;
		case NULL_CHANNEL11_UP: device_info.save.channel_up[11] = atof((char *)temp_buf);break;          //通道上限
		case NULL_CHANNEL11_DOWN: device_info.save.channel_down[11] = atof((char *)temp_buf);break;       //通道下限     //
		case NULL_CHANNEL11_F: device_info.save.channel_f[11] = atof((char *)temp_buf);break;        //通道波动阈值
		case NULL_CHANNEL11_HC: device_info.save.channel_hc[11] = atof((char *)temp_buf);break;        //通道报警回差
		case NULL_CHANNEL11_C: device_info.save.channel_c[11] = atof((char *)temp_buf);break;
		
		case NULL_CHANNEL12_UP: device_info.save.channel_up[12] = atof((char *)temp_buf);break;          //通道上限
		case NULL_CHANNEL12_DOWN: device_info.save.channel_down[12] = atof((char *)temp_buf);break;       //通道下限     //
		case NULL_CHANNEL12_F: device_info.save.channel_f[12] = atof((char *)temp_buf);break;        //通道波动阈值
		case NULL_CHANNEL12_HC: device_info.save.channel_hc[12] = atof((char *)temp_buf);break;        //通道报警回差
		case NULL_CHANNEL12_C: device_info.save.channel_c[12] = atof((char *)temp_buf);break;
		case NULL_CHANNEL13_UP: device_info.save.channel_up[13] = atof((char *)temp_buf);break;          //通道上限
		case NULL_CHANNEL13_DOWN: device_info.save.channel_down[13] = atof((char *)temp_buf);break;       //通道下限     //
		case NULL_CHANNEL13_F: device_info.save.channel_f[13] = atof((char *)temp_buf);break;        //通道波动阈值
		case NULL_CHANNEL13_HC: device_info.save.channel_hc[13] = atof((char *)temp_buf);break;        //通道报警回差
		case NULL_CHANNEL13_C: device_info.save.channel_c[13] = atof((char *)temp_buf);break;
		
		case NULL_CHANNEL14_UP: device_info.save.channel_up[14] = atof((char *)temp_buf);break;          //通道上限
		case NULL_CHANNEL14_DOWN: device_info.save.channel_down[14] = atof((char *)temp_buf);break;       //通道下限     //
		case NULL_CHANNEL14_F: device_info.save.channel_f[14] = atof((char *)temp_buf);break;        //通道波动阈值
		case NULL_CHANNEL14_HC: device_info.save.channel_hc[14] = atof((char *)temp_buf);break;        //通道报警回差
		case NULL_CHANNEL14_C: device_info.save.channel_c[14] = atof((char *)temp_buf);break;
		case NULL_CHANNEL15_UP: device_info.save.channel_up[15] = atof((char *)temp_buf);break;          //通道上限
		case NULL_CHANNEL15_DOWN: device_info.save.channel_down[15] = atof((char *)temp_buf);break;       //通道下限     //
		case NULL_CHANNEL15_F: device_info.save.channel_f[15] = atof((char *)temp_buf);break;        //通道波动阈值
		case NULL_CHANNEL15_HC: device_info.save.channel_hc[15] = atof((char *)temp_buf);break;        //通道报警回差
		case NULL_CHANNEL15_C: device_info.save.channel_c[15] = atof((char *)temp_buf);break;
		
		default: set = *temp_buf - 0X30;break;														
	}
	if(set == 1)
	{
		device_info.debug_en = 0;
		device_param_get(set_m_id[3]);
		delay_vtask(1000);
		device_info.debug_en = 1;
	}
	else 
	{
		device_info.debug_en = 0;
		device_param_get(set_m_id[3]);
	}
}
/****************************************************
void device_get_mqtt(uint8_t get_m_id)   //mqtt模式串口获取数据
{
			switch(get_m_id)
			{				
				case mqtt_SAVE: device_ack_crc("保存参数Save",""); break;				
				case mqtt_REST: device_ack_crc("重启设备Restart",""); break;				
				case mqtt_CLEAR_HIST: device_ack_crc("清除历史数据Clear historical data","");break;	
				
				case mqtt_local_read: device_ack_crc("------设备基础信息读取Basic Information reading------","");break;
				case mqtt_MODEL: device_get_model();break;
				case mqtt_SENSOR: device_get_sensor();break;				
				case mqtt_IMEI: device_ack_crc("IMEI:",nb.nb_imei);break;
				case mqtt_IMSI: device_ack_crc("IMSI:",nb.nb_imsi);break;
				case mqtt_ICCID: device_ack_crc("ICCID:",nb.nb_iccid);break;				
				case mqtt_CBC: device_get_VBAT();break;
				case mqtt_CSQ: device_get_QSignal();break;							
				case mqtt_S1_value:  device_get_sensor1_data() ;break;
				case mqtt_S2_value:  device_get_sensor2_data() ;break;
				case mqtt_mpu_gx:   device_get_mpu_gx_data() ;break;
				case mqtt_mpu_gy:   device_get_mpu_gy_data() ;break;
				case mqtt_mpu_gz:   device_get_mpu_gz_data() ;break;
				case mqtt_mpu_ax:   device_get_mpu_ax_data() ;break;
				case mqtt_mpu_ay:   device_get_mpu_ay_data() ;break;
				case mqtt_mpu_az:   device_get_mpu_az_data() ;break;
				case mqtt_new_time: device_get_sensor_timers() ;break;	
				
				case mqtt_SRV_SETT: device_ack_crc("------服务器网络设置Server Settings------","");break;	
        case mqtt_SRV_OPERATOR: device_get_operator();break;				
				case mqtt_SRV_PROTOCOL: device_get_protocol();break;
				case mqtt_SRV_ADDR: device_ack_crc("服务器地址Server Addr:",device_info.save.srv_addr);break;
				case mqtt_SRV_PORT: device_ack_crc("服务器端口Server Port:",device_info.save.srv_port);break;
				case mqtt_SRV_LOGIN: device_ack_crc("客户端ID/序列号Client ID:",device_info.save.srv_login);break;					
				case mqtt_SRV_USERNAME: device_ack_crc("用户名User Name:",device_info.save.srv_UserName);break;
				case mqtt_SRV_PASSWORD: device_ack_crc("密码Password:",device_info.save.srv_Password);break;
				case mqtt_SRV_TOPIC: device_ack_crc("主题Topic:",device_info.save.srv_Topic);break;
				case mqtt_SRV_SUBSCRIPTION: device_ack_crc("订阅Subacription:",device_info.save.srv_Subacription);break;
				case mqtt_SIM_PIN: device_ack_crc("SIM PIN[默认为空default empty]:",device_info.save.srv_sim_pin);break;
				case mqtt_SIM_PUK: device_ack_crc("SIM PUK[默认为空default empty]:",device_info.save.srv_sim_puk);break;
				case mqtt_APN:device_ack_crc("APN[默认为空default empty]:",device_info.save.srv_apn);break;
				case mqtt_APN_USER_NAME:device_ack_crc("APN NAME[默认为空default empty]:",device_info.save.srv_apn_user_name);break;
				case mqtt_APN_USER_PASS:device_ack_crc("APN PASSWORD[默认为空default empty]:",device_info.save.srv_apn_user_pass);break;
				
				case mqtt_SYS_SETT: device_ack_crc("------系统设置System Settings------","");break;				
				case mqtt_SRV_TIME: device_get_srv_timr();break;
        case mqtt_SAM_TIME: device_get_sam_timr();break;
				case mqtt_DIP_NUM: device_get_dip_num();break;
				case mqtt_device_dabao: device_get_dabao();break;	
				case mqtt_zhengdian:device_get_zhengdian();break;
				case mqtt_device_mpu:	device_get_mpu();break;
				case mqtt_device_shock:	device_get_shock();break;
				case mqtt_device_BJ_pinlv_stu: device_get_BJ_pinlv_stu();break;
				case mqtt_device_BJ_pinlv_time: device_get_BJ_pinlv_time();break;
				case mqtt_S1_RANGE: device_get_sensor1_range();break;
				case mqtt_S2_RANGE: device_get_sensor2_range();break;
				case mqtt_S1_UNIT:  device_get_sennsor1_unit();break; 	
        case mqtt_S2_UNIT:  device_get_sennsor2_unit();break;				
				case mqtt_eliminate_signal:	device_get_eliminate_signal();break;
        case mqtt_sensor_clear_f:  device_get_sensor_clear();break;					
				case mqtt_S1_CORRECT: device_get_sensor1_correct();break;
				case mqtt_S2_CORRECT: device_get_sensor2_correct();break;				
					
				case mqtt_ALARM_SEET: device_ack_crc("-----报警设置Transfinite Settings-----","");break;	
				case mqtt_shock_alarm: device_get_shock_data();break;
				case mqtt_S1_UP: device_get_S1_UP();break;			
				case mqtt_S1_DOWN: device_get_S1_DOWN();break;			
				case mqtt_S2_UP: device_get_S2_UP();break;				
				case mqtt_S2_DOWN: device_get_S2_DOWN();break;
				case mqtt_S1_fluctuate: device_get_S1_fluctuate();break;
				case mqtt_S2_fluctuate: device_get_S2_fluctuate();break;
				case mqtt_fluctuate_dowm_S1: device_get_fluctuate_dowm_S1();break;
				case mqtt_fluctuate_dowm_S2: device_get_fluctuate_dowm_S2();break;
				
				case mqtt_vbat_reset: device_vbat_reset_get();break;
									
				case mqtt_BULID_TIME: device_ack_crc("生成时间Bulid date:",__DATE__);break;		
				case mqtt_BULID_Version: device_ack_crc("版本号Version:",Version);break;				
        default:{device_ack_crc("",""); }break;
			}
}
****************************************************
void device_set_mqtt(uint8_t *set_m_id)   //mqtt模式串口设置数据
{	
	uint8_t set = 0;
	uint8_t *temp_buf = set_m_id, param_size = temp_buf[6] - 1;
	temp_buf += 7;
	
			switch(set_m_id[3])
			{
				case mqtt_SAVE: 
					device_param_write(&device_info.save);
		      	break;				
				case mqtt_REST: 	
				    	device_param_get(set_m_id[3]);	
				      delay_vtask(200);
							LL_GPIO_ResetOutputPin(pwr3v_gpio, pwr3v_gpio_id);   //传感器断电
							LL_GPIO_ResetOutputPin(pwrpcie_gpio, pwrpcie_gpio_id);   			 //模块断电   
							while(device_info.event_flash == EVENT_RUN)
							{
								delay_vtask(1000);
							}
//							NVIC_SystemReset();
							LL_RCC_SetChipReset();
				case mqtt_CLEAR_HIST:
					device_info.event_flash = EVENT_RUN;
					xTaskNotify(device_flashHandle, C_FLASH, eSetBits);	
									break;				
				case mqtt_MODEL:device_set_model(temp_buf);break;
				
				case mqtt_SRV_OPERATOR: device_set_operator(temp_buf);device_info.device_en=0;break;
				case mqtt_SRV_PROTOCOL: device_set_protocol(temp_buf);device_info.device_en=0;break;			
				case mqtt_SRV_ADDR: device_set_strparam(device_info.save.srv_addr,temp_buf,sizeof(device_info.save.srv_addr),param_size);	
														device_info.device_en=0; break;																				
				case mqtt_SRV_PORT:device_set_strparam(device_info.save.srv_port,temp_buf,sizeof(device_info.save.srv_port),param_size);	
													 device_info.device_en=0;	break;
				case mqtt_SRV_LOGIN:device_set_strparam(device_info.save.srv_login,temp_buf,sizeof(device_info.save.srv_login),param_size); 
														device_info.device_en=0;	break;					
				case mqtt_SRV_USERNAME: device_set_strparam(device_info.save.srv_UserName,temp_buf,sizeof(device_info.save.srv_UserName),param_size);
																device_info.device_en=0; break;
				case mqtt_SRV_PASSWORD: device_set_strparam(device_info.save.srv_Password,temp_buf,sizeof(device_info.save.srv_Password),param_size);
																device_info.device_en=0; break;
				case mqtt_SRV_TOPIC: device_set_strparam(device_info.save.srv_Topic,temp_buf,sizeof(device_info.save.srv_Topic),param_size);
														 device_info.device_en=0; break;
				case mqtt_SRV_SUBSCRIPTION: device_set_strparam(device_info.save.srv_Subacription,temp_buf,sizeof(device_info.save.srv_Subacription),param_size);
																		device_info.device_en=0; break;
				case mqtt_SIM_PIN:device_set_strparam(device_info.save.srv_sim_pin,temp_buf,sizeof(device_info.save.srv_sim_pin),param_size); //SIM PIN
											device_info.device_en=0;	break;
				case mqtt_SIM_PUK:device_set_strparam(device_info.save.srv_sim_puk,temp_buf,sizeof(device_info.save.srv_sim_puk),param_size); //SIM PUK
											device_info.device_en=0;	break;
				case mqtt_APN:device_set_strparam(device_info.save.srv_apn,temp_buf,sizeof(device_info.save.srv_apn),param_size); //APN
											device_info.device_en=0;	break;
				case mqtt_APN_USER_NAME:device_set_strparam(device_info.save.srv_apn_user_name,temp_buf,sizeof(device_info.save.srv_apn_user_name),param_size); 
																device_info.device_en=0;	break;
				case mqtt_APN_USER_PASS:device_set_strparam(device_info.save.srv_apn_user_pass,temp_buf,sizeof(device_info.save.srv_apn_user_pass),param_size); 
																device_info.device_en=0;	break;
				
				case mqtt_SRV_TIME: 	device_set_srv_time(temp_buf,param_size);device_info.device_en=0;break;
				case mqtt_SAM_TIME: 	device_set_sam_time(temp_buf,param_size);device_info.device_en=0;break;
				case mqtt_DIP_NUM:    device_set_dip_num(temp_buf,param_size);device_info.device_en=0;break;
				case mqtt_device_dabao: device_set_dabao(temp_buf);device_info.device_en=0;break;
				case mqtt_zhengdian: device_set_zhengdian(temp_buf);device_info.device_en=0;break;
        case mqtt_device_mpu: device_set_mpu(temp_buf);device_info.device_en=0;break;
        case mqtt_device_shock: device_set_shock(temp_buf);device_info.device_en=0;break;
				case mqtt_device_BJ_pinlv_stu: device_set_BJ_pinlv_stu(temp_buf);device_info.device_en=0;break;
				case mqtt_device_BJ_pinlv_time: device_set_BJ_pinlv_time(temp_buf,param_size);device_info.device_en=0;break;
				case mqtt_S1_UNIT: device_set_sennsor_unit(temp_buf,&device_info.save.sensor1_unit);device_info.device_en=0;break;
        case mqtt_S2_UNIT: device_set_sennsor_unit(temp_buf,&device_info.save.sensor2_unit);device_info.device_en=0;break;				
				case mqtt_eliminate_signal:	device_set_eliminate_signal(temp_buf);device_info.device_en=0;break;
        case mqtt_sensor_clear_f:		device_set_sensor_clear(temp_buf);device_info.device_en=0;break;						
				case mqtt_S1_CORRECT: device_info.save.sensor1_C = atof((char *)temp_buf);device_info.device_en=0;break;
				case mqtt_S2_CORRECT: device_info.save.sensor2_C = atof((char *)temp_buf);device_info.device_en=0;break;				
				
        case mqtt_shock_alarm: device_set_shock_data(temp_buf,param_size);device_info.device_en=0;break;		
				case mqtt_S1_UP: device_info.save.sensor1_up = atof((char *)temp_buf);device_info.device_en=0;break;			
				case mqtt_S1_DOWN: device_info.save.sensor1_down = atof((char *)temp_buf);device_info.device_en=0;break;			
				case mqtt_S2_UP: device_info.save.sensor2_up = atof((char *)temp_buf);device_info.device_en=0;break;				
				case mqtt_S2_DOWN: device_info.save.sensor2_down = atof((char *)temp_buf);device_info.device_en=0; break;
				case mqtt_S1_fluctuate: device_info.save.sensor1_f = atof((char *)temp_buf);device_info.device_en=0;break;
				case mqtt_S2_fluctuate: device_info.save.sensor2_f = atof((char *)temp_buf);device_info.device_en=0;break;
				case mqtt_fluctuate_dowm_S1: device_info.save.sensor1_hc = atof((char *)temp_buf);device_info.device_en=0;break;
				case mqtt_fluctuate_dowm_S2: device_info.save.sensor2_hc = atof((char *)temp_buf);device_info.device_en=0;break;
				case mqtt_vbat_reset: device_vbat_reset();break;
				
				default: set = *temp_buf - 0X30;break;														
	}
		if(set == 1)
		{
			device_info.debug_en = 0;
			device_get_mqtt(set_m_id[3]);
			delay_vtask(1000);
			device_info.debug_en = 1;
		}
		else 
		{
			device_info.debug_en = 0;
			device_get_mqtt(set_m_id[3]);
		}	
}
****************************************************/
/****************************************************/
void device_get_nb(uint8_t get_nb_id)    //NB模式串口获取数据
{
	switch(get_nb_id)
	{		
		case nb_local_read: device_ack_crc("------设备联网信息读取Network Information reading------","");break;		
		case nb_IMEI: device_ack_crc("IMEI:",nb.nb_imei);break;
		case nb_IMSI: device_ack_crc("IMSI:",nb.nb_imsi);break;
		case nb_ICCID: device_ack_crc("ICCID:",nb.nb_iccid);break;
		case nb_CSQ: device_get_QSignal();break;
		case nb_CESQ: device_get_cesq();break;							
				
		
		case nb_SRV_SETT: device_ack_crc("------设备联网信息设置Network Information Setting------","");break;
		case nb_SRV_OPERATOR: device_get_operator();break;					
		case nb_SRV_PROTOCOL: device_get_protocol();break;
		case nb_SRV_TYPE: device_get_sever_type();break;
		case nb_SRV_ADDR: device_ack_crc("服务器地址Server Addr:",device_info.save.srv_addr);break;
		case nb_SRV_PORT: device_ack_crc("服务器端口Server Port:",device_info.save.srv_port);break;
		case nb_SRV_LOGIN: device_ack_crc("序列号SN:",device_info.save.srv_login);		break;
		case nb_SIM_PIN: device_ack_crc("SIM PIN[默认为空default empty]:",device_info.save.srv_sim_pin);break;
		case nb_SIM_PUK: device_ack_crc("SIM PUK[默认为空default empty]:",device_info.save.srv_sim_puk);break;
		case nb_APN:device_ack_crc("APN[默认为空default empty]:",device_info.save.srv_apn);break;
		case nb_APN_USER_NAME:device_ack_crc("APN NAME[默认为空default empty]:",device_info.save.srv_apn_user_name);break;
		case nb_APN_USER_PASS:device_ack_crc("APN PASSWORD[默认为空default empty]:",device_info.save.srv_apn_user_pass);break;	

		case mqtt_SRV_USERNAME: get_mqtt_username();break;
		case mqtt_SRV_PASSWORD: get_mqtt_password();break;
		case mqtt_SRV_TOPIC: get_mqtt_topic();break;
		case mqtt_SRV_SUBSCRIPTION: get_mqtt_subscription();break;

		case nb_SYS_SETT: device_ack_crc("------系统参数设置System Settings------","");break;				
		case nb_SRV_TIME: device_get_srv_timr();break;
		case nb_device_dabao:	device_get_dabao();break;
		case nb_zhengdian:device_get_zhengdian();break;
		case nb_device_BJ_pinlv_stu: device_get_BJ_pinlv_stu();break;
		case nb_device_BJ_pinlv_time: device_get_BJ_pinlv_time();break;
		case nb_gps_stu: get_gps_status();break;
		case nb_gps_E: get_gps_data_E();break;
		case nb_gps_N: get_gps_data_N();break;
						
		default:device_get_null(get_nb_id-nb_end);break;
	}
}
/****************************************************/
void device_set_nb(uint8_t *set_nb_id,uint8_t end)    //NB模式串口设置数据
{	
//	uint8_t set = 0;
	uint8_t *temp_buf = set_nb_id, param_size = temp_buf[6] - 1;
	temp_buf += 7;

	switch(set_nb_id[3]-end)
	{
		case nb_SRV_OPERATOR: device_set_operator(temp_buf);break;				
		case nb_SRV_PROTOCOL: device_set_protocol(temp_buf);break;	
		case nb_SRV_TYPE: device_set_sever_type(temp_buf);break;				
		case nb_SRV_ADDR: device_set_strparam(device_info.save.srv_addr,temp_buf,sizeof(device_info.save.srv_addr),param_size);	break;																					
		case nb_SRV_PORT:device_set_strparam(device_info.save.srv_port, temp_buf,sizeof(device_info.save.srv_port),param_size);	break;
		case nb_SRV_LOGIN:device_set_strparam(device_info.save.srv_login,temp_buf,sizeof(device_info.save.srv_login),param_size); break;
		case nb_SIM_PIN:device_set_strparam(device_info.save.srv_sim_pin,temp_buf,sizeof(device_info.save.srv_sim_pin),param_size); break;
		case nb_SIM_PUK:device_set_strparam(device_info.save.srv_sim_puk,temp_buf,sizeof(device_info.save.srv_sim_puk),param_size); break;
		case nb_APN:device_set_strparam(device_info.save.srv_apn,temp_buf,sizeof(device_info.save.srv_apn),param_size);break;
		case nb_APN_USER_NAME:device_set_strparam(device_info.save.srv_apn_user_name,temp_buf,sizeof(device_info.save.srv_apn_user_name),param_size);break;
		case nb_APN_USER_PASS:device_set_strparam(device_info.save.srv_apn_user_pass,temp_buf,sizeof(device_info.save.srv_apn_user_pass),param_size); break;

		case mqtt_SRV_USERNAME: device_set_strparam(device_info.save.srv_UserName,temp_buf,sizeof(device_info.save.srv_UserName),param_size);break;
		case mqtt_SRV_PASSWORD:  device_set_strparam(device_info.save.srv_Password,temp_buf,sizeof(device_info.save.srv_Password),param_size);break;
		case mqtt_SRV_TOPIC: device_set_strparam(device_info.save.srv_Topic,temp_buf,sizeof(device_info.save.srv_Topic),param_size);break;
		case mqtt_SRV_SUBSCRIPTION: device_set_strparam(device_info.save.srv_Subacription,temp_buf,sizeof(device_info.save.srv_Subacription),param_size);break;
																					 
		case nb_SRV_TIME: device_set_srv_time(temp_buf,param_size);break;
		case nb_device_dabao: device_set_dabao(temp_buf);break;	
		case nb_zhengdian: device_set_zhengdian(temp_buf);break;
		case nb_device_BJ_pinlv_stu: device_set_BJ_pinlv_stu(temp_buf);break;
		case nb_device_BJ_pinlv_time: device_set_BJ_pinlv_time(temp_buf,param_size);break;
		case nb_gps_stu: set_gps_status(temp_buf);break;

	
		default: device_set_null(set_nb_id,nb_end+end);return;														
	}
	if(device_info.debug_en == 1)
	{
		device_info.debug_en = 0;		
	}
	device_param_get(set_nb_id[3]);
}
/****************************************************
void device_get_lorawan(uint8_t get_nb_id)    //NB模式串口获取数据
{
			switch(get_nb_id)
			{
				case lorawan_SAVE: device_ack_crc("保存参数Save",""); break;				
				case lorawan_REST: device_ack_crc("重启设备Restart",""); break;				
				case lorawan_CLEAR_HIST: device_ack_crc("清除历史数据Clear historical data","");break;	
				
				case lorawan_local_read: device_ack_crc("------设备基础信息读取Basic Information reading------","");break;
				case lorawan_MODEL: device_get_model();break;
				case lorawan_SENSOR: device_get_sensor();break;			
				case lorawan_NETID: device_ack_crc("lorawan网络ID:",device_info.save.srv_Password);;break;
				case lorawan_STATUS: device_get_lorawan_satatus();break;
				case lorawan_CSQ: device_get_QSignal();break;
				
				case lorawan_S1_value:  device_get_sensor1_data() ;break;
				case lorawan_S2_value:  device_get_sensor2_data() ;break;
				case lorawan_mpu_gx:   device_get_mpu_gx_data() ;break;
				case lorawan_mpu_gy:   device_get_mpu_gy_data() ;break;
				case lorawan_mpu_gz:   device_get_mpu_gz_data() ;break;
				case lorawan_mpu_ax:   device_get_mpu_ax_data() ;break;
				case lorawan_mpu_ay:   device_get_mpu_ay_data() ;break;
				case lorawan_mpu_az:   device_get_mpu_az_data() ;break;
				case lorawan_new_time: device_get_sensor_timers() ;break;				
				
				case lorawan_SRV_SETT: device_ack_crc("------服务器网络设置Server Settings------","");break;
        case lorawan_SRV_DEVEUI: device_ack_crc("设备EUI[DEV EUI]:",device_info.save.srv_login);break;			
				case lorawan_SRV_APPEUI: device_ack_crc("应用EUI[APP EUI]:",device_info.save.srv_UserName);break;	
				case lorawan_SRV_DEVADDR: device_ack_crc("设备地址[DEV ADDR](只有ABP模式下支持设置):",device_info.save.srv_addr);break;
				case lorawan_SRV_APPKEY: device_get_lorawan_APPKEY();break;
				case lorawan_SRV_NWKSKEY: device_get_lorawan_NWKSKEY();break;
				case lorawan_SRV_APPSKEY: device_get_lorawan_APPSKEY();break;
				case lorawan_SRV_CHGROUP: device_get_lorawan_CHGROUP();break;
				case lorawan_SRV_ULDLMODE: device_get_lorawan_ULDLMODE();break;
        case lorawan_SRV_JOINMODE: device_get_lorawan_JOINMODE();break;
				case lorawan_SRV_CLASS: device_get_lorawan_CLASS();break;
				case lorawan_SRV_BAND: device_get_lorawan_BAND();break;	
        case lorawan_SRV_APPPORT: device_get_lorawan_APPPORT();break;
				case lorawan_SRV_DATARATE: device_get_lorawan_DATARATE();break;
				case lorawan_SRV_TXPWR: device_get_lorawan_TXPWR();break;
				case lorawan_SRV_ADR: device_get_lorawan_ADR();break;

				case lorawan_SYS_SETT: device_ack_crc("------系统参数设置System Settings------","");break;				
				case lorawan_SRV_TIME: device_get_srv_timr();break;
        case lorawan_SAM_TIME: device_get_sam_timr();break;
				case lorawan_DIP_NUM: device_get_dip_num();break;
        case lorawan_device_dabao:	device_get_dabao();break;
				case lorawan_zhengdian:device_get_zhengdian();break;
				case lorawan_device_mpu:	device_get_mpu();break;	
				case lorawan_device_shock:	device_get_shock();break;
				case lorawan_device_BJ_pinlv_stu: device_get_BJ_pinlv_stu();break;
				case lorawan_device_BJ_pinlv_time: device_get_BJ_pinlv_time();break;
        case lorawan_S1_RANGE: device_get_sensor1_range();break;
				case lorawan_S2_RANGE: device_get_sensor2_range();break;				
				case lorawan_S1_UNIT:  device_get_sennsor1_unit();break; 	
        case lorawan_S2_UNIT:  device_get_sennsor2_unit();break;    					
				case lorawan_eliminate_signal: device_get_eliminate_signal();break;			  
        case lorawan_sensor_clear_f:  device_get_sensor_clear();break;												
				case lorawan_S1_CORRECT: device_get_sensor1_correct();break;
				case lorawan_S2_CORRECT: device_get_sensor2_correct();break;				
					
				case lorawan_ALARM_SEET: device_ack_crc("-----报警参数设置Transfinite Settings-----","");break;	
        case lorawan_shock_alarm: device_get_shock_data();break;				
				case lorawan_S1_UP: device_get_S1_UP();break;			
				case lorawan_S1_DOWN: device_get_S1_DOWN();break;			
				case lorawan_S2_UP: device_get_S2_UP();break;				
				case lorawan_S2_DOWN: device_get_S2_DOWN();break;
				case lorawan_S1_fluctuate: device_get_S1_fluctuate();break;
				case lorawan_S2_fluctuate: device_get_S2_fluctuate();break;
				case lorawan_fluctuate_dowm_S1: device_get_fluctuate_dowm_S1();break;
				case lorawan_fluctuate_dowm_S2: device_get_fluctuate_dowm_S2();break;
																
				case lorawan_device_restart_num: device_get_restart_num();break;
				case lorawan_vbat_reset: device_vbat_reset_get();break;
						
				case lorawan_BULID_TIME: device_ack_crc("生成时间Bulid date:",__DATE__);break;		
				case lorawan_BULID_Version: device_ack_crc("版本号Version:",Version);break;
				
        default:{device_ack_crc("",""); }break;
			}
}
****************************************************
void device_set_lorawan(uint8_t *set_nb_id)    //NB模式串口设置数据
{	
	uint8_t set = 0;
	uint8_t *temp_buf = set_nb_id, param_size = temp_buf[6] - 1;
	temp_buf += 7;

			switch(set_nb_id[3])
			{
				case lorawan_SAVE: device_param_write(&device_info.save);
													 if(device_info.lorawan_set_data_en==1)
													 {
														 while(device_info.event_up == EVENT_RUN)
														 {
																delay_vtask(1000);
														 }
														 device_info.event_up = EVENT_RUN;
														 xTaskNotify(sever_nbHandle,loraset_IT,eSetBits);
													 }
//													 lorawan_setstate();
										       break;				
				case lorawan_REST: 	
										device_param_get(set_nb_id[3]);	
				            delay_vtask(200);
//										nb_OFF();
										LL_GPIO_ResetOutputPin(pwr3v_gpio, pwr3v_gpio_id);   //传感器断电
										LL_GPIO_ResetOutputPin(pwrpcie_gpio, pwrpcie_gpio_id);   			 //模块断电  
				            while(device_info.event_flash == EVENT_RUN)
										{
											delay_vtask(1000);
										}
	//									NVIC_SystemReset();
										LL_RCC_SetChipReset();
				case lorawan_CLEAR_HIST: device_info.event_flash = EVENT_RUN;
														xTaskNotify(device_flashHandle, C_FLASH, eSetBits);	
														break;	
				
				case lorawan_MODEL: device_set_model(temp_buf);device_info.device_en=0;break;	
				case lorawan_SRV_DEVEUI: device_set_strparam(device_info.save.srv_login,temp_buf,sizeof(device_info.save.srv_login),param_size);
										             device_info.lorawan_set_data_en=1;break;			
				case lorawan_SRV_APPEUI: device_set_strparam(device_info.save.srv_UserName,temp_buf,sizeof(device_info.save.srv_UserName),param_size);
																 device_info.lorawan_set_data_en=1;break;	
				case lorawan_SRV_DEVADDR: device_set_strparam(device_info.save.srv_addr,temp_buf,sizeof(device_info.save.srv_addr),param_size);
																  device_info.lorawan_set_data_en=1;break;
				case lorawan_SRV_APPKEY: device_string_to_hex(temp_buf,device_info.save.srv_apn,param_size,sizeof(device_info.save.srv_apn));
																 device_info.device_en=0; device_info.lorawan_set_data_en=1;break;
				case lorawan_SRV_NWKSKEY: device_string_to_hex(temp_buf,device_info.save.srv_apn_user_name,param_size,sizeof(device_info.save.srv_apn_user_name));
																	device_info.device_en=0; device_info.lorawan_set_data_en=1;break;
				case lorawan_SRV_APPSKEY: device_string_to_hex(temp_buf,device_info.save.srv_apn_user_pass,param_size,sizeof(device_info.save.srv_apn_user_pass));
																	device_info.device_en=0;device_info.lorawan_set_data_en=1; break;
				case lorawan_SRV_CHGROUP: device_set_lorawan_CHGROUP(temp_buf,param_size);device_info.lorawan_set_data_en=1; break;
				case lorawan_SRV_ULDLMODE: device_set_lorawan_ULDLMODE(temp_buf);device_info.lorawan_set_data_en=1; break;
        case lorawan_SRV_JOINMODE: device_set_lorawan_JOINMODE(temp_buf);device_info.lorawan_set_data_en=1; break;
				case lorawan_SRV_CLASS: device_set_lorawan_CLASS(temp_buf);device_info.lorawan_set_data_en=1; break;
				case lorawan_SRV_BAND: device_set_lorawan_BAND(temp_buf);device_info.lorawan_set_data_en=1; break;	
        case lorawan_SRV_APPPORT: device_set_lorawan_APPPORT(temp_buf,param_size);device_info.lorawan_set_data_en=1; break;
				case lorawan_SRV_DATARATE: device_set_lorawan_DATARATE(temp_buf,param_size);device_info.lorawan_set_data_en=1; break;
				case lorawan_SRV_TXPWR: device_set_lorawan_TXPWR(temp_buf,param_size);device_info.lorawan_set_data_en=1; break;
				case lorawan_SRV_ADR: device_set_lorawan_ADR(temp_buf);device_info.lorawan_set_data_en=1; break;								
									
				case lorawan_SRV_TIME: device_set_srv_time(temp_buf,param_size);device_info.device_en=0;	break;
        case lorawan_SAM_TIME: device_set_sam_time(temp_buf,param_size);device_info.device_en=0;	break;	
				case lorawan_DIP_NUM:    device_set_dip_num(temp_buf,param_size);device_info.device_en=0;break;
        case lorawan_device_dabao: device_set_dabao(temp_buf);device_info.device_en=0;break;	
				case lorawan_zhengdian: device_set_zhengdian(temp_buf);device_info.device_en=0;break;
				case lorawan_device_mpu: device_set_mpu(temp_buf);device_info.device_en=0;break;
				case lorawan_device_shock: device_set_shock(temp_buf);device_info.device_en=0;break;
				case lorawan_device_BJ_pinlv_stu: device_set_BJ_pinlv_stu(temp_buf);device_info.device_en=0;break;
				case lorawan_device_BJ_pinlv_time: device_set_BJ_pinlv_time(temp_buf,param_size);device_info.device_en=0;break;
				case lorawan_S1_UNIT: device_set_sennsor_unit(temp_buf,&device_info.save.sensor1_unit);device_info.device_en=0;break;
        case lorawan_S2_UNIT: device_set_sennsor_unit(temp_buf,&device_info.save.sensor2_unit);device_info.device_en=0;break;
				case lorawan_eliminate_signal:	device_set_eliminate_signal(temp_buf);device_info.device_en=0;break;
        case lorawan_sensor_clear_f:		device_set_sensor_clear(temp_buf);device_info.device_en=0;break;				
				case lorawan_S1_CORRECT: device_info.save.sensor1_C = atof((char *)temp_buf);device_info.device_en=0;break;
				case lorawan_S2_CORRECT: device_info.save.sensor2_C = atof((char *)temp_buf);device_info.device_en=0;break;				
				
        case lorawan_shock_alarm: device_set_shock_data(temp_buf,param_size);device_info.device_en=0;break;		
				case lorawan_S1_UP: device_info.save.sensor1_up = atof((char *)temp_buf);device_info.device_en=0;break;			
				case lorawan_S1_DOWN: device_info.save.sensor1_down = atof((char *)temp_buf);device_info.device_en=0;break;			
				case lorawan_S2_UP: device_info.save.sensor2_up = atof((char *)temp_buf);device_info.device_en=0;break;				
				case lorawan_S2_DOWN: device_info.save.sensor2_down = atof((char *)temp_buf);device_info.device_en=0; break;
				case lorawan_S1_fluctuate: device_info.save.sensor1_f = atof((char *)temp_buf);device_info.device_en=0;break;
				case lorawan_S2_fluctuate: device_info.save.sensor2_f = atof((char *)temp_buf);device_info.device_en=0;break;
				case lorawan_fluctuate_dowm_S1: device_info.save.sensor1_hc = atof((char *)temp_buf);device_info.device_en=0;break;
				case lorawan_fluctuate_dowm_S2: device_info.save.sensor2_hc = atof((char *)temp_buf);device_info.device_en=0;break;
				case lorawan_vbat_reset: device_vbat_reset();break;
				
				default: set = *temp_buf - 0X30;break;														
	}
		if(set == 1)
		{
			device_info.debug_en = 0;
			device_get_lorawan(set_nb_id[3]);
			delay_vtask(1000);
			device_info.debug_en = 1;
		}
		else 
		{
			device_info.debug_en = 0;
			device_get_lorawan(set_nb_id[3]);
		}	
}
****************************************************
void device_get_lora(uint8_t get_lora_id)   //LORA模式串口获取数据
{
			switch(get_lora_id)
			{				
				case lora_SAVE: device_ack_crc("保存参数Save",""); 
													break;				
				case lora_REST: device_ack_crc("重启设备Restart",""); break;				
				case lora_CLEAR_HIST: device_ack_crc("清除历史数据Clear historical data","");break;		
				
				case lora_local_read: device_ack_crc("------设备基础信息读取Basic Information reading------","");break;
				case lora_MODEL: device_get_model();break;
				case lora_SENSOR: device_get_sensor();break;		
				case lora_once:    device_get_lora_once();break;
				case lora_rerty:   device_get_lora_retry();break;
				case lora_updata:  device_get_lora_updata();break;				
				case lora_upack:   device_get_lora_upack();break;
				case lora_num:     device_get_lora_num();break;			
				case lora_slot:    device_get_lora_slot();break;			
				case lora_time:    device_get_lora_time();break;			
				case lora_pwrtime: device_get_lora_pwrtime();break;			
				case lora_vbat:    device_get_VBAT();break;			
				case lora_qsignal: device_get_QSignal();break;							
				case lora_S1_value:  device_get_sensor1_data() ;break;
				case lora_S2_value:  device_get_sensor2_data() ;break;
				case lora_mpu_gx:   device_get_mpu_gx_data() ;break;
				case lora_mpu_gy:   device_get_mpu_gy_data() ;break;
				case lora_mpu_gz:   device_get_mpu_gz_data() ;break;
				case lora_mpu_ax:   device_get_mpu_ax_data() ;break;
				case lora_mpu_ay:   device_get_mpu_ay_data() ;break;
				case lora_mpu_az:   device_get_mpu_az_data() ;break;
				case lora_new_time: device_get_sensor_timers() ;break;				
				
				case lora_SRV_SETT:    device_ack_crc("------LORA参数设置Parameters Settings]-----","");break;				
				case lora_SRV_NS:      device_ack_crc("序号ID:",device_info.save.srv_UserName);break ;			
				case lora_SRV_NETID:   device_ack_crc("网络号NetID:",device_info.save.srv_addr);break;
				case lora_SRV_TX_POWER:device_ack_crc("发射功率Tx Power[dB]:",device_info.save.srv_port);break;
				case lora_SRV_speed:   device_get_lora_sudu();	break;					
				case lora_SRV_m_band:  device_ack_crc("频段BAND[MHz]:",device_info.save.srv_login); break;

				case lora_SYS_SETT: device_ack_crc("------系统参数设置System Settings------","");break;				
				case lora_SRV_TIME: device_get_srv_timr();break;		
        case lora_SAM_TIME: device_get_sam_timr();break;
				case lora_DIP_NUM: device_get_dip_num();break;
        case lora_device_mpu:	device_get_mpu();break;	
        case lora_device_shock:	device_get_shock();break;
				case lora_device_BJ_pinlv_stu: device_get_BJ_pinlv_stu();break;
				case lora_device_BJ_pinlv_time: device_get_BJ_pinlv_time();break;				
  			case lora_S1_RANGE: device_get_sensor1_range();break;
				case lora_S2_RANGE: device_get_sensor2_range();break;				
				case lora_S1_UNIT:  device_get_sennsor1_unit();break; 	
        case lora_S2_UNIT:  device_get_sennsor2_unit();break;  						
				case lora_eliminate_signal: device_get_eliminate_signal();break;
        case lora_sensor_clear_f:  device_get_sensor_clear();break;						
				case lora_S1_CORRECT: device_get_sensor1_correct();break;
				case lora_S2_CORRECT: device_get_sensor2_correct();break;				
					
				case lora_ALARM_SEET: device_ack_crc("-----报警参数设置Transfinite Settings-----","");break;
				case lora_shock_alarm: device_get_shock_data();break;				
				case lora_S1_UP: device_get_S1_UP();break;			
				case lora_S1_DOWN: device_get_S1_DOWN();break;			
				case lora_S2_UP: device_get_S2_UP();break;				
				case lora_S2_DOWN: device_get_S2_DOWN();break;
				case lora_S1_fluctuate: device_get_S1_fluctuate();break;
				case lora_S2_fluctuate: device_get_S2_fluctuate();break;
				case lora_fluctuate_dowm_S1: device_get_fluctuate_dowm_S1();break;
				case lora_fluctuate_dowm_S2: device_get_fluctuate_dowm_S2();break;
				
				case lora_vbat_reset: device_vbat_reset_get();break;
				
				case lora_BULID_TIME: device_ack_crc("生成时间Bulid date:",__DATE__);break;		
				case lora_BULID_Version: device_ack_crc("版本号Version:",Version);break;
				
        default:{device_ack_crc("",""); }break;
			}
}
****************************************************
void device_set_lora(uint8_t *set_lora_id)   //LORA模式串口设置数据
{	
	uint8_t set = 0;
	uint8_t *temp_buf = set_lora_id, param_size = temp_buf[6] - 1;
	temp_buf += 7;	
			switch(set_lora_id[3])
			{
				case lora_SAVE:
											device_param_write(&device_info.save);
											break;					
				case lora_REST:		
											device_param_get(set_lora_id[3]);
											delay_vtask(200);
											LL_GPIO_ResetOutputPin(pwr3v_gpio, pwr3v_gpio_id);   //传感器断电
											LL_GPIO_ResetOutputPin(pwrpcie_gpio, pwrpcie_gpio_id);   			 //模块断电   
									  	while(device_info.event_flash == EVENT_RUN)
											{
												delay_vtask(1000);
											}
	//										NVIC_SystemReset();
											LL_RCC_SetChipReset();
				case lora_CLEAR_HIST:	device_info.event_flash = EVENT_RUN; 
															xTaskNotify(device_flashHandle, C_FLASH, eSetBits);	
															break;		
				
				case lora_MODEL:device_set_model(temp_buf);device_info.device_en=0;break;		
								
				case lora_SRV_NS:  	 device_set_strparam(device_info.save.srv_UserName,temp_buf,sizeof(device_info.save.srv_UserName),param_size);  // Lora的 参数重复利用！！！
														device_info.device_en=0; break ;			
				case lora_SRV_NETID:  	device_set_strparam(device_info.save.srv_addr,temp_buf,sizeof(device_info.save.srv_addr),param_size);
                              device_info.device_en=0;	  	
				                      device_info.event_up = EVENT_RUN;
															xTaskNotify(sever_nbHandle, loraset_IT,eSetBits);    
															break;
				case lora_SRV_TX_POWER:device_set_strparam(device_info.save.srv_port,temp_buf,sizeof(device_info.save.srv_port),param_size);
																device_info.device_en=0;
                                device_info.event_up = EVENT_RUN;				
																xTaskNotify(sever_nbHandle,loraset_IT,eSetBits);    
																break;
				case lora_SRV_speed: device_set_lora_sudu(temp_buf);device_info.device_en=0;	
				                      device_info.event_up = EVENT_RUN;
															xTaskNotify(sever_nbHandle,loraset_IT,eSetBits);		
															break;					
				case lora_SRV_m_band:  	device_set_strparam(device_info.save.srv_login,temp_buf,sizeof(device_info.save.srv_login),param_size);
																device_info.device_en=0; 
																device_info.event_up = EVENT_RUN;
							                  xTaskNotify(sever_nbHandle,loraset_IT,eSetBits);	  
																break;
		                            			
				case lora_SRV_TIME: device_set_srv_time(temp_buf,param_size);device_info.device_en=0;	break;	
        case lora_SAM_TIME: device_set_sam_time(temp_buf,param_size);device_info.device_en=0;	break;
				case lora_DIP_NUM:    device_set_dip_num(temp_buf,param_size);device_info.device_en=0;break;
        case lora_device_mpu: device_set_mpu(temp_buf);device_info.device_en=0;break;
        case lora_device_shock: device_set_shock(temp_buf);device_info.device_en=0;break;
				case lora_device_BJ_pinlv_stu: device_set_BJ_pinlv_stu(temp_buf);device_info.device_en=0;break;
				case lora_device_BJ_pinlv_time: device_set_BJ_pinlv_time(temp_buf,param_size);device_info.device_en=0;break;				
				case lora_S1_UNIT: device_set_sennsor_unit(temp_buf,&device_info.save.sensor1_unit);device_info.device_en=0;break;
        case lora_S2_UNIT: device_set_sennsor_unit(temp_buf,&device_info.save.sensor2_unit);device_info.device_en=0;break;
				case lora_eliminate_signal:		device_set_eliminate_signal(temp_buf);device_info.device_en=0;break;
        case lora_sensor_clear_f:		device_set_sensor_clear(temp_buf);device_info.device_en=0;break;
				case lora_S1_CORRECT: device_info.save.sensor1_C = atof((char *)temp_buf);device_info.device_en=0;break;
				case lora_S2_CORRECT: device_info.save.sensor2_C = atof((char *)temp_buf);device_info.device_en=0;break;				
				
        case lora_shock_alarm: device_set_shock_data(temp_buf,param_size);device_info.device_en=0;break;				
				case lora_S1_UP: device_info.save.sensor1_up = atof((char *)temp_buf);device_info.device_en=0;break;			
				case lora_S1_DOWN: device_info.save.sensor1_down = atof((char *)temp_buf);device_info.device_en=0;break;			
				case lora_S2_UP: device_info.save.sensor2_up = atof((char *)temp_buf);device_info.device_en=0;break;				
				case lora_S2_DOWN: device_info.save.sensor2_down = atof((char *)temp_buf);device_info.device_en=0; break;
				case lora_S1_fluctuate: device_info.save.sensor1_f = atof((char *)temp_buf);device_info.device_en=0;break;
				case lora_S2_fluctuate: device_info.save.sensor2_f = atof((char *)temp_buf);device_info.device_en=0;break;
				case lora_fluctuate_dowm_S1: device_info.save.sensor1_hc = atof((char *)temp_buf);device_info.device_en=0;break;
				case lora_fluctuate_dowm_S2: device_info.save.sensor2_hc = atof((char *)temp_buf);device_info.device_en=0;break;
				case lora_vbat_reset: device_vbat_reset();break;

				default: set = *temp_buf - 0X30;break;														
	}
		if(set == 1)
		{
			device_info.debug_en = 0;
			device_get_lora(set_lora_id[3]);
			delay_vtask(1000);
			device_info.debug_en = 1;
		}
		else 
		{
			device_info.debug_en = 0;
			device_get_lora(set_lora_id[3]);
		}	
}
****************************************************
void device_get_mdbus485(uint8_t get_mdbus485_id)
{
		switch(get_mdbus485_id)
		{				
			case mdbus485_SAVE: device_ack_crc("保存参数Save",""); break;				
			case mdbus485_REST: device_ack_crc("重启设备Restart",""); break;				
			case mdbus485_CLEAR_HIST: device_ack_crc("清除历史数据Clear historical data","");break;	
			
			case mdbus485_local_read: device_ack_crc("------设备基础信息读取Basic Information reading------","");break;
			case mdbus485_MODEL: device_get_model();break;
			case mdbus485_SENSOR: device_get_sensor();break;								
			case mdbus485_S1_value:  device_get_sensor1_data() ;break;
			case mdbus485_S2_value:  device_get_sensor2_data() ;break;
			case mdbus485_new_time: device_get_sensor_timers() ;break;				
			case mdbus485_vbat: device_get_VBAT();break;		

			case mdbus485_SRV_SETT:  device_ack_crc("------RS485通讯设置local Settings------","");break;				
			case mdbus485_SRV_addr:  device_get_modbus_addr(); break ;			
			case mdbus485_SRV_band:  device_get_baud(); break;
			case mdbus485_SRV_parity:device_get_parity();  break;
			case mdbus485_SRV_data:  device_get_data_bits(); 	break;					
			case mdbus485_SRV_syop:  device_get_stop_bits(); break;
			
			case mdbus485_SYS_SETT: device_ack_crc("------系统参数设置System Settings------","");break;				
			case mdbus485_SAM_TIME: device_get_sam_timr();break;
			case mdbus485_DIP_NUM: device_get_dip_num();break;				
			case mdbus485_S1_RANGE: device_get_sensor1_range();break;
			case mdbus485_S2_RANGE: device_get_sensor2_range();break;				
			case mdbus485_S1_UNIT:  device_get_sennsor1_unit();break; 	
			case mdbus485_S2_UNIT:  device_get_sennsor2_unit();break;   					
			case mdbus485_eliminate_signal:device_get_eliminate_signal();break;
			case mdbus485_sensor_clear_f:  device_get_sensor_clear();break;				
			case mdbus485_S1_CORRECT: device_get_sensor1_correct();break;
			case mdbus485_S2_CORRECT: device_get_sensor2_correct();break;				
				
			case mdbus485_ALARM_SEET: device_ack_crc("-----报警参数设置Transfinite Settings-----","");break;
			case mdbus485_shock_alarm: device_get_shock_data();break;				
			case mdbus485_S1_UP: device_get_S1_UP();break;			
			case mdbus485_S1_DOWN: device_get_S1_DOWN();break;			
			case mdbus485_S2_UP: device_get_S2_UP();break;				
			case mdbus485_S2_DOWN: device_get_S2_DOWN();break;
			case mdbus485_S1_fluctuate: device_get_S1_fluctuate();break;
			case mdbus485_S2_fluctuate: device_get_S2_fluctuate();break;
			case mdbus485_fluctuate_dowm_S1: device_get_fluctuate_dowm_S1();break;
			case mdbus485_fluctuate_dowm_S2: device_get_fluctuate_dowm_S2();break;
			
			case mdbus485_vbat_reset: device_vbat_reset_get();break;
			
			case mdbus485_BULID_TIME: device_ack_crc("生成时间Bulid date:",__DATE__);break;		
			case mdbus485_BULID_Version: device_ack_crc("版本号Version:",Version);break;
			
			default:{device_ack_crc("",""); }break;
		}
}
****************************************************
void device_set_mdbus485(uint8_t *set_mdbus485_id)
{	
	uint8_t set = 0;
	uint8_t *temp_buf = set_mdbus485_id, param_size = temp_buf[6] - 1;
	temp_buf += 7;
	
			switch(set_mdbus485_id[3])
			{
				case mdbus485_SAVE: 
													device_param_write(&device_info.save);
													break;				
				case mdbus485_REST: 	
													device_param_get(set_mdbus485_id[3]);	
													delay_vtask(200);
													LL_GPIO_ResetOutputPin(pwr3v_gpio, pwr3v_gpio_id);   //传感器断电
													LL_GPIO_ResetOutputPin(pwrpcie_gpio, pwrpcie_gpio_id);   			 //模块断电   
													while(device_info.event_flash == EVENT_RUN)
													{
														delay_vtask(1000);
													}
//													NVIC_SystemReset();	
												  LL_RCC_SetChipReset();
				case mdbus485_CLEAR_HIST: device_info.event_flash = EVENT_RUN;
													xTaskNotify(device_flashHandle, C_FLASH, eSetBits);	
				                  break;		
				
				case mdbus485_MODEL:device_set_model(temp_buf);device_info.device_en=0;break;				
				case mdbus485_SRV_addr: device_set_modbus_addr(temp_buf);device_info.device_en=0;break ;			
				case mdbus485_SRV_band: device_set_baud(temp_buf);device_info.device_en=0; break;																					
				case mdbus485_SRV_parity:device_set_parity(temp_buf);device_info.device_en=0;	break;
				case mdbus485_SRV_data: device_set_data_bits(temp_buf);device_info.device_en=0;	break;					
				case mdbus485_SRV_syop:device_set_stop_bits(temp_buf);device_info.device_en=0;break;
				
				case mdbus485_SAM_TIME: device_set_sam_time(temp_buf,param_size);device_info.device_en=0;	break;
				case mdbus485_DIP_NUM:    device_set_dip_num(temp_buf,param_size);device_info.device_en=0;break;
				case mdbus485_S1_UNIT: device_set_sennsor_unit(temp_buf,&device_info.save.sensor1_unit);device_info.device_en=0;break;
        case mdbus485_S2_UNIT: device_set_sennsor_unit(temp_buf,&device_info.save.sensor2_unit);device_info.device_en=0;break;
				case mdbus485_eliminate_signal:		device_set_eliminate_signal(temp_buf);device_info.device_en=0;break;
        case mdbus485_sensor_clear_f:		device_set_sensor_clear(temp_buf);device_info.device_en=0;break;
				case mdbus485_S1_CORRECT: device_info.save.sensor1_C = atof((char *)temp_buf);device_info.device_en=0;break;
				case mdbus485_S2_CORRECT: device_info.save.sensor2_C = atof((char *)temp_buf);device_info.device_en=0;break;				
				
        case mdbus485_shock_alarm: device_set_shock_data(temp_buf,param_size);device_info.device_en=0;break;		
				case mdbus485_S1_UP: device_info.save.sensor1_up = atof((char *)temp_buf);device_info.device_en=0;break;			
				case mdbus485_S1_DOWN: device_info.save.sensor1_down = atof((char *)temp_buf);device_info.device_en=0;break;			
				case mdbus485_S2_UP: device_info.save.sensor2_up = atof((char *)temp_buf);device_info.device_en=0;break;				
				case mdbus485_S2_DOWN: device_info.save.sensor2_down = atof((char *)temp_buf);device_info.device_en=0; break;
				case mdbus485_S1_fluctuate: device_info.save.sensor1_f = atof((char *)temp_buf);device_info.device_en=0;break;
				case mdbus485_S2_fluctuate: device_info.save.sensor2_f = atof((char *)temp_buf);device_info.device_en=0;break;
				case mdbus485_fluctuate_dowm_S1: device_info.save.sensor1_hc = atof((char *)temp_buf);device_info.device_en=0;break;
				case mdbus485_fluctuate_dowm_S2: device_info.save.sensor2_hc = atof((char *)temp_buf);device_info.device_en=0;break;
				case mdbus485_vbat_reset: device_vbat_reset();break;
				default: set = *temp_buf - 0X30;break;														
	}
		if(set == 1)
		{
			device_info.debug_en = 0;
			device_get_mdbus485(set_mdbus485_id[3]);
			delay_vtask(1000);
			device_info.debug_en = 1;
		}
		else 
		{
			device_info.debug_en = 0;
			device_get_mdbus485(set_mdbus485_id[3]);
		}
}
****************************************************/
/****************************************************/
static void device_get_next(uint8_t next)
{
	if(device_info.save.tab_model == 0 || device_info.save.tab_module == 0)
	{
		device_get_new(next);
	}
	else if(device_info.save.tab_module == NULL1)
	{
		device_get_null(next);
	}
	else if(device_info.save.tab_module == BC26 || device_info.save.tab_module == BC95 || device_info.save.tab_module == BC20 || device_info.save.tab_module == EC25 || device_info.save.tab_module == EC25_G)
	{
		device_get_nb(next);
	}
	else if(device_info.save.tab_module == TP307V1_1_A)
	{
		
	}
	else if(device_info.save.tab_module == LORAWAN)
	{
		
	}
	else 
		device_get_new(next);
}
/****************************************************/
static void device_set_next(uint8_t *next,uint8_t end)
{
	if(device_info.save.tab_model == 0 || device_info.save.tab_module == 0)
	{
		device_set_new(next,end);
	}
	else if(device_info.save.tab_module == NULL1)
	{
		device_set_null(next,end);
	}
	else if(device_info.save.tab_module == BC26 || device_info.save.tab_module == BC95 || device_info.save.tab_module == BC20 || device_info.save.tab_module == EC25 || device_info.save.tab_module == EC25_G)
	{
		device_set_nb(next,end);
	}
	else if(device_info.save.tab_module == TP307V1_1_A)
	{
		
	}
	else if(device_info.save.tab_module == LORAWAN)
	{
		
	}
	else 
		device_set_new(next,end);
}
/****************************************************/
void device_param_get(uint8_t param_id)   //串口读取数据
{		
	switch(param_id)
	{	
		case main_SAVE: device_ack_crc("保存参数Save",""); break;				
		case main_REST: device_ack_crc("重启设备Restart",""); break;				
		case main_CLEAR_HIST: device_ack_crc("清除历史数据Clear historical data","");break;	
		case main_MODEL:	device_get_model();break;       
		case main_MODULE: device_get_module();break;
		case main_BULID_TIME: device_ack_crc("生成时间Bulid date:",__DATE__);break;		
		case main_BULID_Version: device_ack_crc("版本号Version:",Version);break;
		default: device_get_next(param_id-main_end);break;
	}
}
/****************************************************/
void device_param_set(uint8_t *wr_buf)     //串口设置数据
{
	switch(wr_buf[3])
	{	
		case main_SAVE: device_param_write(&device_info.save); break;				
		case main_REST: device_param_get(wr_buf[3]);	
			delay_vtask(80);
			LL_GPIO_ResetOutputPin(pwr3v_gpio, pwr3v_gpio_id);   //传感器断电
			LL_GPIO_ResetOutputPin(pwrpcie_gpio, pwrpcie_gpio_id);   			 //模块断电   
			while(device_info.event_flash == EVENT_RUN) {
				delay_vtask(200);
			}
			LL_RCC_SetChipReset();				
		case main_CLEAR_HIST: device_info.event_flash = EVENT_RUN; xTaskNotify(device_flashHandle, C_FLASH, eSetBits);break;
		case main_MODEL: device_set_model(wr_buf);device_info.device_en=0;break;					
		case main_MODULE: device_set_module(wr_buf);device_info.device_en=0;break;										
		case main_BULID_TIME: break;		
		case main_BULID_Version: break;
		default: device_set_next(wr_buf,main_end);return;	
	}
	if(device_info.debug_en == 1)
	{
		device_info.debug_en = 0;		
	}
	device_param_get(wr_buf[3]);
}
/****************************************************/
/****************************************************/
/****************************************************/
/****************************************************/
/*  wr_buff >> usb-buff     &device_info.data_time   */

void device_rtc_set(uint8_t *wr_buf,rtc_t *rtc_time)  //同步时钟
{
	rtc_time->year  = wr_buf[7];
	rtc_time->month = wr_buf[8];
	rtc_time->day   = wr_buf[9];
	rtc_time->hour  = wr_buf[10];	
	rtc_time->min   = wr_buf[11];
	rtc_time->sec   = wr_buf[12];
	rtc_time->week  = wr_buf[13];
	device_ack_crc("OK","");	
}

/****************************************************/
/****************************************************/
union vbat_valreg
{
char data3[4];  // [0]HH  [3] LL
float fval3;
};
union vbat_valreg vbat_f;
/****************************************************/
/****************************************************/

/****************************************************/
void VBAT_get(void)
{
	float use_vbt = 0;
	float use_rest = 0;
	float battery_data;
	
	battery_data = (float)(device_info.save.battery_cap/100*0.85);   //考虑电池无法完全放电，取电池容量的85%
	
	device_info.gps_time++;          //gps定位时间间隔1小时累加
	device_info.vabt_timer=0;
	
	nb.Vbat_data = device_nbvbat_get();	  ////  //读取电池电量  外部时钟flash中
	nb.device_restart_num = device_restart_num_get();    //重启次数读取
	
	if(nb.device_restart_num>60000)
	{
		nb.device_restart_num = 0;
		device_restart_num_set(nb.device_restart_num);
	}
		
	vbat_f.data3[3]=nb.Vbat_data>>24;
	vbat_f.data3[2]=nb.Vbat_data>>16;
	vbat_f.data3[1]=nb.Vbat_data>>8;
	vbat_f.data3[0]=nb.Vbat_data;

	if(device_info.save.tab_model  == TP280xVx_0_P)   //外部供电可充电版本
	{
		get_adc_vbat_value();
	}
	else           //		if(vbat_f.fval3>=0)  //电池电量不小于0
	{
		switch(device_info.save.tab_module)
		{
			case BC26:use_vbt = 0.172+(0.033*(float)(60/device_info.save.srv_time));   //1小时用电量
								use_rest = nb.device_restart_num*0.417/battery_data;  //重启耗去电能百分比								
								break;
			case BC95:use_vbt = 0.172+(0.033*(float)(60/device_info.save.srv_time));   //1小时用电量
								use_rest = nb.device_restart_num*0.417/battery_data;  //重启耗去电能百分比								
								break;
			case BC20:use_vbt = 0.172+(0.417*(float)(60/device_info.save.srv_time)); break;  //1小时用电量	
			case M101:use_vbt = 0.172+(0.417*(float)(60/device_info.save.srv_time)); break;  //1小时用电量
			case TP307V1_1_A: use_vbt = 0.172+(0.01*(float)(60/device_info.save.srv_time));break;   //1小时用电量												
			case LORAWAN: use_vbt = 0.172+(0.01*(float)(60/device_info.save.srv_time));break;   //1小时用电量										
			case EC25:use_vbt = 0.172+(0.584*(float)(60/device_info.save.srv_time));break;   //1小时用电量								
			case EC25_G:use_vbt = 0.172+(0.584*(float)(60/device_info.save.srv_time)); break;  //1小时用电量									
			default:use_vbt = 0.172; break;  //1小时用电量
							
		}
		vbat_f.fval3 = vbat_f.fval3+use_vbt;    //待机和工作总共耗去电能
		use_vbt = vbat_f.fval3/battery_data;   //用去电能百分比
		device_info.channel_data[0] = 100.0 - use_vbt - use_rest;

		
		if(device_info.channel_data[0] > 100)
			device_info.channel_data[0] = 100;
		else
			if(device_info.channel_data[0] < 0)
				device_info.channel_data[0] = 0;
		nb.Vbat_data=((uint32_t)vbat_f.data3[3]<<24)|((uint32_t)vbat_f.data3[2]<<16)|((uint32_t)vbat_f.data3[1]<<8)|(vbat_f.data3[0]);
		device_nbvbat_set(nb.Vbat_data);
	}
	//超限判断
	if(device_info.channel_data[0]<device_info.save.channel_down[0]  && (device_info.alarm_flag_STATUS.ch_status[0]&alarm_channel_down) != alarm_channel_down)
	{
		device_info.alarm_flag_STATUS.ch_status[0] |= alarm_channel_down;
		device_info.alarm_flag_ID.ch_status[0] = 1;
		device_info.alarming_flag++;
		Trace_log("电池电量过低\r\n");
	}
	if((device_info.alarm_flag_STATUS.ch_status[0]&alarm_channel_down) == alarm_channel_down)   //数据超下限恢复
	{
		if(device_info.channel_data[0] > (device_info.save.channel_up[0]))
		{			
			device_info.alarm_flag_STATUS.ch_status[0] &= (~alarm_channel_down);
			device_info.alarm_flag_ID.ch_status[0] = 1;
			device_info.alarming_flag++;
			Trace_log("电池电量恢复\r\n");
		}
	}
}
/****************************************************/
void device_nbvbat_set(uint32_t vbat)
{	
	sd3078_ram_set(user_ram1,vbat);
}
/****************************************************/
uint32_t device_nbvbat_get(void)
{
	 return  sd3078_ram_get(user_ram1);
//	return 0;
}
/****************************************************/
void device_restart_num_set(uint32_t restart_num)
{
	sd3078_ram_set(user_ram7,restart_num);
}
/****************************************************/
uint32_t device_restart_num_get(void)
{
	 return  sd3078_ram_get(user_ram7);
//	return 0;
}
/****************************************************/

///**
//  * @brief  保存当前写入flash地址到bak
//  * @param  写入flash的地址
//  * @retval None
//  */
void device_perv_addr_set(void)
{
	sd3078_ram_set(user_ram2,device_info.pend_addr);
}
/****************************************************/
uint32_t device_perv_addr_get(void)
{ 
	return  sd3078_ram_get(user_ram2);
//	return 0;
}
/****************************************************/
/**
  * @brief  设置断网补发次数
  * @param  待补发次数
  * @retval None
  */				
void device_pending_num_set(void )
{	  
	sd3078_ram_set(user_ram3,device_info.pend_num);
}
/****************************************************/
uint32_t device_pending_num_get(void)
{
	return  sd3078_ram_get(user_ram3);
//	return 0;
}
/****************************************************/
void device_iwdg_flag_set(uint16_t flag)
{
		sd3078_ram_set(user_ram4,flag);
}
/****************************************************/
uint32_t device_iwdg_flag__get(void)
{
	return  sd3078_ram_get(user_ram4);
//	return 0;
}
/****************************************************/
/****************************************************/
void device_set_flahaddr_addr(uint32_t flag_addr)
{
	sd3078_ram_set(user_ram8,flag_addr);
}
/****************************************************/
uint32_t device_get_flahaddr_addr(void)
{
	return  sd3078_ram_get(user_ram8);
}
/****************************************************/
/****************************************************/
void DZ_init(void)       //定制类标签初始化
{
	
	switch(device_info.save.DZ_NUM)
	{
		case 0:		break ;  //默认
//		case 1:		device_info.liaopeng_DM = 1;break ;
//		case 2:		device_info.liaopeng_IMEI = 1;break ;
//		case 3:		device_info.liulin_JDQ = 1;break ;
//		case 4:		device_info.yali_wendu = 1;break ;
//		case 5:   device_info.yali_wendu_xiumian = 1;break;
//		case 6:   device_info.shangbao_zhengxu = 1;break;
//		case 7:   device_info.NOSLEEP_MODE = 1;break;
//		case 8:   device_info.double_sensoriic = 1;break;
//		case 9:   device_info.UDP_GPS = 1;break;
//		case 10:	device_info.TCP_LP = 1;break;
//		case 11:  device_info.EC200S_GPS = 1;break;
		default :  device_info.save.DZ_NUM = 0;break ;    //
	}
}
/****************************************************/
void channel_data_init(void)    ////通道数据初始化
{
	uint8_t i;
//	device_info.channel_data[0] = 100;
//	device_info.channel_data[1] = 1000;
//	device_info.channel_data[2] = 1000;
//	device_info.channel_data[3] = 1000;
//	device_info.channel_data[4] = 100000;
//	device_info.channel_data[5] = 1000;
//	device_info.channel_data[6] = 100000;
//	device_info.channel_data[7] = 1000;
//	device_info.channel_data[8] = 100000;
//	device_info.channel_data[9] = 1000;
//	device_info.channel_data[10] = 100000;
//	device_info.channel_data[11] = 1000;
//	device_info.channel_data[12] = 100000;
//	device_info.channel_data[13] = 1000;
//	device_info.channel_data[14] = 100000;
//	device_info.channel_data[15] = 1000;
	
	for(i=0;i<9;i++)
		device_info.port_err_num[i] = device_info.save.sensorerr_num;
	for(i=0;i<16;i++)
	{
		device_info.channel_data_f[i] = 0;
		device_info.channel_data[i] = 0;
	}
}
/****************************************************/
