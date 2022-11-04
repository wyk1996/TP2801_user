#include "LCDdisplay.h"
#include "dianwei.h"


//TP1622  HT1622    VK0256B 兼容模式

#define   LCD_PORT  GPIOB
#define   LCD_CS_PIN       LL_GPIO_PIN_12
#define   LCD_WR_PIN       LL_GPIO_PIN_13
#define   LCD_DA_PIN       LL_GPIO_PIN_14

#define add_len   32
#define ziduan    256
uint8_t dis_buf[add_len];
uint16_t LCD_buf[ziduan];
xTaskHandle device_LCDHandle;



/***********************************************************************************/
static void LCD_PIN_init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = LCD_CS_PIN|LCD_WR_PIN|LCD_DA_PIN;     //
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = ENABLE;
	LL_GPIO_Init(LCD_PORT, &GPIO_InitStruct);
	LL_GPIO_SetOutputPin(LCD_PORT, LCD_CS_PIN|LCD_WR_PIN|LCD_DA_PIN);  
}
/***********************************************************************************/
static void LCD_bits_send(uint8_t comm, uint8_t size)
{
  uint8_t mask = 0x01;
	LL_GPIO_ResetOutputPin(LCD_PORT, LCD_WR_PIN); 
  for(uint16_t i = 0; i < size; i++)
	{
		if(comm & mask)			
			LL_GPIO_SetOutputPin(LCD_PORT, LCD_DA_PIN);
		else 
			LL_GPIO_ResetOutputPin(LCD_PORT, LCD_DA_PIN);
		delay_10pus(1);
		LL_GPIO_SetOutputPin(LCD_PORT, LCD_WR_PIN); 
		delay_10pus(1);
		LL_GPIO_ResetOutputPin(LCD_PORT, LCD_WR_PIN); 
		mask <<= 1;
	}
}
/***********************************************************************************/
static void LCD_port_comm_write(uint8_t cmd)
{
	LL_GPIO_ResetOutputPin(LCD_PORT, LCD_CS_PIN); 
	delay_10pus(20);
	LCD_bits_send(0x01, 3);
	LCD_bits_send(cmd, 8);
	delay_10pus(20);
	LL_GPIO_SetOutputPin(LCD_PORT, LCD_CS_PIN); 
}
/***********************************************************************************/
static void LCD_port_data_write(uint8_t addr, uint8_t *sdata, uint16_t size)    //数据写入
{
	uint8_t i;
	LL_GPIO_ResetOutputPin(LCD_PORT, LCD_CS_PIN); 
	delay_10pus(20);
	LCD_bits_send(0x05, 3);
	LCD_bits_send(addr, 6);
	for(i = 0; i < size; i++) {
		LCD_bits_send(sdata[i], 8);
	}
	delay_10pus(50);
	
	LL_GPIO_SetOutputPin(LCD_PORT, LCD_WR_PIN|LCD_DA_PIN);
	delay_10pus(50);	
	LL_GPIO_SetOutputPin(LCD_PORT, LCD_CS_PIN);
	delay_10pus(50);
}
/***********************************************************************************/
static void data_to_buf(uint8_t len)    //字段数组转换为显示数据
{
	uint8_t i;
	uint8_t TM1622_BF_H,TM1622_BF_L;
	uint16_t TM1622_BF;
	
	for(i=0;i<len;i++) {
		TM1622_BF = LCD_buf[i];
		TM1622_BF_L = TM1622_BF;
		TM1622_BF_H = (TM1622_BF>>8);
		dis_buf[TM1622_BF_H] |= TM1622_BF_L;
	}
}
/***********************************************************************************/
static void LCD_dis_all(void)    //全显
{
	memset(dis_buf,0xff,sizeof(dis_buf));
	LCD_port_data_write(0x00,dis_buf,add_len);
}
/***********************************************************************************/
void LCD_close_all(void)    //全灭
{
	memset(dis_buf,0x00,sizeof(dis_buf));
	LCD_port_data_write(0x00,dis_buf,add_len);
}
/***********************************************************************************/
void LCD_CLOSE(void)
{
   Trace_log("关闭液晶显示\r\n");
   LCD_port_comm_write(CMD_LCD_OFF);
}
/***********************************************************************************/
static void LCD_init(void)
{
	LCD_PIN_init();
	vTaskDelay(200);
	
	LCD_port_comm_write(CMD_SYS_EN);
	LCD_port_comm_write(CMD_RC_INT);
	LCD_port_comm_write(CMD_LCD_OFF);
	LCD_port_comm_write(CMD_LCD_ON);   
}
/***********************************************************************************/
static uint16_t shuzi_display(uint8_t dis_dat,uint8_t dat_cnt)   //测试程序部分 数字字段显示
{
	uint8_t i,j;
	uint8_t dat_value;
			
	for(j=0;j<28;j++)
	{
		dat_value = Num_0_9[dis_dat];
		for(i=0;i<8;i++)
		{
			if(dat_value&0x80)
			  LCD_buf[dat_cnt++] = Num_A[j][i];
			dat_value<<=1;
		}
	}
	return dat_cnt;
}

