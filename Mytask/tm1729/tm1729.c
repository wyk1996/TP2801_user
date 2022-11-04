#include "main.h"
#include "dianwei.h"
#include "tm1729.h"
extern device_t device_info;
extern int16_t sennsor_mode_flag;
uint16_t Dis_abc[136];     //缓存要写入的字段   一共34*4=136个
uint8_t TM1729_BUFF[52];//TM1792里面34个寄存器数组中需要写入的数据

#define SDA6_PIN      LL_GPIO_PIN_1
#define SCL6_PIN      LL_GPIO_PIN_0

#define  SCL6_H         LL_GPIO_SetOutputPin(GPIOC,SCL6_PIN)
#define  SCL6_L         LL_GPIO_ResetOutputPin(GPIOC,SCL6_PIN)

#define  SDA6_H         LL_GPIO_SetOutputPin(GPIOC,SDA6_PIN)
#define  SDA6_L         LL_GPIO_ResetOutputPin(GPIOC,SDA6_PIN)

#define  SDA6_read       LL_GPIO_IsInputPinSet(GPIOC, SDA6_PIN)


/************************************************************************************/
void TM1792_IIC6_INIT(void)
{
	LL_GPIO_InitTypeDef GPIO_Initure;

	GPIO_Initure.Pin=SDA6_PIN;
	GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
	GPIO_Initure.Pull=ENABLE;          //上拉
	GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;    
	LL_GPIO_Init(GPIOC,&GPIO_Initure);
 
	GPIO_Initure.Pin=SCL6_PIN;
	GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
  GPIO_Initure.Pull=ENABLE;       
	GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;    
	LL_GPIO_Init(GPIOC,&GPIO_Initure);

	SDA6_H;
	SCL6_H;		
}

void TM1792_IIC6_DEINIT(void)
{
	LL_GPIO_InitTypeDef GPIO_Initure;

//	GPIO_Initure.Pin= SDA6_PIN | SCL6_PIN;
//	GPIO_Initure.Mode=LL_GPIO_MODE_ANALOG;   
//	GPIO_Initure.Pull=DISABLE;          //上拉
//	GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;    
//	LL_GPIO_Init(GPIOC,&GPIO_Initure);
 
	GPIO_Initure.Pin= SDA6_PIN | SCL6_PIN;
	GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
  GPIO_Initure.Pull=DISABLE;       
	GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;    
	LL_GPIO_Init(GPIOC,&GPIO_Initure);
  LL_GPIO_ResetOutputPin(GPIOC, SDA6_PIN | SCL6_PIN);	
}
/*********************************************************************************************/
static void gpio_set_SDA6_mode(uint8_t mode)
{
	LL_GPIO_InitTypeDef GPIO_Initure;
	if(mode == gpio_input)
		GPIO_Initure.Mode=LL_GPIO_MODE_INPUT;
	else if(mode == gpio_output)
	{
		GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT; 		
	}
	
	GPIO_Initure.Pin=SDA6_PIN;                       //SDA  
	GPIO_Initure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_Initure.Pull=ENABLE;          //上拉	    
	LL_GPIO_Init(GPIOC,&GPIO_Initure);
}
/************************************************************************************/
/*********************************************
 * 函数名：I6Cdelay
 * 描  述：I6C延时函数
 * 输  入：us
 * 输  出：无
 ********************************************/
static void I6Cdelay(uint8_t d_us)
{	
  uint8_t i=100; //这里可以优化速度，通常延时3~10us，可以用示波器看波形来调试
	uint8_t j;
	j = d_us;
	while(j--)
	{
		 while(i) 
		 { 
			 i--; 
		 }
  }
}
/************************************************************************************/
/*********************************************
 * 函数名：I6CStart
 * 描  述：开启I6C总线
 * 输  入：无
 * 输  出：TRUE:操作成功，FALSE:操作失败
 ********************************************/
