#include "isl1208.h"

MY_TIME sMyTime;
//const uint8_t *COMPILED_DATE=__DATE__;//获得编译日期
//const uint8_t *COMPILED_TIME=__TIME__;//获得编译时间
//const uint8_t Month_Tab[12][3]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"}; 
//uint8_t const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	

//MY_TIME sMyTime={0x13,0x04,0x13,0x14,0x10,0x05,0x03};
//static int IntToBCD(int i);//十进制转BCD
//static int BCDToInt(int bcd); //BCD转十进制
//static uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day);
// 
int IntToBCD(int i) //十进制转BCD
{
	return (((i / 10) << 4) + ((i % 10) & 0x0f));
}
int BCDToInt(int bcd) //BCD转十进制
{
	return (0xff & (bcd >> 4)) * 10 + (0xf & bcd);
}
 
//比较两个字符串指定长度的内容是否相等
//参数:s1,s2要比较的两个字符串;len,比较长度
//返回值:1,相等;0,不相等
uint8_t my_strcmp(uint8_t*s1,uint8_t*s2,uint8_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)if((*s1++)!=*s2++)return 0;
	return 1;	   
}
///使用的是24Mhz的晶振，一个机器周期就是1/(24)us
void IIC3_delay(uint8_t us)
{
		for(uint8_t i=0;i<5;i++)
	{
		__nop();__nop();__nop();
	}
}
void IIC3_init(void )
{
	  LL_GPIO_InitTypeDef GPIO_Initure;


   LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    GPIO_Initure.Pin=SDA3_PIN;
    GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
  //  GPIO_Initure.Pull=LL_GPIO_PULL_UP;          //上拉
		GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_Initure.Speed=LL_GPIO_SPEED_FREQ_HIGH;    
    LL_GPIO_Init(GPIOB,&GPIO_Initure);
   
	    GPIO_Initure.Pin=SCL3_PIN;
    GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
  //  GPIO_Initure.Pull=LL_GPIO_PULL_UP;       
		GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_Initure.Speed=LL_GPIO_SPEED_FREQ_HIGH;    
    LL_GPIO_Init(GPIOB,&GPIO_Initure);
   
	  IIC3_SDA_H();
	  IIC3_SCL_H();

}
void IIC3_Start(void)
{
		IIC3_SDA_H();
	  IIC3_SCL_H();
	  IIC3_delay(4);
	  IIC3_SDA_L();//START:when CLK is high,DATA change form high to low 
  	IIC3_delay(4);
	  IIC3_SCL_L();//钳住I2C总线，准备发送或接收数据 
}
void IIC3_Stop(void)
{
	//sda线输出
  IIC3_SCL_L();
  IIC3_SDA_L();//STOP:when CLK is high DATA change form low to high
 	IIC3_delay(4);
	IIC3_SCL_H();
	IIC3_SDA_H();//发送I2C总线结束信号
	IIC3_delay(4);		
}
void IIC3_Send_Byte(uint8_t txd)
{    uint8_t t;   
	 	    
    IIC3_SCL_L();     //拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
       // IIC_SDA=(txd&0x80)>>7;
			  if(txd&0x80)
					  IIC3_SDA_H();
				else 
					  IIC3_SDA_L();
      
				txd<<=1; 	  
		IIC3_delay(2);   //对TEA5767这三个延时都是必须的
	  IIC3_SCL_H();  
		IIC3_delay(2); 
    IIC3_SCL_L();  
		IIC3_delay(2);
    }	
}
uint8_t IIC3_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	//SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
          IIC3_SCL_L();  
        IIC3_delay(2);
    IIC3_SCL_H();  
        receive<<=1;
        if(READ_BIT(GPIOB->IDR, SDA3_PIN))//= (SDA_PIN))
					receive++;   
		IIC3_delay(1); 
    }					 
    if (!ack)
        IIC3_NAck();//发送nACK
    else
        IIC3_Ack(); //发送ACK   
    return receive;
}
uint8_t IIC3_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	//SDA_IN();      //SDA设置为输入  
	IIC3_SDA_H();IIC3_delay(1);	   
	IIC3_SCL_H();IIC3_delay(1);	 
	while(READ_BIT(GPIOB->IDR, SDA3_PIN) )//== (SDA_PIN))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC3_Stop();
			return 1;
		}
	}
	  IIC3_SCL_L();//时钟输出0 	   
	return 0;  
	
}
void IIC3_Ack(void)
{
	IIC3_SCL_L();
 IIC3_SDA_L();
 IIC3_delay(2);
 IIC3_SCL_H();
 IIC3_delay(2);
//IIC3_SDA_H();
 IIC3_SCL_L();
}
void IIC3_NAck(void)
{
  IIC3_SCL_L();
  IIC3_SDA_H();
	IIC3_delay(2);
  IIC3_SCL_H();
	IIC3_delay(2);
  IIC3_SCL_L();
	//  IIC3_SCL_H();
}