#if 0
static uint16_t else_display(uint8_t dis_dat,uint8_t dat_cnt)  //测试程序部分 符号等其他数据显示
{
	uint8_t j;
			
	for(j=0;j<6;j++)
	{
	 LCD_buf[dat_cnt++] = test_buf[dis_dat][j];
	}
	return dat_cnt;
}

static void LCD_TEST(void)    //显示测试部分
{
	uint8_t i;
	uint8_t dis_len;
	
	LCD_init();	
	for(i=0;i<12;i++)
	{		
		memset(dis_buf,0,sizeof(dis_buf));
		memset(LCD_buf,0,sizeof(LCD_buf));
		dis_len = shuzi_display(i,0);
//		dis_len = else_display(i,dis_len);
//		dis_len = dis_len + dis_len1;
		data_to_buf(dis_len);
		LCD_port_data_write(0x00,dis_buf,add_len);
		vTaskDelay(1000);
	}
	for(i=0;i<4;i++)
	{		
		memset(dis_buf,0,sizeof(dis_buf));
		memset(LCD_buf,0,sizeof(LCD_buf));
//		dis_len = shuzi_display(i,0);
		dis_len = else_display(i,0);
//		dis_len = dis_len + dis_len1;
		data_to_buf(dis_len);
		LCD_port_data_write(0x00,dis_buf,add_len);
		vTaskDelay(1000);
	}
	
}
#endif
/***********************************************************************************/
/*导入变量  
参数1 data：需要显示的数字
参数2 data_add：需要显示的数字位置
参数3 sadd：显示数组的起始地址
返回：显示数组的地址
*/
static uint8_t dis_shuzi(uint8_t data,uint8_t data_add,uint8_t sadd)
{
	uint8_t addr,i,dat_value;
	addr = sadd;
	dat_value = Num_0_9[data];
	for(i=0;i<7;i++)
	{
		if(dat_value&0x80)
			LCD_buf[addr++] = Num_A[data_add][i];
		dat_value<<=1;
	}
	return addr;
}

static uint8_t Number_Display(uint8_t data,uint8_t data_add,uint8_t sadd)
{
	uint8_t addr,i,dat_value;
	addr = sadd;
	dat_value = Num_0_9[data];
	for(i=0;i<7;i++)
	{
		if(dat_value&0x80)
			LCD_buf[addr++] = Number_BUF[data_add][i];
		dat_value<<=1;
	}
	return addr;
}
/***********************************************************************************/
static uint8_t chaoxain(uint8_t dis_add,uint8_t dis_lan,uint8_t sadd)   //数据超限   显示 ---
{
	uint8_t i;
	for(i=0;i<dis_lan;i++)
	{
		sadd = dis_shuzi(10,dis_add-i,sadd);
	}
	return sadd;
}

static uint8_t Ultralimit_Handle(uint8_t dis_add,uint8_t dis_lan,uint8_t sadd)   //数据超限   显示 ---
{
	uint8_t i;
	for(i=0;i<dis_lan;i++)
	{
		sadd = Number_Display(10,dis_add-i,sadd);
	}
	return sadd;
}