static uint8_t I6CStart(void)
{
    SDA6_H;
		I6Cdelay(1);
	  gpio_set_SDA6_mode(gpio_input);
    SCL6_H;
    I6Cdelay(1);
    if(!SDA6_read)
		{
			gpio_set_SDA6_mode(gpio_output);
			return FALSE;	//SDA线为低电平则总线忙,退出
		}
		gpio_set_SDA6_mode(gpio_output);
    SDA6_L;
    I6Cdelay(1);
    SCL6_L;
    I6Cdelay(1);
    return TRUE;
}
/*********************************************
 * 函数名：I6CStop
 * 描  述：释放I6C总线
 * 输  入：无
 * 输  出：无
 ********************************************/
static void I6CStop(void)
{
    SCL6_L;
    I6Cdelay(1);
    SDA6_L;
    I6Cdelay(1);
    SCL6_H;
    I6Cdelay(1);
    SDA6_H;
    I6Cdelay(1);
}
/*********************************************
 * 函数名：I6CAck
 * 描  述：发送ASK
 * 输  入：无
 * 输  出：无
 ********************************************/
static void I6CAck(void)
{
    SCL6_L;
    I6Cdelay(1);
    SDA6_L;
    I6Cdelay(1);
    SCL6_H;
    I6Cdelay(1);
    SCL6_L;
    I6Cdelay(1);
}
/*********************************************
 * 函数名：I6CNoAck
 * 描  述：发送NOASK
 * 输  入：无
 * 输  出：无
 ********************************************/
static void I6CNoAck(void)
{
    SCL6_L;
    I6Cdelay(1);
    SDA6_H;
    I6Cdelay(1);
    SCL6_H;
    I6Cdelay(1);
    SCL6_L;
    I6Cdelay(1);
}
/*********************************************
 * 函数名：I6CWaitAck
 * 描  述：读取ACK信号
 * 输  入：无
 * 输  出：TRUE=有ACK,FALSE=无ACK
 ********************************************/
static uint8_t I6CWaitAck(void)
{
    SCL6_L;
    I6Cdelay(1);
    SDA6_H;			
    I6Cdelay(1);
		gpio_set_SDA6_mode(gpio_input);
    SCL6_H;
    I6Cdelay(1);
    if(SDA6_read)
    {
			SCL6_L;
			gpio_set_SDA6_mode(gpio_output);
			return FALSE;
    }
		gpio_set_SDA6_mode(gpio_output);
    SCL6_L;
    return TRUE;
}
/*********************************************
 * 函数名：I6CSendByte
 * 描  述：MCU发送一个字节
 * 输  入：无
 * 输  出：无
 ********************************************/
static void I6CSendByte(uint8_t SendByte) //数据从高位到低位
{
    uint8_t i=8;
		while(i--)
		{
			SCL6_L;
			I6Cdelay(1);
			if(SendByte&0x80)
			SDA6_H;  
			else 
			SDA6_L;   
			SendByte<<=1;
			I6Cdelay(1);
			SCL6_H;
			I6Cdelay(1);
		}
    SCL6_L;
}
/*********************************************
 * 函数名：I6CReceiveByte
 * 描  述：MCU读入一个字节
 * 输  入：无
 * 输  出：ReceiveByte
 ********************************************/