#if isl1208


void WriteREG(uint8_t adds,uint8_t datas)
{
	IIC3_Start();
	IIC3_Send_Byte(ISLWR);
	IIC3_Wait_Ack();
	
	IIC3_Send_Byte(adds);
	IIC3_Wait_Ack();
	
	IIC3_Send_Byte(datas);
	IIC3_Wait_Ack();
	
	IIC3_Stop();
	
}
uint8_t ReadREG(uint8_t adds)
{
	uint8_t reg=0;
	IIC3_Start();
	IIC3_Send_Byte(ISLWR);
	IIC3_Wait_Ack();
	
	IIC3_Send_Byte(adds);
	IIC3_Wait_Ack();
	
	IIC3_Start();
	IIC3_Send_Byte(ISLRD);
	IIC3_Wait_Ack();
	reg=IIC3_Read_Byte(0);
	IIC3_Stop();
	 return (reg) ;
}

void ISL128Init(void)
{
  uint8_t R=0;
	IIC3_init();
	
	R=ReadREG(ISL1208_USER_DAT0);
	if(R != 0xAB)
	{
			WriteREG(ISL1208_SR_REG,0X00);         //设置SR,使能RTC            1101 0000  D0
			WriteREG(ISL1208_INT_REG,0X00);  
			WriteREG(ISL1208_SR_REG,0x90);         //设置SR,使能RTC            1101 0000  D0
			WriteREG(ISL1208_INT_REG,0XE0);        //设置INT                     1110 0000  E           //A  1HZ
		//	WriteREG(ISL1208_DTR_REG,0x00);      //数字微调DTR
		//                                 	     //晶振12.5pF
			WriteREG(ISL1208_USER_DAT0,0xAB);   //初始化标记
		//	                                     //清除闹钟
			WriteREG(ISL1208_RTC_SECOND,0x00);   //初始化时间
			WriteREG(ISL1208_RTC_MINUTE,0x00);
			WriteREG(ISL1208_RTC_HOUR,0x80);     //24小时的
			WriteREG(ISL1208_RTC_DAY,0x00);
			WriteREG(ISL1208_RTC_MONTH,0x00);
			WriteREG(ISL1208_RTC_YEAR,0x00);
			WriteREG(ISL1208_RTC_WEEK,0x00);
		}
	
}
void read_time(void )
{
	sMyTime._sec=ReadREG(0x00);    ///寄存器BCD码
	sMyTime._min=ReadREG(0X01);
	sMyTime._hour=ReadREG(0X02);
	sMyTime._day=ReadREG(0x03);
	sMyTime._month=ReadREG(0x04);
	sMyTime._year=ReadREG(0x05);
	sMyTime._week=ReadREG(0x06);
 // sMyTime.buff[2]=ReadREG(ISL1208_USER_DAT0);
  sMyTime._hour=sMyTime._hour<<1;
	sMyTime._hour=sMyTime._hour>>1;
	sMyTime.buff[0]=BCDToInt(sMyTime._sec);    //十进制
	sMyTime.buff[1]=BCDToInt(sMyTime._min);
	sMyTime.buff[2]=BCDToInt(sMyTime._hour);
  sMyTime.buff[3]=BCDToInt(	sMyTime._day);
	sMyTime.buff[4]=BCDToInt(sMyTime._month);
	sMyTime.buff[5]=BCDToInt(sMyTime._year);
	sMyTime.buff[6]=BCDToInt(sMyTime._week);

}
void write_time(void)
{
		WriteREG(0x00,sMyTime._sec);
		WriteREG(0x01,sMyTime._min);	
		WriteREG(0x02,sMyTime._hour);	
		WriteREG(0x03,sMyTime._day);	
		WriteREG(0x04,sMyTime._month);
		WriteREG(0x05,sMyTime._year);
		WriteREG(0x06,sMyTime._week);
}
void Auto_Time_Set(uint8_t *buf)
{
//		sMyTime._year=IntToBCD(buf[7]);
//		sMyTime._month=IntToBCD(buf[8]);
//    sMyTime._day=IntToBCD(buf[9]);
//		sMyTime._hour=IntToBCD(buf[10]);
//		sMyTime._min=IntToBCD(buf[11]);
//		sMyTime._sec=IntToBCD(buf[12]);
//		sMyTime._week=IntToBCD(buf[13]);
//	

		sMyTime._year=IntToBCD(19);
		sMyTime._month=IntToBCD(5);
    sMyTime._day=IntToBCD(15);
		sMyTime._hour=IntToBCD(19);
		sMyTime._min=IntToBCD(51);
		sMyTime._sec=IntToBCD(0);
		sMyTime._week=IntToBCD(3);
	
		sMyTime._hour|=0x80;
	
	  write_time();

}
	