static uint8_t Display_Sensor_Handle(uint8_t add,float data,uint8_t sadd,uint8_t dip)
{
	uint8_t d1,i,st_add;
	uint32_t value_temp;
	uint8_t value_num[5];
	uint8_t dip2,fushu;
	
	dip2 = 0;
	fushu = 0;
	if(add == 0 || add == 2 || add == 4)
	{
		st_add = 24-(add*4);		
		if(data>=0)
		{
			if(dip>4)
				dip = 4;
			if(data>99999)
			{
				sadd = Ultralimit_Handle(st_add,5,sadd);
				return sadd;
			}
			while(dip--)   //找出小数点位数和数据进行放大   这里会多放大10倍    后边做四舍五入
			{				
				if(data>9999)
				{
					break;
				}
				else
				{
					data = data*10;
					dip2++;
				}
			}	
		}
		else    //负数
		{
			fushu = 1;
			if(dip>3)
				dip = 3;
			data = -data;
			if(data>9999)
			{

				sadd = Ultralimit_Handle(st_add,5,sadd);
				return sadd;
			}
			while(dip--) //找出小数点位数和数据进行放大
			{			
				if(data>999)
				{
					break;
				}
				else
				{
					data = data*10;
					dip2++;
				}
			}			
		}
		LCD_buf[sadd++] = Number_BUF[st_add-dip2][7];		  //小数点显示
	}
	else    //1 3 5位置
	{
		st_add = 27-((add-1)*4);		
		if(data>=0)
		{
			if(dip>2)
				dip = 2;
			if(data>999)
			{
				sadd = Ultralimit_Handle(st_add,3,sadd);
				return sadd;
			}
			while(dip--)  //找出小数点位数和数据进行放大
			{				
				if(data>99)
				{
					break;
				}
				else
				{
					data = data*10;
					dip2++;
				}
			}			
		}
		else    //负数
		{
			fushu = 1;
			if(dip>1)
				dip = 1;
			data = -data;
			if(data>99)
			{
				sadd = Ultralimit_Handle(st_add,3,sadd);
				return sadd;
			}
			while(dip--)  //找出小数点位数和数据进行放大
			{				
				if(data>9)
				{
					break;
				}
				else
				{
					data = data*10;
					dip2++;
				}					
			}			
		}
		LCD_buf[sadd++] = Number_BUF[st_add-dip2][7];   //小数点显示
	}
	
	data = data*10;
	value_temp = (uint32_t)data;  //数据取整
	d1 = value_temp%10;   //四舍五入位
	value_temp = value_temp/10;
	if(d1>=5)
		value_temp = value_temp+1;
	value_num[4]=	value_temp/10000;
	value_num[3]= value_temp%10000/1000;
	value_num[2]=	value_temp%1000/100;
	value_num[1]= value_temp%100/10;
	value_num[0]= value_temp%10;
	
	for(i=4;i>0;i--)
	{
		if(value_num[i] == 0)
		{
			if(dip2 == i)
				break;
			else
				value_num[i] = 20;
		}
		else
			break;
	}
	if(fushu == 1)
	{
		value_num[i+1] = 10;
	}
	
	for(i=0;i<5;i++)
	{
		if(value_num[i]<20)
			sadd = Number_Display(value_num[i],st_add-i,sadd);
	}
	
	return sadd;
}