static uint8_t I6CReceiveByte(unsigned char ack)
{
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    SDA6_H;	
    gpio_set_SDA6_mode(gpio_input);	
    while(i--)
    {
      ReceiveByte<<=1;         
      SCL6_L;
      I6Cdelay(1);
      SCL6_H;
      I6Cdelay(1);	
      if(SDA6_read)
      {
        ReceiveByte|=0x1;
      }
    }
		gpio_set_SDA6_mode(gpio_output);
    SCL6_L;
		
		if (!ack)
        I6CNoAck();//发送nACK
    else
        I6CAck(); //发送ACK
    return ReceiveByte;   
}
/************************************************************************************/
//IIC写一个字节 
//data:数据
//返回值:TRUE,正常
//    其他,错误代码
uint8_t TM1729_Write_Byte(uint8_t data) 				 
{ 
    I6CStart(); 
	I6CSendByte((uint8_t)(TM1729_ADDR<<1)|0);//发送器件地址+写命令	
	if(I6CWaitAck()  == FALSE)	//等待应答
	{
		I6CStop();		 
		return FALSE;		
	} 
	I6CSendByte(data);//发送数据
	if(I6CWaitAck()  == FALSE)	//等待ACK
	{
		I6CStop();	 
		return FALSE;		 
	}		 
  I6CStop();	 
	return TRUE;
}
/************************************************************************************/
//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
uint8_t TM1729_Read_Byte(uint8_t reg)
{
	uint8_t res_b;
    I6CStart(); 
	I6CSendByte((uint8_t)(TM1729_ADDR<<1)|0);//发送器件地址+写命令	
	I6CWaitAck();		//等待应答 
    I6CSendByte(reg);	//写寄存器地址
    I6CWaitAck();		//等待应答
    I6CStart();
	I6CSendByte((uint8_t)(TM1729_ADDR<<1)|1);//发送器件地址+读命令	
    I6CWaitAck();		//等待应答 
	res_b=I6CReceiveByte(0);//读取数据,发送nACK 
    I6CStop();			//产生一个停止条件 
	return res_b;		
}
/************************************************************************************/
//IIC连续写入显示数据
//len:写入长度
//buf:数据区
//返回值:TRUE,正常
//    其他,错误代码
uint8_t TM1729_Write_DATA(uint8_t len,uint8_t *buf)
{
	uint8_t i,w_data; 
    I6CStart(); 
	I6CSendByte((uint8_t)(TM1729_ADDR<<1)|0);//发送器件地址+写命令	
	if(I6CWaitAck()  == FALSE)	//等待应答
	{
		I6CStop();		 
		return FALSE;		
	}
    I6CSendByte(0x00);	//写寄存器地址
    I6CWaitAck();		//等待应答
	for(i=0;i<len;i++)
	{
		w_data = buf[i];
		w_data <<= 4;
		i++;
		w_data = w_data|buf[i];
		I6CSendByte(w_data);	//发送数据
		if(I6CWaitAck()  == FALSE)		//等待ACK
		{
			I6CStop();	 
			return FALSE;		 
		}		
	}    
  I6CStop();	 
	return TRUE;	
} 
/************************************************************************************/
/************************************************************************************/
//IIC连续写入控制命令
//len:写入长度
//buf:数据区
//返回值:TRUE,正常
//    其他,错误代码
uint8_t TM1729_Write_CMD(uint8_t len,uint8_t *buf)
{
	uint8_t i; 
    I6CStart(); 
	I6CSendByte((TM1729_ADDR<<1)|0);//发送器件地址+写命令	
	if(I6CWaitAck()  == FALSE)	//等待应答
	{
		I6CStop();		 
		return FALSE;		
	}
	for(i=0;i<len;i++)
	{
		I6CSendByte(buf[i]);	//发送数据
		if(I6CWaitAck()  == FALSE)		//等待ACK
		{
			I6CStop();	 
			return FALSE;		 
		}		
	}    
  I6CStop();	 
	return TRUE;	
} 
/************************************************************************************/

/************************************************************************************/

/************************************************************************************/
 

/************************************************************************************/
uint8_t TM1729_Init(void )
{  
  uint8_t cm[2],retu;
  cm[0] = TM1729_ICSET|TM1729_ICSET_RESET;     //复位
  cm[1] = TM1729_DISCTL|TM1729_SAVE_3_MODE|TM1729_POWER_SAVE_1|TM1729_FRAME_FLIP;
	retu = TM1729_Write_CMD(2,cm);
	return retu;
}