//uint8_t alarmbuff[7];
void SetAlarm(void)
{
	
//	alarmbuff[0]=IntToBCD(sca);
//	//alarmbuff[1]=IntToBCD(mna);
//	alarmbuff[0]|=0x80;
//	//alarmbuff[1]|=0x80;

	WriteREG(ISL1208_ALM_SECOND,0x81); 
	WriteREG(ISL1208_ALM_MINUTE,0X00);  
	WriteREG(ISL1208_ALM_HOUR,0x00);   
	WriteREG(ISL1208_ALM_DAY,0X00);  
	WriteREG(ISL1208_ALM_MONTH,0x00);   
	WriteREG(ISL1208_ALM_WEEK,0X00);    

//	alarmbuff[0]=ReadREG(ISL1208_ALM_SECOND);
//	alarmbuff[1]=ReadREG(ISL1208_ALM_MINUTE);
//	alarmbuff[2]=ReadREG(ISL1208_ALM_HOUR);
//	alarmbuff[3]=ReadREG(ISL1208_ALM_DAY);
//	alarmbuff[4]=ReadREG(ISL1208_ALM_MONTH);
//	alarmbuff[5]=ReadREG(ISL1208_ALM_WEEK);
//	
//	alarmbuff[6]=ReadREG(ISL1208_INT_REG);
	//WriteREG(ISL1208_ALM_MINUTE,alarmbuff[1]);   
	
}

