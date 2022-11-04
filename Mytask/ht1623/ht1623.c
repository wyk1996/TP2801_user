//#include "ht1623.h"

//#include "spi2.h"


//#include "isl1208.h"
#include "Includes.h"
#include "bihua.h"
extern device_t device_info;
extern int16_t sennsor_mode_flag;
uint8_t Dis_abc[160];  
uint8_t Num_128[20];//VK1072B里面72个笔画对应数组，数组有31个，多加一个。只用低四位 

/************************************************************************************/
void SendBit_1621(uint8_t data,uint8_t cnt) /*0x80*/
{  

  bsp_spiWrite1(data,cnt );

} 
/************************************************************************************/
void SendDataBit_1621(uint8_t data,uint8_t cnt) //0x01
{  

	   bsp_spiWrite2(data,cnt );
}
/************************************************************************************/
void Write_1621(uint8_t addr,uint8_t data) 
{ 
  CS_0(); 
	SendBit_1621(0xa0,3);     //
	SendBit_1621(addr<<2,6);  //  
	SendDataBit_1621(data,8); //
  CS_1();
}

/************************************************************************************/
void WriteAll_1621(uint8_t addr,uint8_t *p,uint8_t cnt) 
{  
	uint8_t i; 
  CS_0(); 
	SendBit_1621(0xa0,3); 
//SendBit_1621(addr<<1,7); 
  SendBit_1621(0,6); 
	for(i =0; i <cnt; i ++,p++) 
	{  
		SendDataBit_1621(*p,8); 
	} 
   CS_1();
	
}
/************************************************************************************/
void writer_all_man(void)
{	
	uint8_t i; 
  CS_0(); 
	SendBit_1621(0xa0,3); 
//SendBit_1621(addr<<1,7); 
  SendBit_1621(0,6); 
	for(i =0; i <20; i ++) 
	{  
		SendDataBit_1621(0xff,8); 
	} 
  CS_1();

}
/************************************************************************************/
void SendCmd(uint8_t command) 
{ 
  CS_0(); 
	SendBit_1621(0x80,3);
	SendBit_1621(command,9);
  CS_1();  
}  

/************************************************************************************/
void HT1621_Init(void )
{  

 SendCmd(SYSEN); 
 SendCmd(LCDON);
	SendCmd(LCD_F16);
	
 SendCmd(TIMERDIS);
 SendCmd(WDTDIS);

	
}

/************************************************************************************/
void LCDD_deint(void)
{
 Trace_log("关闭液晶显示\r\n");
  SendCmd(SYSDIS);  
}


/************************************************************************************/
void WriteAll_HT1623(uint8_t *p,uint8_t cnt) 
{  
	uint8_t i; 
//	cs_0;  
	for(i=0;i<cnt;i++,p++)
  {
         Write_1621(Seg_HT1621b[i],*p);       
      		bsp_spiDelay();
					bsp_spiDelay();
  }
//	cs_1; 
}  




/************************************************************************************/
void init_clear_Num(uint8_t *p,uint8_t cnt)
{
  uint8_t i;
  for(i=0;i<cnt;i++)
	p[i]=0;
}
/************************************************************************************/
uint8_t Retrieve(uint8_t dat,uint8_t *p,uint8_t cnt)//查表，有就回1。
{
  uint8_t i;
  for(i=0;i<cnt;i++)
  {
    if(dat==*p)
			return 1;
    p++;
  }
  return 0;
}	