/************************************************************************************/
void LCD_CLOSE(void)
{
   Trace_log("关闭液晶显示\r\n");
   TM1729_Write_Byte(TM1729_MODESET);
}
/************************************************************************************/
void LCD_OPEN_ALL(void)
{
	Trace_log("显示全部字段\r\n");
  TM1729_Write_Byte(TM1729_APCTL|TM1729_APCTL_ON);
}
/************************************************************************************/
void LCD_OFF_ALL(void)
{
	Trace_log("关闭全部字段\r\n");
  TM1729_Write_Byte(TM1729_APCTL|TM1729_APCTL_OFF);
}
/************************************************************************************/
void TM1729_display_init(void)
{
	uint8_t cm[3];
	cm[0] = TM1729_BLKCTL|TM1729_BLKCTL_OFF;     //不闪烁控制
	cm[1] = TM1729_APCTL;
	cm[2] = TM1729_MODESET | TM1729_1_3_BIAS;
	TM1729_Write_CMD(3,cm);
}
/************************************************************************************/
void init_clear_Num(uint8_t *p,uint8_t cnt)
{
  uint8_t i;
  for(i=0;i<cnt;i++)
	p[i]=0;
}
/************************************************************************************/
uint8_t Retrieve_TM1729(uint16_t *p,uint8_t cnt)//查表，有就回1。
{
  uint8_t i;
	uint8_t TM1729_BF_H,TM1729_BF_L;
	uint16_t TM1729_BF;
  for(i=0;i<cnt;i++)
  {
		TM1729_BF = *p;
		TM1729_BF_L = TM1729_BF;
		TM1729_BF_H = (TM1729_BF>>8);
		TM1729_BUFF[TM1729_BF_H] |= TM1729_BF_L;
    p++;
  }
  return 0;
}	