static void Display_Sensor(uint8_t ss)
{
	uint8_t dis_len;
	uint8_t i;
	uint8_t da1;
	uint8_t sen_unit[6];
	float sen_da[6]; 
	
	memset(dis_buf,0,sizeof(dis_buf));
	memset(LCD_buf,0,sizeof(LCD_buf));
	
	dis_len = 0;
	for(i=0;i<2;i++)    //常亮部分显示
	{
		LCD_buf[dis_len++] = CL_TB[i];
	}
	//时钟部分显示	
	da1 = device_info.data_time.hour/10;   //时
	dis_len = Number_Display(da1,0,dis_len);
	da1 = device_info.data_time.hour%10;	
	dis_len = Number_Display(da1,1,dis_len);
	
	da1 = device_info.data_time.min/10;   //分
	dis_len = Number_Display(da1,2,dis_len);
	da1 = device_info.data_time.min%10;	
	dis_len = Number_Display(da1,3,dis_len);
	
	/***************充电标志显示*******************/ 
	 if(device_info.save.tab_model  == TP280xVx_0_P)   //外部供电可充电版本
	 {
		 if(device_info.waibu_pwr_24 == 1 )//|| device_info.waibu_pwr_usb == 1
			LCD_buf[dis_len++] = LCD_BZ_PW;
	 }
/***************GPS状态指示图标显示*******************/ 
	 if(device_info.gps_satues == 1)
		LCD_buf[dis_len++] = LCD_BZ_GPS;
	
//	***************电量显示******************* 	
	if(device_info.channel_data[0]>=20&&device_info.channel_data[0] <40)
		da1 = 1;			 
	else if(device_info.channel_data[0]>=40&&device_info.channel_data[0] <60)
		da1 = 2;
	else if(device_info.channel_data[0]>=60&&device_info.channel_data[0] <80)
		da1 = 3;
	else if(device_info.channel_data[0]>=80)
	 	da1 = 4;
	else
	 	da1 = 0;			
	for(i=0;i<da1;i++)
	 	LCD_buf[dis_len++] = BAT_BF[i];
	 
	 //信号强度显示  signal_DB	
	if(device_info.save.tab_module == TP307V1_1_A || device_info.save.tab_module == LORAWAN )  //lora
	{		
		 if(nb.nb_csq>70&&nb.nb_csq <90)   //信号
		 {
				da1 = 1;   
		 }
		 else if(nb.nb_csq>=90&&nb.nb_csq <120)
		 {
			 da1 = 2;
		 }
		 else if(nb.nb_csq>=120&&nb.nb_csq <150)
		 {	    
			 da1 = 3;
		 }
		 else if(nb.nb_csq>=150 && nb.nb_csq <200)
		 {
			 da1 = 4;
		 }
     else
			 da1 = 0;
		 for(i=0;i<da1;i++)
		 {
			 LCD_buf[dis_len++] = single_DB[i];
		 }			 
	 }
	 else
	 {		
		 if(nb.nb_csq>0&&nb.nb_csq <9)    //信号
		 {
				da1 = 1;
		 }
		 else if(nb.nb_csq>=9&&nb.nb_csq <17)
		 {
				da1 = 2;
		 }
		 else if(nb.nb_csq>=17&&nb.nb_csq <24)
		 {
			 da1 = 3;
		 }
		 else if(nb.nb_csq>=24)
		 {
			 da1 = 4;
		 }
		 else
			 da1 = 0;
		 for(i=0;i<da1;i++)
		 {
			 LCD_buf[dis_len++] = single_DB[i];
		 }
	 }
	
	
	//实时数据显示	
	if(ss>2)
		ss = 0;
	if(ss == 0)
	{
		for(i=0;i<6;i++)
		{
			sen_da[i] = device_info.channel_data[4+i];  
			sen_unit[i] = device_info.save.channel_unit[4+i];
		}
	}
	else if(ss == 1)
	{
		for(i=0;i<6;i++)
		{
			sen_da[i] = device_info.channel_data[10+i];  
			sen_unit[i] = device_info.save.channel_unit[10+i];
		}		
	}
	else 
	{
		sen_da[0] = 100000;    
		sen_unit[0] = UNIT_0;
		sen_da[1] = device_info.channel_data[1];  
		sen_unit[1] = device_info.save.channel_unit[1];
		sen_da[2] = 100000;
		sen_unit[2] = UNIT_0;
		sen_da[3] = device_info.channel_data[2];  
		sen_unit[3] = device_info.save.channel_unit[2];
		sen_da[4] = 100000;
		sen_unit[4] = UNIT_0;
		sen_da[5] = device_info.channel_data[3];  
		sen_unit[5] = device_info.save.channel_unit[3];
	}
	
	for(i=0;i<6;i++)
	{
		dis_len = Display_Sensor_Handle(i,sen_da[i],dis_len,device_info.save.dip_num);
		switch(sen_unit[i])
		{
			case UNIT_MPa:		LCD_buf[dis_len++] = DIS_Buf[i][1];break ;
			case UNIT_KPa:		LCD_buf[dis_len++] = DIS_Buf[i][0];;break ;		
			case UNIT_C:      	LCD_buf[dis_len++] = DIS_Buf[i][0];break ;
			case UNIT_F:     	LCD_buf[dis_len++] = DIS_Buf[i][1];break ;
			default : break ;
		}
	}
		
	data_to_buf(dis_len);
	LCD_port_data_write(0x00,dis_buf,add_len);
}