/************************************************************************************
Dis_unit  显示单位
Dis_data   显示数据
*time      显示时钟
Dis_signal     显示信号
vbet1      显示电量
Dis_dip     显示最多小数位
************************************************************************************/
//void Dis_All(char Dis_unit,float Dis_data ,uint8_t *time ,uint8_t signal ,uint16_t vbet1 ,uint8_t error_flag)
void Dis_All(char Dis_unit,float Dis_data ,uint8_t *time ,uint8_t Dis_signal ,uint16_t vbet1,uint8_t Dis_dip)
{
  uint8_t i,j;
  uint16_t tmp;
  uint16_t cnt=0;
  uint8_t tmp1;
	uint8_t vbet;
	
  vbet=(uint8_t)(vbet1/170);   ////  求 100% 

	memset(Dis_abc, 0, sizeof(Dis_abc)); 
  memset(Num_128, 0, sizeof(Num_128)); 
	
	
  cnt=display_num_value(Dis_data,Dis_abc,Dis_dip);   //显示数字

  /***************显示单位*******************/		
   switch(Dis_unit)
	 {
		 case  UNIT_MPa    :		Dis_abc[cnt++]=LCD_T30; break;      //Mpa单位																					
		 case  UNIT_KPa    :	  Dis_abc[cnt++]=LCD_T31; break;      //kpa单位
		 case  UNIT_Pa     :    Dis_abc[cnt++]=LCD_T32; break;      //pa单位
		 case  UNIT_psf    : 	  Dis_abc[cnt++]=LCD_T16; break;      //psf单位
		 case  UNIT_psi    :		Dis_abc[cnt++]=LCD_T15; break ;     //psi单位
		 case  UNIT_bar    :		Dis_abc[cnt++]=LCD_T14; break ;     //bar单位
		 case  UNIT_M      :		Dis_abc[cnt++]=LCD_T2;  break ;     //m单位
		 case  UNIT_ppm    :	  Dis_abc[cnt++]=LCD_T12; break ;     //ppm单位
		 case  UNIT_B      :		Dis_abc[cnt++]=LCD_T11; break ;     //%单位
		 case  UNIT_C      :	  Dis_abc[cnt++]=LCD_T10; break ;     //℃单位 
		 case  UNIT_ppb    :	  Dis_abc[cnt++]=LCD_T13; break ;     //ppb单位
		 case  UNIT_mm     :	  Dis_abc[cnt++]=LCD_T1;  break ;     //mm单位 
		 case  UNIT_M3     :	  Dis_abc[cnt++]=LCD_T4;  break ;     //M3单位
		 case  UNIT_mm_s   :	  Dis_abc[cnt++]=LCD_T6;  break ;     //mm/s单位
		 case  UNIT_M_s    :	  Dis_abc[cnt++]=LCD_T7;  break ;     //M/s单位
		 case  UNIT_M3_h   :	  Dis_abc[cnt++]=LCD_T8;  break ;     //M3/h单位
		 case  UNIT_0      :    break;                              //不显示单位
		
    default : break ; 
	}
/*******信号显示**********************/
	if(device_info.save.tab_sensor == TP307V1_1_A )  //lora
	{		
		 if(Dis_signal>70&&Dis_signal <90)
		 {
					 Dis_abc[cnt++]=LCD_T29;   //信号
		 }
		 else if(Dis_signal>=90&&Dis_signal <120)
		 {
						 Dis_abc[cnt++]=LCD_T29;   //信号
						 Dis_abc[cnt++]=LCD_T28;
		 }
		 else if(Dis_signal>=120&&Dis_signal <150)
		 {	    Dis_abc[cnt++]=LCD_T29;   //信号
						Dis_abc[cnt++]=LCD_T28;
						Dis_abc[cnt++]=LCD_T27;
		 }
		 else if(Dis_signal>=150 && Dis_signal <191)
		 {      Dis_abc[cnt++]=LCD_T29;   //信号
						Dis_abc[cnt++]=LCD_T28;
						Dis_abc[cnt++]=LCD_T27;
						Dis_abc[cnt++]=LCD_T26;
		 }	
		 else
		 {}
	 }
	 else
	 {		
		 if(Dis_signal>0&&Dis_signal <9)
		 {
					 Dis_abc[cnt++]=LCD_T29;   //信号
		 }
		 else if(Dis_signal>=9&&Dis_signal <17)
		 {
						 Dis_abc[cnt++]=LCD_T29;   //信号
						 Dis_abc[cnt++]=LCD_T28;
		 }
		 else if(Dis_signal>=17&&Dis_signal <24)
		 {	    Dis_abc[cnt++]=LCD_T29;   //信号
						Dis_abc[cnt++]=LCD_T28;
						Dis_abc[cnt++]=LCD_T27;
		 }
		 else if(Dis_signal>=24)
		 {     Dis_abc[cnt++]=LCD_T29;   //信号
						Dis_abc[cnt++]=LCD_T28;
						Dis_abc[cnt++]=LCD_T27;
						Dis_abc[cnt++]=LCD_T26;
		 }
	 }
/***************电量显示*******************/ 	 
	 if(vbet>0&&vbet<20)
	 {Dis_abc[cnt++]=LCD_T17; 	 //电量
	 }
	 else if(vbet>=20&&vbet <40)
	 {Dis_abc[cnt++]=LCD_T17; 	 //电量
		Dis_abc[cnt++]=LCD_T19;
	 }			 
	 else if(vbet>=40&&vbet <60)
	 {Dis_abc[cnt++]=LCD_T17; 	 //电量
		Dis_abc[cnt++]=LCD_T19;
		Dis_abc[cnt++]=LCD_T20;
	 }
	 else if(vbet>=60&&vbet <80)
	 {Dis_abc[cnt++]=LCD_T17; 	 //电量
		Dis_abc[cnt++]=LCD_T19;
		Dis_abc[cnt++]=LCD_T20;
		Dis_abc[cnt++]=LCD_T21;
	 }
	 else if(vbet>=80)
	 {Dis_abc[cnt++]=LCD_T17; 	 //电量
		Dis_abc[cnt++]=LCD_T19;
		Dis_abc[cnt++]=LCD_T20;
		Dis_abc[cnt++]=LCD_T21;
		Dis_abc[cnt++]=LCD_T33;
	 }
/***************时间显示*******************/			 
	 Dis_abc[cnt++]=LCD_T3;  //年
	 Dis_abc[cnt++]=LCD_T5;  //月
	 Dis_abc[cnt++]=LCD_T9;  //时
		 
	  tmp=sMyTime.buff[1]/10;	    //min
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[8][i];    //笔画对应的值
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
    tmp=sMyTime.buff[1]%10;    
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
   			if(tmp1&0x80)Dis_abc[cnt++]=Num_A[9][i];
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
//	
		tmp=sMyTime.buff[2]/10;	    //hour
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
      if(tmp)//如果是非0值
      {
        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[6][i];    //笔画对应的值
        else Dis_abc[cnt++]=0;
      }
      else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
    tmp=sMyTime.buff[2]%10;
		tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
        if(tmp1&0x80)Dis_abc[cnt++]=Num_A[7][i];
        tmp1<<=1;
    }
		tmp=sMyTime.buff[5]/10;	    //year
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {

        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[0][i];    //笔画对应的值
        else Dis_abc[cnt++]=0;

        tmp1<<=1;
    }
    tmp=sMyTime.buff[5]%10;
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
   			if(tmp1&0x80)Dis_abc[cnt++]=Num_A[1][i];
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
		tmp=sMyTime.buff[4]/10;	    //month
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {

        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[2][i];    //笔画对应的值
        else Dis_abc[cnt++]=0;

        tmp1<<=1;
    }
    tmp=sMyTime.buff[4]%10;
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
   			if(tmp1&0x80)Dis_abc[cnt++]=Num_A[3][i];
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
	  tmp=sMyTime.buff[3]/10;	    //day
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {

        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[4][i];    //笔画对应的值
        else Dis_abc[cnt++]=0;


        tmp1<<=1;
    }
    tmp=sMyTime.buff[3]%10;
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
   			if(tmp1&0x80)Dis_abc[cnt++]=Num_A[5][i];
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
    /*****转换成HT1623的显示内存***********/
    for(i=0;i<20;i++)
    {
      for(j=0;j<8;j++)
      {
        Num_128[i]>>=1;//显示内存
        if(Retrieve(LCD_SCREEN[i][j],Dis_abc,160))		
				Num_128[i]|=0x80;
      }
    }		
    WriteAll_HT1623(Num_128,20);
}
/************************************************************************************/
/************************************************************************************/
void oled_callback(void)
{
	device_info.event_oled = EVENT_OK; 
}
/************************************************************************************/
void Dis_err(uint8_t *timer ,uint8_t Dis_signal,uint16_t Dis_vbat,uint16_t Dis_error_flag)
{
  uint8_t i,j;
  uint16_t tmp;
  uint16_t cnt=0;
  uint8_t tmp1;

//	int  TEMP2;
//	uint8_t tempnum[5];
	uint8_t vbet;
  vbet=(uint8_t)(Dis_vbat /170);

		 memset(Dis_abc, 0, sizeof(Dis_abc)); 
		 memset(Num_128, 0, sizeof(Num_128)); 
 
/***************err状态显示*******************/				
    tmp1=0x9E;   //abcdefgh   10011110   E
		for(i=0;i<7;i++)
		{						
				if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[14][i];   
				else Dis_abc[cnt++]=0;
		 tmp1<<=1;
		}  
		tmp1=0xEE;   //abcdefgh   11101110   R
		for(i=0;i<7;i++)
		{						
				if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[13][i];   
				else Dis_abc[cnt++]=0;
		 tmp1<<=1;
		}  
		tmp1=0xEE;   //abcdefgh   11101110   R
		for(i=0;i<7;i++)
		{						
				if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[12][i];   
				else Dis_abc[cnt++]=0;
		 tmp1<<=1;
		}  
		tmp1=0x02;   //abcdefgh   00000010   -
		for(i=0;i<7;i++)
		{						
				if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[11][i];   
				else Dis_abc[cnt++]=0;
		 tmp1<<=1;
		}  
		tmp1=Num_0_9[Dis_error_flag];   //abcdefgh   11101110   R
		for(i=0;i<7;i++)
		{						
				if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[10][i];   
				else Dis_abc[cnt++]=0;
		 tmp1<<=1;
		} 

			 
/*******信号显示**********************/
	if(device_info.save.tab_sensor == TP307V1_1_A )  //lora
	{		
		 if(Dis_signal>70&&Dis_signal <90)
		 {
					 Dis_abc[cnt++]=LCD_T29;   //信号
		 }
		 else if(Dis_signal>=90&&Dis_signal <120)
		 {
						 Dis_abc[cnt++]=LCD_T29;   //信号
						 Dis_abc[cnt++]=LCD_T28;
		 }
		 else if(Dis_signal>=120&&Dis_signal <150)
		 {	    Dis_abc[cnt++]=LCD_T29;   //信号
						Dis_abc[cnt++]=LCD_T28;
						Dis_abc[cnt++]=LCD_T27;
		 }
		 else if(Dis_signal>=150 && Dis_signal <191)
		 {      Dis_abc[cnt++]=LCD_T29;   //信号
						Dis_abc[cnt++]=LCD_T28;
						Dis_abc[cnt++]=LCD_T27;
						Dis_abc[cnt++]=LCD_T26;
		 }	
		 else
		 {}
	 }
	 else
	 {		
		 if(Dis_signal>0&&Dis_signal <9)
		 {
					 Dis_abc[cnt++]=LCD_T29;   //信号
		 }
		 else if(Dis_signal>=9&&Dis_signal <17)
		 {
						 Dis_abc[cnt++]=LCD_T29;   //信号
						 Dis_abc[cnt++]=LCD_T28;
		 }
		 else if(Dis_signal>=17&&Dis_signal <24)
		 {	    Dis_abc[cnt++]=LCD_T29;   //信号
						Dis_abc[cnt++]=LCD_T28;
						Dis_abc[cnt++]=LCD_T27;
		 }
		 else if(Dis_signal>=24)
		 {     Dis_abc[cnt++]=LCD_T29;   //信号
						Dis_abc[cnt++]=LCD_T28;
						Dis_abc[cnt++]=LCD_T27;
						Dis_abc[cnt++]=LCD_T26;
		 }
	 }
/***************电量显示*******************/ 	 
	 if(vbet>0&&vbet<20)
	 {Dis_abc[cnt++]=LCD_T17; 	 //电量
	 }
	 else if(vbet>=20&&vbet <40)
	 {Dis_abc[cnt++]=LCD_T17; 	 //电量
		Dis_abc[cnt++]=LCD_T19;
	 }			 
	 else if(vbet>=40&&vbet <60)
	 {Dis_abc[cnt++]=LCD_T17; 	 //电量
		Dis_abc[cnt++]=LCD_T19;
		Dis_abc[cnt++]=LCD_T20;
	 }
	 else if(vbet>=60&&vbet <80)
	 {Dis_abc[cnt++]=LCD_T17; 	 //电量
		Dis_abc[cnt++]=LCD_T19;
		Dis_abc[cnt++]=LCD_T20;
		Dis_abc[cnt++]=LCD_T21;
	 }
	 else if(vbet>=80)
	 {Dis_abc[cnt++]=LCD_T17; 	 //电量
		Dis_abc[cnt++]=LCD_T19;
		Dis_abc[cnt++]=LCD_T20;
		Dis_abc[cnt++]=LCD_T21;
		Dis_abc[cnt++]=LCD_T33;
	 }
/***************时间显示*******************/			 
	 Dis_abc[cnt++]=LCD_T3;  //年
	 Dis_abc[cnt++]=LCD_T5;  //月
	 Dis_abc[cnt++]=LCD_T9;  //时
		 
	  tmp=sMyTime.buff[1]/10;	    //min
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[8][i];    //笔画对应的值
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
    tmp=sMyTime.buff[1]%10;    
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
   			if(tmp1&0x80)Dis_abc[cnt++]=Num_A[9][i];
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
//	
		tmp=sMyTime.buff[2]/10;	    //hour
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
      if(tmp)//如果是非0值
      {
        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[6][i];    //笔画对应的值
        else Dis_abc[cnt++]=0;
      }
      else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
    tmp=sMyTime.buff[2]%10;
		tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
        if(tmp1&0x80)Dis_abc[cnt++]=Num_A[7][i];
        tmp1<<=1;
    }
		tmp=sMyTime.buff[5]/10;	    //year
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {

        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[0][i];    //笔画对应的值
        else Dis_abc[cnt++]=0;

        tmp1<<=1;
    }
    tmp=sMyTime.buff[5]%10;
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
   			if(tmp1&0x80)Dis_abc[cnt++]=Num_A[1][i];
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
		tmp=sMyTime.buff[4]/10;	    //month
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {

        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[2][i];    //笔画对应的值
        else Dis_abc[cnt++]=0;

        tmp1<<=1;
    }
    tmp=sMyTime.buff[4]%10;
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
   			if(tmp1&0x80)Dis_abc[cnt++]=Num_A[3][i];
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
	  tmp=sMyTime.buff[3]/10;	    //day
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {

        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[4][i];    //笔画对应的值
        else Dis_abc[cnt++]=0;


        tmp1<<=1;
    }
    tmp=sMyTime.buff[3]%10;
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
   			if(tmp1&0x80)Dis_abc[cnt++]=Num_A[5][i];
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
    /*****转换成HT1623的显示内存***********/
    for(i=0;i<20;i++)
    {
      for(j=0;j<8;j++)
      {
        Num_128[i]>>=1;//显示内存
        if(Retrieve(LCD_SCREEN[i][j],Dis_abc,160))		
				Num_128[i]|=0x80;
      }
    }		
    WriteAll_HT1623(Num_128,20);
}