/************************************************************************************
num_value  显示数据值
*buf_num   显示数组
dip        小数点位数  
************************************************************************************/
uint8_t display_num_value(float num_value ,uint16_t *buf_num ,uint8_t dip)
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
//					value_temp = (int32_t)num_value;
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
					buf_num[cnt++]=Num_A[9+j][i];    //笔画对应的值
				else buf_num[cnt++]=0;
				tmp1<<=1;
			}
			j--;
		}
	}
	else
	if(num_value<0)
	{
		num_value=-num_value;
		if(dip1>3)
			dip1 = 3;
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
//					value_temp = (int32_t)num_value;
					break;
				}
				else
				{
					dip2++;
				  num_value = num_value*10;
				}
			}
		}
		num_value = num_value*10;  //多取一位作为四舍五入  小于0时   上面计算依据多取了一位
		value_temp = (int32_t)num_value;  //数据取整
		value_num[5] = value_temp%10;   //四舍五入位
		value_temp = value_temp/10;
		if(value_num[5]>=5)
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
			case 4 : buf_num[cnt++]=LCD_DP14; break;
			default : break;
		}		
		
		tmp1=Signal[0];//tmp=0，显示-;tmp=1;显示-1
		for(i=0;i<7;i++)
		{
				if(tmp1&0x80)
					buf_num[cnt++]=Num_A[13][i];    //笔画对应的值
				else buf_num[cnt++]=0;
				tmp1<<=1;
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
			
		while(j>=0)   //取显示数据
		{			
			tmp1=Num_0_9[value_num[j]];
			for(i=0;i<7;i++)
			{
				if(tmp1&0x80)
					buf_num[cnt++]=Num_A[9+j][i];    //笔画对应的值
				else buf_num[cnt++]=0;
				tmp1<<=1;
			}
			j--;
		}
	}
	return cnt ;
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
void Dis_All(char Dis_unit,float Dis_data ,uint8_t *time ,uint8_t Dis_signal ,float vbet1,uint8_t Dis_dip)
{
  uint8_t i;
  uint16_t tmp;
  uint16_t cnt=0;
  uint8_t tmp1;
	uint8_t vbet;
	
  vbet=(uint8_t)vbet1;   ////  求 100% 

	memset(Dis_abc, 0, sizeof(Dis_abc)); 
  memset(TM1729_BUFF, 0, sizeof(TM1729_BUFF)); 
	
	
  cnt=display_num_value(Dis_data,Dis_abc,Dis_dip);   //显示数字

  /***************显示单位*******************/		
   switch(Dis_unit)
	 {
		 case  UNIT_MPa    :		Dis_abc[cnt++]=LCD_DW_MPa; break;     //Mpa单位																					
		 case  UNIT_KPa    :	  Dis_abc[cnt++]=LCD_DW_KPa; break;     //kpa单位
		 case  UNIT_Pa     :    Dis_abc[cnt++]=LCD_DW_Pa;  break;     //pa单位
		 case  UNIT_psf    : 	  Dis_abc[cnt++]=LCD_DW_psf; break;     //psf单位
		 case  UNIT_psi    :		Dis_abc[cnt++]=LCD_DW_psi; break;     //psi单位
		 case  UNIT_bar    :		Dis_abc[cnt++]=LCD_DW_bar; break;     //bar单位
		 
		 case  UNIT_ppm    :	  Dis_abc[cnt++]=LCD_DW_ppm; break;     //ppm单位
		 case  UNIT_ppb    :	  Dis_abc[cnt++]=LCD_DW_ppb; break;     //ppb单位
		 
		 case  UNIT_V      :	  Dis_abc[cnt++]=LCD_DW_V;   break;     //V单位
		 case  UNIT_mA     :	  Dis_abc[cnt++]=LCD_DW_mA;  break;     //mA单位
		 
		 case  UNIT_mm     :	  Dis_abc[cnt++]=LCD_DW_mm;  break;     //mm单位
		 case  UNIT_M      :		Dis_abc[cnt++]=LCD_DW_M;   break;     //M单位
		 
		 case  UNIT_C      :	  Dis_abc[cnt++]=LCD_DW_C;   break;     //℃单位 
		 case  UNIT_F      :	  Dis_abc[cnt++]=LCD_DW_F;   break;     //F单位
	   case  UNIT_dp      :	  Dis_abc[cnt++]=LCD_DW_dp;  break;     //dp单位	 
		 case  UNIT_BF      :		Dis_abc[cnt++]=LCD_DW_BF;  break;     //%单位

		 case  UNIT_0      :    break;                              //不显示单位
		
    default : break ; 
	}
/*******信号显示**********************/
	if(device_info.save.tab_module == TP307V1_1_A || device_info.save.tab_module == LORAWAN )  //lora
	{		
		 if(Dis_signal>70&&Dis_signal <90)   //信号
		 {
				tmp1 = 1;   
		 }
		 else if(Dis_signal>=90&&Dis_signal <120)
		 {
			 tmp1 = 2;
		 }
		 else if(Dis_signal>=120&&Dis_signal <150)
		 {	    
			 tmp1 = 3;
		 }
		 else if(Dis_signal>=150 && Dis_signal <200)
		 {
			 tmp1 = 4;
		 }
     else
			 tmp1 = 0;
		 for(i=0;i<tmp1;i++)
		 {
			 Dis_abc[cnt++] = single_DB[i];
		 }
			 
	 }
	 else
	 {		
		 if(Dis_signal>0&&Dis_signal <9)    //信号
		 {
				tmp1 = 1;
		 }
		 else if(Dis_signal>=9&&Dis_signal <17)
		 {
				tmp1 = 2;
		 }
		 else if(Dis_signal>=17&&Dis_signal <24)
		 {
			 tmp1 = 3;
		 }
		 else if(Dis_signal>=24)
		 {
			 tmp1 = 4;
		 }
		 else
			 tmp1 = 0;
		 for(i=0;i<tmp1;i++)
		 {
			 Dis_abc[cnt++] = single_DB[i];
		 }
	 }
/***************电量显示*******************/ 	 
	 if(vbet>=20&&vbet <40)
	 {
		 tmp1 = 1;
	 }			 
	 else if(vbet>=40&&vbet <60)
	 {
		 tmp1 = 2;
	 }
	 else if(vbet>=60&&vbet <80)
	 {
		 tmp1 = 3;
	 }
	 else if(vbet>=80)
	 {
		 tmp1 = 4;
	 }
	 else
		 tmp1 = 0;
	 for(i=0;i<tmp1;i++)
	 {
		 Dis_abc[cnt++] = BAT_BF[i];
	 }
/***************充电标志显示*******************/ 
	 if(device_info.save.tab_model  == TP240XVX_3_P || device_info.save.tab_model  == TP240XVX_3_P_Ex)   //外部供电可充电版本
	 {
		 if(device_info.waibu_pwr_24 == 1 )//|| device_info.waibu_pwr_usb == 1
			 Dis_abc[cnt++] = ZT_TB[0];
	 }
/***************状态指示图标显示*******************/ 
	 if(device_info.gps_satues == 1)
		 Dis_abc[cnt++] = LCD_DW_DW;
//	 if(device_info.out_power == 1)
//		 Dis_abc[cnt++] = LCD_DW_CD;
/***************常亮内容*******************/			 
	 for(i=0;i<4;i++)
	 {
		 Dis_abc[cnt++] = CL_TB[i];
	 }
/***************时间显示*******************/
	  tmp=sMyTime.buff[1]/10;	    //min
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[7][i];    //笔画对应的值
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
    tmp=sMyTime.buff[1]%10;    
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
   			if(tmp1&0x80)Dis_abc[cnt++]=Num_A[8][i];
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
					Dis_abc[cnt++]=Num_A[5][i];    //笔画对应的值
        else Dis_abc[cnt++]=0;
      }
      else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
    tmp=sMyTime.buff[2]%10;
		tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
        if(tmp1&0x80)Dis_abc[cnt++]=Num_A[6][i];
        tmp1<<=1;
    }