/***********************************************************************************/
/*导入变量 
参数1 add：需要位置
参数2 data：需要显示的数据
参数3 sadd：显示数组的起始地址
参数4 dip: 显示小数点位数
返回：显示数组的地址
*/
static uint8_t dis_sensor_da(uint8_t add,float data,uint8_t sadd,uint8_t dip)
{
	uint8_t d1,i,st_add;
	uint32_t value_temp;
	uint8_t value_num[5];
	uint8_t dip2,fushu;
	
	dip2 = 0;
	fushu = 0;
	if(add == 0 || add == 2 || add == 4)
	{
		st_add = 24-(add*4);		
		if(data>=0)
		{
			if(dip>4)
				dip = 4;
			if(data>99999)
			{
//				data = 99999;
				sadd = chaoxain(st_add,5,sadd);
				return sadd;
			}
			while(dip--)   //找出小数点位数和数据进行放大   这里会多放大10倍    后边做四舍五入
			{				
				if(data>9999)
				{
					break;
				}
				else
				{
					data = data*10;
					dip2++;
				}
			}	
		}
		else    //负数
		{
			fushu = 1;
			if(dip>3)
				dip = 3;
			data = -data;
			if(data>9999)
			{
	//			data = 9999;
				sadd = chaoxain(st_add,5,sadd);
				return sadd;
			}
			while(dip--) //找出小数点位数和数据进行放大
			{			
				if(data>999)
				{
					break;
				}
				else
				{
					data = data*10;
					dip2++;
				}
			}			
		}
		LCD_buf[sadd++] = Num_A[st_add-dip2][7];		  //小数点显示
	}
	else    //1 3 5位置
	{
		st_add = 27-((add-1)*4);		
		if(data>=0)
		{
			if(dip>2)
				dip = 2;
			if(data>999)
			{
				sadd = chaoxain(st_add,3,sadd);
				return sadd;
			}
			while(dip--)  //找出小数点位数和数据进行放大
			{				
				if(data>99)
				{
					break;
				}
				else
				{
					data = data*10;
					dip2++;
				}
			}			
		}
		else    //负数
		{
			fushu = 1;
			if(dip>1)
				dip = 1;
			data = -data;
			if(data>99)
			{
				sadd = chaoxain(st_add,3,sadd);
				return sadd;
			}
			while(dip--)  //找出小数点位数和数据进行放大
			{				
				if(data>9)
				{
					break;
				}
				else
				{
					data = data*10;
					dip2++;
				}					
			}			
		}
		LCD_buf[sadd++] = Num_A[st_add-dip2][7];   //小数点显示
	}
	
	data = data*10;
	value_temp = (uint32_t)data;  //数据取整
	d1 = value_temp%10;   //四舍五入位
	value_temp = value_temp/10;
	if(d1>=5)
		value_temp = value_temp+1;
	value_num[4]=	value_temp/10000;
	value_num[3]= value_temp%10000/1000;
	value_num[2]=	value_temp%1000/100;
	value_num[1]= value_temp%100/10;
	value_num[0]= value_temp%10;
	
	for(i=4;i>=0;i--)
	{
		if(value_num[i] == 0)
		{
			if(dip2 == i)
				break;
			else
				value_num[i] = 20;
		}
		else
			break;
	}
	if(fushu == 1)
	{
		value_num[i+1] = 10;
	}
	
	for(i=0;i<5;i++)
	{
		if(value_num[i]<20)
			sadd = dis_shuzi(value_num[i],st_add-i,sadd);
	}
	
	return sadd;
}
/***********************************************************************************/
/***********************************************************************************/
static void dis_sensor(uint8_t ss)
{
	uint8_t dis_len;
	uint8_t i;
	uint8_t da1;
	uint8_t sen_unit[6];
	float sen_da[6]; 
	
	memset(dis_buf,0,sizeof(dis_buf));
	memset(LCD_buf,0,sizeof(LCD_buf));
	
	dis_len = 0;
	for(i=0;i<2;i++)    //常亮部分显示
	{
		LCD_buf[dis_len++] = CL_TB[i];
	}
	//时钟部分显示	
	da1 = device_info.data_time.hour/10;   //时
	dis_len = dis_shuzi(da1,0,dis_len);
	da1 = device_info.data_time.hour%10;	
	dis_len = dis_shuzi(da1,1,dis_len);
	
	da1 = device_info.data_time.min/10;   //分
	dis_len = dis_shuzi(da1,2,dis_len);
	da1 = device_info.data_time.min%10;	
	dis_len = dis_shuzi(da1,3,dis_len);
	
	/***************充电标志显示*******************/ 
	 if(device_info.save.tab_model  == TP280xVx_0_P)   //外部供电可充电版本
	 {
		 if(device_info.waibu_pwr_24 == 1 )//|| device_info.waibu_pwr_usb == 1
			 LCD_buf[dis_len++] = LCD_BZ_PW;
	 }
/***************GPS状态指示图标显示*******************/ 
	 if(device_info.gps_satues == 1)
		 LCD_buf[dis_len++] = LCD_BZ_GPS;
	
//	***************电量显示******************* 	
	if(device_info.channel_data[0]>=20&&device_info.channel_data[0] <40)
	 da1 = 1;			 
	else if(device_info.channel_data[0]>=40&&device_info.channel_data[0] <60)
	 da1 = 2;
	else if(device_info.channel_data[0]>=60&&device_info.channel_data[0] <80)
	 da1 = 3;
	else if(device_info.channel_data[0]>=80)
	 da1 = 4;
	else
	 da1 = 0;			
	for(i=0;i<da1;i++)
	 LCD_buf[dis_len++] = BAT_BF[i];
	 
	 //信号强度显示  signal_DB	
	if(device_info.save.tab_module == TP307V1_1_A || device_info.save.tab_module == LORAWAN )  //lora
	{		
		 if(nb.nb_csq>70&&nb.nb_csq <90)   //信号
		 {
				da1 = 1;   
		 }
		 else if(nb.nb_csq>=90&&nb.nb_csq <120)
		 {
			 da1 = 2;
		 }
		 else if(nb.nb_csq>=120&&nb.nb_csq <150)
		 {	    
			 da1 = 3;
		 }
		 else if(nb.nb_csq>=150 && nb.nb_csq <200)
		 {
			 da1 = 4;
		 }
     else
			 da1 = 0;
		 for(i=0;i<da1;i++)
		 {
			 LCD_buf[dis_len++] = single_DB[i];
		 }			 
	 }
	 else
	 {		
		 if(nb.nb_csq>0&&nb.nb_csq <9)    //信号
		 {
				da1 = 1;
		 }
		 else if(nb.nb_csq>=9&&nb.nb_csq <17)
		 {
				da1 = 2;
		 }
		 else if(nb.nb_csq>=17&&nb.nb_csq <24)
		 {
			 da1 = 3;
		 }
		 else if(nb.nb_csq>=24)
		 {
			 da1 = 4;
		 }
		 else
			 da1 = 0;
		 for(i=0;i<da1;i++)
		 {
			 LCD_buf[dis_len++] = single_DB[i];
		 }
	 }
	
	
	//实时数据显示	
	if(ss>2)
		ss = 0;
	if(ss == 0)
	{
		for(i=0;i<6;i++)
		{
			sen_da[i] = device_info.channel_data[4+i];   //
			sen_unit[i] = device_info.save.channel_unit[4+i];
		}
	}
	else if(ss == 1)
	{
		for(i=0;i<6;i++)
		{
			sen_da[i] = device_info.channel_data[10+i];  //
			sen_unit[i] = device_info.save.channel_unit[10+i];
		}		
	}
	else 
	{
		sen_da[0] = 100000;    //100000
		sen_unit[0] = UNIT_0;
		sen_da[1] = device_info.channel_data[1];   //
		sen_unit[1] = device_info.save.channel_unit[1];
		sen_da[2] = 100000;
		sen_unit[2] = UNIT_0;
		sen_da[3] = device_info.channel_data[2];  //
		sen_unit[3] = device_info.save.channel_unit[2];
		sen_da[4] = 100000;
		sen_unit[4] = UNIT_0;
		sen_da[5] = device_info.channel_data[3];  //
		sen_unit[5] = device_info.save.channel_unit[3];
	}
	
	for(i=0;i<6;i++)
	{
		dis_len = dis_sensor_da(i,sen_da[i],dis_len,device_info.save.dip_num);
		switch(sen_unit[i])
		{
			case UNIT_MPa:		LCD_buf[dis_len++] = DW_unit[i][1];break ;
			case UNIT_KPa:		LCD_buf[dis_len++] = DW_unit[i][0];;break ;		
			case UNIT_C:      LCD_buf[dis_len++] = DW_unit[i][0];break ;
			case UNIT_F:      LCD_buf[dis_len++] = DW_unit[i][1];break ;
			default : break ;
		}
	}
		
	data_to_buf(dis_len);
	LCD_port_data_write(0x00,dis_buf,add_len);
}
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
void wait_time(void)
{	
	if( device_info.save.SENSOR_SAM_TIME>250 || device_info.save.SENSOR_SAM_TIME<1)    //数据采样时间重置
		device_info.save.SENSOR_SAM_TIME = 6;
		
	if( device_info.sensor_sam_timer == 2 )   //达到上报时间间隔
	{   
		Trace_log("TOA\r\n");		 
		device_info.sensor_sam_timer = 0;				 
		device_info.event_flash = EVENT_RUN;			
		device_info.event_up = EVENT_RUN;		

		xTaskNotify(device_flashHandle, W_FLASH, eSetBits);	
		xTaskNotify(sever_nbHandle,NET_IT, eSetBits);			 
	}
	else
	if(device_info.save.device_dabao == ON && device_info.sensor_sam_timer == 1 )   //打包发送开启且达到数据采样记录时间
	{	
		Trace_log("数据累计存储....\r\n");
		device_info.sensor_sam_timer = 0;		
		device_info.event_flash = EVENT_RUN;		
		xTaskNotify(device_flashHandle, W_FLASH, eSetBits);	//存储数据						 
		if(	device_info.pend_num ==0)               ///
		{	
				device_info.pend_addr = device_info.save_addr;//device_get_hist_num_addr();
				device_perv_addr_set();
		}
		device_info.pend_num =device_info.pend_num + 1;
		device_pending_num_set();				
	}
	else if(device_info.alarming_flag!=0)
	{
		Trace_log("报警触发上报\r\n");
		if(device_info.event_up == EVENT_OK)
		{
			device_info.event_up=EVENT_RUN;
			device_info.event_flash=EVENT_RUN;
			xTaskNotify(device_flashHandle, W_FLASH, eSetBits);
			xTaskNotify(sever_nbHandle, NET_IT, eSetBits);
		}	
	}
}
/***********************************************************************************/
void LCD_Task(void *pvParameters)
{
	#if 0
	uint8_t sec=0;
	#endif
	
	LCD_init();
	LCD_dis_all();
	vTaskDelay(1000);
	LCD_close_all();
	vTaskDelay(150);
	for(;;)
	{
		device_info.event_display = EVENT_RUN;		
		RTC_ReadDate(&device_info.data_time);
		Trace_log("显示刷新PT100\r\n");
		dis_sensor(2);
		vTaskDelay(1000);
		if(device_info.save.port_num>0)
		{
			Trace_log("显示刷新pag1\r\n");
			Display_Sensor(0);
			vTaskDelay(1000);
		}
		if(device_info.save.port_num>3)
		{
			Trace_log("显示刷新pag2\r\n");
			dis_sensor(1);
		}
		device_info.event_display = EVENT_OK;
		xTaskNotifyWait(0X00, 0XFFFFFFFF, 0, portMAX_DELAY);     //portMAX_DELAY
	}
}
/***********************************************************************************/
void LCD_config_create_task(void *params, uint8_t priority)
{
    xTaskCreate(LCD_Task, "LCD", 256, params, priority, &device_LCDHandle); 
}
/***********************************************************************************/