//void ISL128Init()
//{
//	uint8_t SR_REG_DATA=0x91;
//	uint8_t INT_REG_DATA=0xca;
////	EEPROM_ADDRESS=ISL1208_ADDRESS;
//	I2C_EE_ByteWrite(&SR_REG_DATA, 0x07);
//	I2C_EE_ByteWrite(&INT_REG_DATA, 0x08);
//}
//void SetTime(pMY_TIME _mytime)
//{
//	EEPROM_ADDRESS=ISL1208_ADDRESS;
//	I2C_EE_ByteWrite(&_mytime->_sec,  0x00);
//	I2C_EE_ByteWrite(&_mytime->_min,  0x01);
//	I2C_EE_ByteWrite(&_mytime->_hour, 0x02);
//	I2C_EE_ByteWrite(&_mytime->_day,  0x03);
//	I2C_EE_ByteWrite(&_mytime->_month,0x04);
//	I2C_EE_ByteWrite(&_mytime->_year, 0x05);
//	I2C_EE_ByteWrite(&_mytime->_week, 0x06);
//}
//void GetTime(pMY_TIME _mytime)
//{
//	EEPROM_ADDRESS=ISL1208_ADDRESS;
//	I2C_EE_BufferRead(&_mytime->_sec,  0x00,1);
//	I2C_EE_BufferRead(&_mytime->_min,  0x01,1);
//	I2C_EE_BufferRead(&_mytime->_hour, 0x02,1);
//	I2C_EE_BufferRead(&_mytime->_day,  0x03,1);
//	I2C_EE_BufferRead(&_mytime->_month,0x04,1);
//	I2C_EE_BufferRead(&_mytime->_year, 0x05,1);
//	I2C_EE_BufferRead(&_mytime->_week, 0x06,1);
//	
//	_mytime->_sec=BCDToInt(_mytime->_sec);
//	_mytime->_min=BCDToInt(_mytime->_min);
//	_mytime->_hour=BCDToInt(_mytime->_hour);
//	_mytime->_day=BCDToInt(_mytime->_day);
//	_mytime->_month=BCDToInt(_mytime->_month);
//	_mytime->_year=BCDToInt(_mytime->_year);
//	_mytime->_week=BCDToInt(_mytime->_week);
//}
// 
//void Auto_Time_Set()
//{
//	uint8_t temp[3];
//	uint8_t i;
//	uint8_t mon,date,week;
//	uint8_t year;
//	uint8_t sec,min,hour;
//	for(i=0;i<3;i++)temp[i]=COMPILED_DATE[i];   
//	for(i=0;i<12;i++)if(my_strcmp((uint8_t*)Month_Tab[i],temp,3))break;	
//	mon=i+1;//得到月份
//	if(COMPILED_DATE[4]==' ')date=COMPILED_DATE[5]-'0'; 
//	else date=10*(COMPILED_DATE[4]-'0')+COMPILED_DATE[5]-'0';  
//	year=10*(COMPILED_DATE[9]-'0')+COMPILED_DATE[10]-'0';	   
//	hour=10*(COMPILED_TIME[0]-'0')+COMPILED_TIME[1]-'0';  
//	min=10*(COMPILED_TIME[3]-'0')+COMPILED_TIME[4]-'0';  
//	sec=10*(COMPILED_TIME[6]-'0')+COMPILED_TIME[7]-'0';  
//	week=RTC_Get_Week(year+2000,mon,date);
//	sMyTime._day=IntToBCD(date);
//	sMyTime._hour=IntToBCD(hour);
//	sMyTime._min=IntToBCD(min);
//	sMyTime._month=IntToBCD(mon);
//	sMyTime._sec=IntToBCD(sec);
//	sMyTime._year=IntToBCD(year);
//	sMyTime._week=IntToBCD(week);
//	SetTime(&sMyTime);
//} 
//uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day)
//{	
//	uint16_t temp2;
//	uint8_t yearH,yearL;
//	
//	yearH=year/100;	yearL=year%100; 
//	// 如果为21世纪,年份数加100  
//	if (yearH>19)yearL+=100;
//	// 所过闰年数只算1900年之后的  
//	temp2=yearL+yearL/4;
//	temp2=temp2%7; 
//	temp2=temp2+day+table_week[month-1];
//	if (yearL%4==0&&month<3)temp2--;
//	return(temp2%7);
//}
#endif 

///*仅限数值为10*/
//static void bcd2dec(MY_TIME *bcd,MY_TIME *dec)
//{
//	dec->sec =  (bcd->sec >> 4) * 10 + (bcd->sec & 0X0F);
//	dec->min =  (bcd->min >> 4) * 10 + (bcd->min & 0X0F);
//	dec->day =  (bcd->day >> 4) * 10 + (bcd->day & 0X0F);
//	dec->week = (bcd->week >> 4) * 10 + (bcd->week & 0X0F);
//	dec->year = (bcd->year >> 4) * 10 + (bcd->year & 0X0F);
//	dec->month = (bcd->month >> 4) * 10 + (bcd->month & 0X0F);
//	dec->hour = ((bcd->hour & 0X3F) >> 4) * 10 + (bcd->hour & 0X0F);
//}

//static void dec2bcd(MY_TIME *bcd,MY_TIME *dec)
//{
//	bcd->sec =  ((dec->sec/10 ) << 4)  | (dec->sec % 10);
//	bcd->min =  ((dec->min/10 ) << 4)  | (dec->min % 10);
//	bcd->day =  ((dec->day/10 ) << 4)  | (dec->day % 10);
//	bcd->week = ((dec->week/10 ) << 4) | (dec->week % 10);
//	bcd->year = ((dec->year/10 ) << 4) | (dec->year % 10);
//	bcd->month = ((dec->month/10 ) << 4) | (dec->month % 10);
//	bcd->hour = ((dec->hour/10 ) << 4) | (dec->hour % 10);
//}

//void ds1337_init(void )
//{
//	

//}