//		tmp=sMyTime.buff[5]/10;	    //year
//    tmp1=Num_0_9[tmp];
//    for(i=0;i<7;i++)
//    {

//        if(tmp1&0x80)
//					Dis_abc[cnt++]=Num_A[0][i];    //笔画对应的值
//        else Dis_abc[cnt++]=0;

//        tmp1<<=1;
//    }
    tmp=sMyTime.buff[5]%10;
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
   			if(tmp1&0x80)Dis_abc[cnt++]=Num_A[0][i];
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
		tmp=sMyTime.buff[4]/10;	    //month
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[1][i];    //笔画对应的值
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
    tmp=sMyTime.buff[4]%10;
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
   			if(tmp1&0x80)Dis_abc[cnt++]=Num_A[2][i];
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
	  tmp=sMyTime.buff[3]/10;	    //day
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[3][i];    //笔画对应的值
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
    tmp=sMyTime.buff[3]%10;
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
   			if(tmp1&0x80)Dis_abc[cnt++]=Num_A[4][i];
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
    /*****转换成HT1623的显示内存***********/
		Retrieve_TM1729(Dis_abc,136);
	
    TM1729_Write_DATA(34,TM1729_BUFF);
}
/************************************************************************************/
/************************************************************************************/

/************************************************************************************/
void Dis_err(uint8_t *timer ,uint8_t Dis_signal,float Dis_vbat,uint16_t Dis_error_flag)
{
  uint8_t i;
  uint16_t tmp;
  uint16_t cnt=0;
  uint8_t tmp1;

//	int  TEMP2;
//	uint8_t tempnum[5];
	uint8_t vbet;
  vbet=(uint8_t)Dis_vbat;

		 memset(Dis_abc, 0, sizeof(Dis_abc)); 
		 memset(TM1729_BUFF, 0, sizeof(TM1729_BUFF)); 
 
/***************err状态显示*******************/				
    tmp1=0x9E;   //abcdefgh   10011110   E
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
		tmp1=0xEE;   //abcdefgh   11101110   R
		for(i=0;i<7;i++)
		{						
				if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[11][i];   
				else Dis_abc[cnt++]=0;
		 tmp1<<=1;
		}  
		tmp1=0x02;   //abcdefgh   00000010   -
		for(i=0;i<7;i++)
		{						
				if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[10][i];   
				else Dis_abc[cnt++]=0;
		 tmp1<<=1;
		}  
		tmp1=Num_0_9[Dis_error_flag];   //abcdefgh   11101110   R
		for(i=0;i<7;i++)
		{						
				if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[9][i];   
				else Dis_abc[cnt++]=0;
		 tmp1<<=1;
		} 

			 
/*******信号显示**********************/
	if(device_info.save.tab_module == TP307V1_1_A || device_info.save.tab_module == LORAWAN )  //lora
	{		
		 if(Dis_signal>70&&Dis_signal <90)   //信号
		 {
				tmp1 = 1;   
		 }
		 else if(Dis_signal>=90&&Dis_signal <120)
		 {
			 tmp1 = 2;
		 }
		 else if(Dis_signal>=120&&Dis_signal <150)
		 {	    
			 tmp1 = 3;
		 }
		 else if(Dis_signal>=150 && Dis_signal <191)
		 {
			 tmp1 = 4;
		 }	
		 for(i=0;i<tmp1;i++)
		 {
			 Dis_abc[cnt++] = single_DB[i];
		 }
			 
	 }
	 else
	 {		
		 if(Dis_signal>0&&Dis_signal <9)    //信号
		 {
				tmp1 = 1;
		 }
		 else if(Dis_signal>=9&&Dis_signal <17)
		 {
				tmp1 = 2;
		 }
		 else if(Dis_signal>=17&&Dis_signal <24)
		 {
			 tmp1 = 3;
		 }
		 else if(Dis_signal>=24)
		 {
			 tmp1 = 4;
		 }
		 for(i=0;i<tmp1;i++)
		 {
			 Dis_abc[cnt++] = single_DB[i];
		 }
	 }
/***************电量显示*******************/ 	 
	 if(vbet>=20&&vbet <40)
	 {
		 tmp1 = 1;
	 }			 
	 else if(vbet>=40&&vbet <60)
	 {
		 tmp1 = 2;
	 }
	 else if(vbet>=60&&vbet <80)
	 {
		 tmp1 = 3;
	 }
	 else if(vbet>=80)
	 {
		 tmp1 = 4;
	 }
	 for(i=0;i<tmp1;i++)
	 {
		 Dis_abc[cnt++] = BAT_BF[i];
	 }
/***************充电标志显示*******************/ 
	 if(device_info.save.tab_model  == TP240XVX_3_P || device_info.save.tab_model  == TP240XVX_3_P_Ex)   //外部供电可充电版本
	 {
		 if(device_info.waibu_pwr_24 == 1 ) //|| device_info.waibu_pwr_usb == 1
			 Dis_abc[cnt++] = ZT_TB[0];
	 }
/***************状态指示图标显示*******************/ 
	 if(device_info.gps_satues == 1)
		 Dis_abc[cnt++] = LCD_DW_DW;
//	 if(device_info.out_power == 1)
//		 Dis_abc[cnt++] = LCD_DW_CD;
/***************常亮内容*******************/			 
	 for(i=0;i<4;i++)
	 {
		 Dis_abc[cnt++] = CL_TB[i];
	 }
/***************时间显示*******************/			 
	  tmp=sMyTime.buff[1]/10;	    //min
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[7][i];    //笔画对应的值
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
    tmp=sMyTime.buff[1]%10;    
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
   			if(tmp1&0x80)Dis_abc[cnt++]=Num_A[8][i];
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
					Dis_abc[cnt++]=Num_A[5][i];    //笔画对应的值
        else Dis_abc[cnt++]=0;
      }
      else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
    tmp=sMyTime.buff[2]%10;
		tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
        if(tmp1&0x80)Dis_abc[cnt++]=Num_A[6][i];
        tmp1<<=1;
    }