/************************************************************************************
num_value  显示数据值
*buf_num   显示数组
dip        小数点位数  
************************************************************************************/
uint8_t display_num_value(float num_value ,uint8_t *buf_num ,uint8_t dip)
{
	uint16_t cnt=0;
	uint8_t tmp1;
	int32_t value_temp;
	int8_t i=0,j=0;
	uint8_t value_num[6];	
	uint8_t dip1,dip2;
	
	dip1 = dip;
	dip2 = 0;

	if(num_value>=0)
	{
		if(num_value>99999)
		{
			dip2 = 0;
			value_temp = 99999;
		}
		else
		{
			while(dip1--)
			{
				if(num_value>9999)
				{
					value_temp = (int32_t)num_value;
					break;
				}
				else
				{
					dip2++;
				  num_value = num_value*10;
				}
			}
		}
		num_value = num_value*10;  //多取一位作为四舍五入
		value_temp = (int32_t)num_value;  //数据取整
		value_num[5] = value_temp%10;   //四舍五入位
		value_temp = value_temp/10;
		if(value_num[5]>=5)
			value_temp = value_temp+1;
		value_num[4]= value_temp/10000;
		value_num[3]=	value_temp%10000/1000;
		value_num[2]= value_temp%1000/100;
		value_num[1]=	value_temp%100/10;
		value_num[0]= value_temp%10;
		switch(dip2)   //确定小数点位置
		{
			case 0 : break;
			case 1 : buf_num[cnt++]=LCD_DP11; break;
			case 2 : buf_num[cnt++]=LCD_DP12; break;
			case 3 : buf_num[cnt++]=LCD_DP13; break;
			case 4 : buf_num[cnt++]=LCD_DP14; break;
			default : break;
		}
		j = 5;
		while(j>0)   //找出首位不为0或者小数点位置
		{
			j--;
			if(value_num[j] == 0)
			{
				if(dip2 == (j))
					break;
			}
			else
				break;
		}		
		while(j>=0)   //取显示数据
		{			
			tmp1=Num_0_9[value_num[j]];
			for(i=0;i<7;i++)
			{
				if(tmp1&0x80)
					buf_num[cnt++]=Num_A[10+j][i];    //笔画对应的值
				else buf_num[cnt++]=0;
				tmp1<<=1;
			}
			j--;
		}
	}
	else
	if(num_value<0)
	{
		dip1 = dip-1;
		num_value=-num_value;
		if(num_value>9999)
		{
			dip2 = 0;
			value_temp = 9999;
		}
		else
		{
			while(dip1--)
			{
				if(num_value>999)
				{
					value_temp = (int32_t)num_value;
					break;
				}
				else
				{
					dip2++;
				  num_value = num_value*10;
				}
			}
		}
		num_value = num_value*10;  //多取一位作为四舍五入
		value_temp = (int32_t)num_value;  //数据取整
		value_num[4] = value_temp%10;   //四舍五入位
		value_temp = value_temp/10;
		if(value_num[4]>=5)
			value_temp = value_temp+1;
		value_num[3]=	value_temp%10000/1000;
		value_num[2]= value_temp%1000/100;
		value_num[1]=	value_temp%100/10;
		value_num[0]= value_temp%10;
		switch(dip2)   //确定小数点位置
		{
			case 0 : break;
			case 1 : buf_num[cnt++]=LCD_DP11; break;
			case 2 : buf_num[cnt++]=LCD_DP12; break;
			case 3 : buf_num[cnt++]=LCD_DP13; break;
			default : break;
		}		
		j = 4;
		while(j>1)   //找出首位不为0或者小数点位置
		{
			j--;
			if(value_num[j] == 0)
			{
				if(dip2 == (j))
					break;
			}
			else
				break;
		}	
		
		tmp1=Signal[0];//tmp=0，显示-;tmp=1;显示-1
		for(i=0;i<7;i++)
		{
				if(tmp1&0x80)
					buf_num[cnt++]=Num_A[11+j][i];    //笔画对应的值
				else buf_num[cnt++]=0;
				tmp1<<=1;
		}
			
		while(j>=0)   //取显示数据
		{			
			tmp1=Num_0_9[value_num[j]];
			for(i=0;i<7;i++)
			{
				if(tmp1&0x80)
					buf_num[cnt++]=Num_A[10+j][i];    //笔画对应的值
				else buf_num[cnt++]=0;
				tmp1<<=1;
			}
			j--;
		}
	}
	return cnt ;
}