uint8_t ds1337_write(uint8_t addr, uint8_t reg_addr, uint8_t *pdata, uint16_t size)
{
	IIC3_Start();
	IIC3_Send_Byte(addr);
	if(IIC3_Wait_Ack())
	{
		IIC3_Stop();
		return 1;
	}
  IIC3_Send_Byte(reg_addr);
	if(IIC3_Wait_Ack())
	{
		IIC3_Stop();
		return 1;
	}
	while(size--)
	{
			IIC3_Send_Byte(*pdata);
		pdata++;
		if(IIC3_Wait_Ack())
		{
			IIC3_Stop();
			return 1;
		}
	}
	IIC3_Stop();
	IIC3_delay(4);
	return 0;
}
uint8_t alm_w(uint8_t addr, uint8_t reg_addr, uint8_t pdata )
{
	IIC3_Start();
	IIC3_Send_Byte(addr);
	if(IIC3_Wait_Ack())
	{
		IIC3_Stop();
		return 1;
	}
  IIC3_Send_Byte(reg_addr);
	if(IIC3_Wait_Ack())
	{
		IIC3_Stop();
		return 1;
	}

	IIC3_Send_Byte(pdata);

		if(IIC3_Wait_Ack())
		{
			IIC3_Stop();
			return 1;
		}
	
	IIC3_Stop();
	IIC3_delay(4);
	return 0;	

}

void ds1337_read(uint8_t addr, uint8_t regaddr,uint8_t *reg_value, uint16_t size)
{
		uint16_t read_pos = 0;
	IIC3_Start();
	IIC3_Send_Byte(addr|0x00);
	if(IIC3_Wait_Ack())
	{
		IIC3_Stop();

	}
  IIC3_Send_Byte(regaddr);
	if(IIC3_Wait_Ack())
	{
		IIC3_Stop();
	}
	
		IIC3_Start();
	IIC3_Send_Byte(addr|0x01);
	if(IIC3_Wait_Ack())
	{
		IIC3_Stop();
	}
 	for( ;read_pos < size - 1; read_pos++)
	{
		reg_value[read_pos] = IIC3_Read_Byte(1);
	}
	reg_value[read_pos] = IIC3_Read_Byte(0);
	
	IIC3_Stop();
	
}

uint8_t ds1337_settime(rtc_t *buf)
{	
	uint8_t time_buf[8];
	uint8_t i=0;

//		sMyTime.year=  buf->year;
//		sMyTime.month= buf->month;
//    sMyTime.day=   buf->day;
//		sMyTime.hour=  buf->hour ;
//		sMyTime.min= 	 buf->min;
//		sMyTime.sec=   buf->sec ;
//		sMyTime.week=  buf->week ;

//		time_buf[6]=IntToBCD(sMyTime.year);
//		time_buf[5]=IntToBCD(sMyTime.month);
//    time_buf[4]=IntToBCD(sMyTime.day);
//		time_buf[3]=IntToBCD(sMyTime.week);
//		
//		time_buf[2]=IntToBCD(sMyTime.hour)&0x3f ;
//		time_buf[1]=IntToBCD(sMyTime.min);
//		time_buf[0]=IntToBCD(sMyTime.sec);
//	
		time_buf[6]=IntToBCD(buf->year);
		time_buf[5]=IntToBCD(buf->month);
    time_buf[4]=IntToBCD(buf->day);
		time_buf[3]=IntToBCD(buf->week );
		
		time_buf[2]=IntToBCD( buf->hour)&0x3f ;
		time_buf[1]=IntToBCD(buf->min);
		time_buf[0]=IntToBCD(buf->sec );
	

	for(i=0;i<7;i++ )
	{
			ds1337_write(DS1337_ADDR, i, &time_buf[i], 1);
	}
//		ds1337_write(DS1337_ADDR, YEAR, &time_buf[6], 1);
//		ds1337_write(DS1337_ADDR, MONTH, &time_buf[5], 1);
//		ds1337_write(DS1337_ADDR, DATE, &time_buf[4], 1);
//		ds1337_write(DS1337_ADDR, DAY, &time_buf[3], 1);
//		ds1337_write(DS1337_ADDR, HOUR, &time_buf[2], 1);
//		ds1337_write(DS1337_ADDR, MINUTES, &time_buf[1], 1);
//		ds1337_write(DS1337_ADDR, SECONDS, &time_buf[0], 1);
return 0;
}