//		tmp=sMyTime.buff[5]/10;	    //year
//    tmp1=Num_0_9[tmp];
//    for(i=0;i<7;i++)
//    {

//        if(tmp1&0x80)
//					Dis_abc[cnt++]=Num_A[0][i];    //笔画对应的值
//        else Dis_abc[cnt++]=0;

//        tmp1<<=1;
//    }
    tmp=sMyTime.buff[5]%10;
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
   			if(tmp1&0x80)Dis_abc[cnt++]=Num_A[0][i];
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
		tmp=sMyTime.buff[4]/10;	    //month
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {

        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[1][i];    //笔画对应的值
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
    tmp=sMyTime.buff[4]%10;
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
   			if(tmp1&0x80)Dis_abc[cnt++]=Num_A[2][i];
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
	  tmp=sMyTime.buff[3]/10;	    //day
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[3][i];    //笔画对应的值
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
    tmp=sMyTime.buff[3]%10;
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
   			if(tmp1&0x80)Dis_abc[cnt++]=Num_A[4][i];
        else Dis_abc[cnt++]=0;
        tmp1<<=1;
    }
    /*****转换成HT1623的显示内存***********/
    Retrieve_TM1729(Dis_abc,136);
	
    TM1729_Write_DATA(34,TM1729_BUFF);
}
/************************************************************************************/
uint8_t shuzi_display(uint8_t dis_dat,uint16_t *buf_num)
{
	uint8_t i,j,dat_cnt;
	uint8_t dat_value;
	
	dat_cnt = 0;	
	for(j=0;j<14;j++)
	{
		dat_value = Num_0_9[dis_dat];
		for(i=0;i<7;i++)
		{
			if(dat_value&0x80)
			  buf_num[dat_cnt++] = Num_A[j][i];
			dat_value<<=1;
		}
	}
	return dat_cnt;
}
/************************************************************************************/
void LCD_TEST(void)
{
	uint8_t i;
  uint16_t cnt=0;
	while(1)
	{
//		IWDT_Clr();
		for(i=0;i<10;i++)
		{
			memset(Dis_abc, 0, sizeof(Dis_abc)); 
			memset(TM1729_BUFF, 0, sizeof(TM1729_BUFF));
			cnt = shuzi_display(i,Dis_abc);	
			Retrieve_TM1729(Dis_abc,136);	
			TM1729_Write_DATA(52,TM1729_BUFF);		
			delay_vtask(1000);
		}
//		IWDT_Clr();
		
		for(i=0;i<16;i++)
		{
			memset(Dis_abc, 0, sizeof(Dis_abc)); 
			memset(TM1729_BUFF, 0, sizeof(TM1729_BUFF));
			Dis_abc[0] = LCD_DW[i];	
			Retrieve_TM1729(Dis_abc,136);	
			TM1729_Write_DATA(52,TM1729_BUFF);
			delay_vtask(500);
		}
//		IWDT_Clr();
		
		cnt = 0;
		for(i=0;i<4;i++)
		{
			memset(Dis_abc, 0, sizeof(Dis_abc)); 
			memset(TM1729_BUFF, 0, sizeof(TM1729_BUFF));
			Dis_abc[cnt++] = single_DB[i];
			Dis_abc[cnt++] = BAT_BF[i];	
			Dis_abc[cnt++] = CL_TB[i];
			Dis_abc[cnt++] = ZT_TB[i];
			Retrieve_TM1729(Dis_abc,136);	
			TM1729_Write_DATA(52,TM1729_BUFF);
			delay_vtask(1000);
		}
//		IWDT_Clr();		
	}
	
}
/************************************************************************************/
/************************************************************************************/
/************************************************************************************/
/************************************************************************************/