uint8_t ds1337_gettime(time_p p)
{
	
//	MY_TIME bcd_rtc;
//	MY_TIME *temp_rtc = &bcd_rtc;

	uint8_t almf;

	ds1337_read(DS1337_ADDR ,SECONDS, &(p->sec), 1);
	ds1337_read(DS1337_ADDR ,MINUTES, &(p->min), 1);
	ds1337_read(DS1337_ADDR ,HOUR, &(p->hour), 1);
	ds1337_read(DS1337_ADDR ,DATE, &(p->day), 1);
	ds1337_read(DS1337_ADDR ,DAY, &(p->week), 1);
	ds1337_read(DS1337_ADDR ,MONTH, &(p->month), 1);
	ds1337_read(DS1337_ADDR ,YEAR, &(p->year), 1);
	
//	bcd2dec(&bcd_rtc, DateTime); 
//	p->hour=p->hour&0x3f;
	sMyTime.buff[0]=BCDToInt(sMyTime.sec);    //十进制
	sMyTime.buff[1]=BCDToInt(sMyTime.min);
	sMyTime.buff[2]=BCDToInt(sMyTime.hour);
	
  sMyTime.buff[3]=BCDToInt(sMyTime.day);
	sMyTime.buff[4]=BCDToInt(sMyTime.month);
	sMyTime.buff[5]=BCDToInt(sMyTime.year);
	sMyTime.buff[6]=BCDToInt(sMyTime.week);
	
	ds1337_read(DS1337_ADDR, STATUS, &almf, 1);
	
	alm_w(DS1337_ADDR, STATUS, 0X00);
	return 0;
}


uint8_t setalarm()
{
//	uint8_t bufalm[4];
//  bufalm[0]=IntToBCD(A2M2);
//	bufalm[1]=IntToBCD(A2M3);
//	bufalm[2]=IntToBCD(A2M4);

//	bufalm[3]=IntToBCD(0x06);
//	
//	alm_w(DS1337_ADDR,A2MINUTES,bufalm[0]);	
//	alm_w(DS1337_ADDR,A2HOURS,bufalm[1]);
//	alm_w(DS1337_ADDR,A2DAY,bufalm[2]);
//	alm_w(DS1337_ADDR,CONTROL,bufalm[3]);
//	alm_w(DS1337_ADDR, STATUS, 0X00);
	uint8_t bufalm[2];
	bufalm[0]=IntToBCD(0X80);
	bufalm[1]=IntToBCD(0x06);
	
	alm_w(DS1337_ADDR,A2MINUTES,bufalm[0]);	
	alm_w(DS1337_ADDR,A2HOURS,bufalm[0]);
	alm_w(DS1337_ADDR,A2DAY,bufalm[0]);
	alm_w(DS1337_ADDR,CONTROL,bufalm[1]);
	alm_w(DS1337_ADDR, STATUS, 0X00);
	return 0;
}

	uint8_t barcmin=0;

void UserCallback( void )
{
   barcmin ++;
	if(barcmin >255)
		barcmin =0;
}

///*设置闹钟按分秒来报警，减少过多判断*/
//uint8_t ISL1208_SetAlarm(uint8_t Hour,uint8_t Min,uint8_t Sec,uint8_t AlarmFlag)
//{
//	uint8_t rtc_buf[8];
////	
////	ds1337_get_time(rtc); /*设置好时分秒*/
////	rtc->rtc_min += min;
////		
////////	ds1337_set_sr(0X00);
//		ds1337_write(DS1337_ADDR, STATUS, 0X00, 1);
//		//iic_read(DS1337_ADDR, CONTROL, &sr_reg, 1);
////////	ds1337_get_sr();	

////	if(rtc->rtc_min >= 60)
////	{
////		rtc->rtc_min -= 60;
////	}
////	dec2bcd(&bcd_rtc, rtc);
////	
////	rtc_buf[0] = bcd_rtc.rtc_sec;
////	rtc_buf[1] = bcd_rtc.rtc_min;
////	rtc_buf[2] = bcd_rtc.rtc_hour | A1M3;
////	rtc_buf[3] = bcd_rtc.rtc_date  | A1M4;
////	if(iic_write(DS1337_ADDR, A1SECONDS, rtc_buf, 4) )
////	{
////		return 1;
////	}	
////ISL1208_WriteReg(CONTROL, 0x00);
//	rtc_buf[0] = 0x00;//INTEN | A2IE;                  /*启动闹钟1并且开启中断*/
//	if(ds1337_write(DS1337_ADDR, CONTROL, rtc_buf, 1))
//	{
//		return 1;
//	}
//	return 0;
//}
