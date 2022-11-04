#include "main.h"
#include "dianwei.h"
#include "tm1729.h"
extern device_t device_info;
extern int16_t sennsor_mode_flag;
uint16_t Dis_abc[136];     //����Ҫд����ֶ�   һ��34*4=136��
uint8_t TM1729_BUFF[52];//TM1792����34���Ĵ�����������Ҫд�������

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
	GPIO_Initure.Pull=ENABLE;          //����
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
//	GPIO_Initure.Pull=DISABLE;          //����
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
	GPIO_Initure.Pull=ENABLE;          //����	    
	LL_GPIO_Init(GPIOC,&GPIO_Initure);
}
/************************************************************************************/
/*********************************************
 * ��������I6Cdelay
 * ��  ����I6C��ʱ����
 * ��  �룺us
 * ��  ������
 ********************************************/
static void I6Cdelay(uint8_t d_us)
{	
  uint8_t i=100; //��������Ż��ٶȣ�ͨ����ʱ3~10us��������ʾ����������������
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
 * ��������I6CStart
 * ��  ��������I6C����
 * ��  �룺��
 * ��  ����TRUE:�����ɹ���FALSE:����ʧ��
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
			return FALSE;	//SDA��Ϊ�͵�ƽ������æ,�˳�
		}
		gpio_set_SDA6_mode(gpio_output);
    SDA6_L;
    I6Cdelay(1);
    SCL6_L;
    I6Cdelay(1);
    return TRUE;
}
/*********************************************
 * ��������I6CStop
 * ��  �����ͷ�I6C����
 * ��  �룺��
 * ��  ������
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
 * ��������I6CAck
 * ��  ��������ASK
 * ��  �룺��
 * ��  ������
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
 * ��������I6CNoAck
 * ��  ��������NOASK
 * ��  �룺��
 * ��  ������
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
 * ��������I6CWaitAck
 * ��  ������ȡACK�ź�
 * ��  �룺��
 * ��  ����TRUE=��ACK,FALSE=��ACK
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
 * ��������I6CSendByte
 * ��  ����MCU����һ���ֽ�
 * ��  �룺��
 * ��  ������
 ********************************************/
static void I6CSendByte(uint8_t SendByte) //���ݴӸ�λ����λ
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
 * ��������I6CReceiveByte
 * ��  ����MCU����һ���ֽ�
 * ��  �룺��
 * ��  ����ReceiveByte
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
        I6CNoAck();//����nACK
    else
        I6CAck(); //����ACK
    return ReceiveByte;   
}
/************************************************************************************/
//IICдһ���ֽ� 
//data:����
//����ֵ:TRUE,����
//    ����,�������
uint8_t TM1729_Write_Byte(uint8_t data) 				 
{ 
    I6CStart(); 
	I6CSendByte((uint8_t)(TM1729_ADDR<<1)|0);//����������ַ+д����	
	if(I6CWaitAck()  == FALSE)	//�ȴ�Ӧ��
	{
		I6CStop();		 
		return FALSE;		
	} 
	I6CSendByte(data);//��������
	if(I6CWaitAck()  == FALSE)	//�ȴ�ACK
	{
		I6CStop();	 
		return FALSE;		 
	}		 
  I6CStop();	 
	return TRUE;
}
/************************************************************************************/
//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
uint8_t TM1729_Read_Byte(uint8_t reg)
{
	uint8_t res_b;
    I6CStart(); 
	I6CSendByte((uint8_t)(TM1729_ADDR<<1)|0);//����������ַ+д����	
	I6CWaitAck();		//�ȴ�Ӧ�� 
    I6CSendByte(reg);	//д�Ĵ�����ַ
    I6CWaitAck();		//�ȴ�Ӧ��
    I6CStart();
	I6CSendByte((uint8_t)(TM1729_ADDR<<1)|1);//����������ַ+������	
    I6CWaitAck();		//�ȴ�Ӧ�� 
	res_b=I6CReceiveByte(0);//��ȡ����,����nACK 
    I6CStop();			//����һ��ֹͣ���� 
	return res_b;		
}
/************************************************************************************/
//IIC����д����ʾ����
//len:д�볤��
//buf:������
//����ֵ:TRUE,����
//    ����,�������
uint8_t TM1729_Write_DATA(uint8_t len,uint8_t *buf)
{
	uint8_t i,w_data; 
    I6CStart(); 
	I6CSendByte((uint8_t)(TM1729_ADDR<<1)|0);//����������ַ+д����	
	if(I6CWaitAck()  == FALSE)	//�ȴ�Ӧ��
	{
		I6CStop();		 
		return FALSE;		
	}
    I6CSendByte(0x00);	//д�Ĵ�����ַ
    I6CWaitAck();		//�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		w_data = buf[i];
		w_data <<= 4;
		i++;
		w_data = w_data|buf[i];
		I6CSendByte(w_data);	//��������
		if(I6CWaitAck()  == FALSE)		//�ȴ�ACK
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
//IIC����д���������
//len:д�볤��
//buf:������
//����ֵ:TRUE,����
//    ����,�������
uint8_t TM1729_Write_CMD(uint8_t len,uint8_t *buf)
{
	uint8_t i; 
    I6CStart(); 
	I6CSendByte((TM1729_ADDR<<1)|0);//����������ַ+д����	
	if(I6CWaitAck()  == FALSE)	//�ȴ�Ӧ��
	{
		I6CStop();		 
		return FALSE;		
	}
	for(i=0;i<len;i++)
	{
		I6CSendByte(buf[i]);	//��������
		if(I6CWaitAck()  == FALSE)		//�ȴ�ACK
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
  cm[0] = TM1729_ICSET|TM1729_ICSET_RESET;     //��λ
  cm[1] = TM1729_DISCTL|TM1729_SAVE_3_MODE|TM1729_POWER_SAVE_1|TM1729_FRAME_FLIP;
	retu = TM1729_Write_CMD(2,cm);
	return retu;
}

/************************************************************************************/
void LCD_CLOSE(void)
{
   Trace_log("�ر�Һ����ʾ\r\n");
   TM1729_Write_Byte(TM1729_MODESET);
}
/************************************************************************************/
void LCD_OPEN_ALL(void)
{
	Trace_log("��ʾȫ���ֶ�\r\n");
  TM1729_Write_Byte(TM1729_APCTL|TM1729_APCTL_ON);
}
/************************************************************************************/
void LCD_OFF_ALL(void)
{
	Trace_log("�ر�ȫ���ֶ�\r\n");
  TM1729_Write_Byte(TM1729_APCTL|TM1729_APCTL_OFF);
}
/************************************************************************************/
void TM1729_display_init(void)
{
	uint8_t cm[3];
	cm[0] = TM1729_BLKCTL|TM1729_BLKCTL_OFF;     //����˸����
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
uint8_t Retrieve_TM1729(uint16_t *p,uint8_t cnt)//����оͻ�1��
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
num_value  ��ʾ����ֵ
*buf_num   ��ʾ����
dip        С����λ��  
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
		num_value = num_value*10;  //��ȡһλ��Ϊ��������
		value_temp = (int32_t)num_value;  //����ȡ��
		value_num[5] = value_temp%10;   //��������λ
		value_temp = value_temp/10;
		if(value_num[5]>=5)
			value_temp = value_temp+1;
		value_num[4]= value_temp/10000;
		value_num[3]=	value_temp%10000/1000;
		value_num[2]= value_temp%1000/100;
		value_num[1]=	value_temp%100/10;
		value_num[0]= value_temp%10;
		switch(dip2)   //ȷ��С����λ��
		{
			case 0 : break;
			case 1 : buf_num[cnt++]=LCD_DP11; break;
			case 2 : buf_num[cnt++]=LCD_DP12; break;
			case 3 : buf_num[cnt++]=LCD_DP13; break;
			case 4 : buf_num[cnt++]=LCD_DP14; break;
			default : break;
		}
		j = 5;
		while(j>0)   //�ҳ���λ��Ϊ0����С����λ��
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
		while(j>=0)   //ȡ��ʾ����
		{			
			tmp1=Num_0_9[value_num[j]];
			for(i=0;i<7;i++)
			{
				if(tmp1&0x80)
					buf_num[cnt++]=Num_A[9+j][i];    //�ʻ���Ӧ��ֵ
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
		num_value = num_value*10;  //��ȡһλ��Ϊ��������  С��0ʱ   ����������ݶ�ȡ��һλ
		value_temp = (int32_t)num_value;  //����ȡ��
		value_num[5] = value_temp%10;   //��������λ
		value_temp = value_temp/10;
		if(value_num[5]>=5)
			value_temp = value_temp+1;
		value_num[3]=	value_temp%10000/1000;
		value_num[2]= value_temp%1000/100;
		value_num[1]=	value_temp%100/10;
		value_num[0]= value_temp%10;
		switch(dip2)   //ȷ��С����λ��
		{
			case 0 : break;
			case 1 : buf_num[cnt++]=LCD_DP11; break;
			case 2 : buf_num[cnt++]=LCD_DP12; break;
			case 3 : buf_num[cnt++]=LCD_DP13; break;
			case 4 : buf_num[cnt++]=LCD_DP14; break;
			default : break;
		}		
		
		tmp1=Signal[0];//tmp=0����ʾ-;tmp=1;��ʾ-1
		for(i=0;i<7;i++)
		{
				if(tmp1&0x80)
					buf_num[cnt++]=Num_A[13][i];    //�ʻ���Ӧ��ֵ
				else buf_num[cnt++]=0;
				tmp1<<=1;
		}
		j = 4;
		while(j>1)   //�ҳ���λ��Ϊ0����С����λ��
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
			
		while(j>=0)   //ȡ��ʾ����
		{			
			tmp1=Num_0_9[value_num[j]];
			for(i=0;i<7;i++)
			{
				if(tmp1&0x80)
					buf_num[cnt++]=Num_A[9+j][i];    //�ʻ���Ӧ��ֵ
				else buf_num[cnt++]=0;
				tmp1<<=1;
			}
			j--;
		}
	}
	return cnt ;
}


/************************************************************************************
Dis_unit  ��ʾ��λ
Dis_data   ��ʾ����
*time      ��ʾʱ��
Dis_signal     ��ʾ�ź�
vbet1      ��ʾ����
Dis_dip     ��ʾ���С��λ
************************************************************************************/
//void Dis_All(char Dis_unit,float Dis_data ,uint8_t *time ,uint8_t signal ,uint16_t vbet1 ,uint8_t error_flag)
void Dis_All(char Dis_unit,float Dis_data ,uint8_t *time ,uint8_t Dis_signal ,float vbet1,uint8_t Dis_dip)
{
  uint8_t i;
  uint16_t tmp;
  uint16_t cnt=0;
  uint8_t tmp1;
	uint8_t vbet;
	
  vbet=(uint8_t)vbet1;   ////  �� 100% 

	memset(Dis_abc, 0, sizeof(Dis_abc)); 
  memset(TM1729_BUFF, 0, sizeof(TM1729_BUFF)); 
	
	
  cnt=display_num_value(Dis_data,Dis_abc,Dis_dip);   //��ʾ����

  /***************��ʾ��λ*******************/		
   switch(Dis_unit)
	 {
		 case  UNIT_MPa    :		Dis_abc[cnt++]=LCD_DW_MPa; break;     //Mpa��λ																					
		 case  UNIT_KPa    :	  Dis_abc[cnt++]=LCD_DW_KPa; break;     //kpa��λ
		 case  UNIT_Pa     :    Dis_abc[cnt++]=LCD_DW_Pa;  break;     //pa��λ
		 case  UNIT_psf    : 	  Dis_abc[cnt++]=LCD_DW_psf; break;     //psf��λ
		 case  UNIT_psi    :		Dis_abc[cnt++]=LCD_DW_psi; break;     //psi��λ
		 case  UNIT_bar    :		Dis_abc[cnt++]=LCD_DW_bar; break;     //bar��λ
		 
		 case  UNIT_ppm    :	  Dis_abc[cnt++]=LCD_DW_ppm; break;     //ppm��λ
		 case  UNIT_ppb    :	  Dis_abc[cnt++]=LCD_DW_ppb; break;     //ppb��λ
		 
		 case  UNIT_V      :	  Dis_abc[cnt++]=LCD_DW_V;   break;     //V��λ
		 case  UNIT_mA     :	  Dis_abc[cnt++]=LCD_DW_mA;  break;     //mA��λ
		 
		 case  UNIT_mm     :	  Dis_abc[cnt++]=LCD_DW_mm;  break;     //mm��λ
		 case  UNIT_M      :		Dis_abc[cnt++]=LCD_DW_M;   break;     //M��λ
		 
		 case  UNIT_C      :	  Dis_abc[cnt++]=LCD_DW_C;   break;     //�浥λ 
		 case  UNIT_F      :	  Dis_abc[cnt++]=LCD_DW_F;   break;     //F��λ
	   case  UNIT_dp      :	  Dis_abc[cnt++]=LCD_DW_dp;  break;     //dp��λ	 
		 case  UNIT_BF      :		Dis_abc[cnt++]=LCD_DW_BF;  break;     //%��λ

		 case  UNIT_0      :    break;                              //����ʾ��λ
		
    default : break ; 
	}
/*******�ź���ʾ**********************/
	if(device_info.save.tab_module == TP307V1_1_A || device_info.save.tab_module == LORAWAN )  //lora
	{		
		 if(Dis_signal>70&&Dis_signal <90)   //�ź�
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
		 if(Dis_signal>0&&Dis_signal <9)    //�ź�
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
/***************������ʾ*******************/ 	 
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
/***************����־��ʾ*******************/ 
	 if(device_info.save.tab_model  == TP240XVX_3_P || device_info.save.tab_model  == TP240XVX_3_P_Ex)   //�ⲿ����ɳ��汾
	 {
		 if(device_info.waibu_pwr_24 == 1 )//|| device_info.waibu_pwr_usb == 1
			 Dis_abc[cnt++] = ZT_TB[0];
	 }
/***************״ָ̬ʾͼ����ʾ*******************/ 
	 if(device_info.gps_satues == 1)
		 Dis_abc[cnt++] = LCD_DW_DW;
//	 if(device_info.out_power == 1)
//		 Dis_abc[cnt++] = LCD_DW_CD;
/***************��������*******************/			 
	 for(i=0;i<4;i++)
	 {
		 Dis_abc[cnt++] = CL_TB[i];
	 }
/***************ʱ����ʾ*******************/
	  tmp=sMyTime.buff[1]/10;	    //min
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[7][i];    //�ʻ���Ӧ��ֵ
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
      if(tmp)//����Ƿ�0ֵ
      {
        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[5][i];    //�ʻ���Ӧ��ֵ
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
//					Dis_abc[cnt++]=Num_A[0][i];    //�ʻ���Ӧ��ֵ
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
					Dis_abc[cnt++]=Num_A[1][i];    //�ʻ���Ӧ��ֵ
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
					Dis_abc[cnt++]=Num_A[3][i];    //�ʻ���Ӧ��ֵ
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
    /*****ת����HT1623����ʾ�ڴ�***********/
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
 
/***************err״̬��ʾ*******************/				
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

			 
/*******�ź���ʾ**********************/
	if(device_info.save.tab_module == TP307V1_1_A || device_info.save.tab_module == LORAWAN )  //lora
	{		
		 if(Dis_signal>70&&Dis_signal <90)   //�ź�
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
		 if(Dis_signal>0&&Dis_signal <9)    //�ź�
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
/***************������ʾ*******************/ 	 
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
/***************����־��ʾ*******************/ 
	 if(device_info.save.tab_model  == TP240XVX_3_P || device_info.save.tab_model  == TP240XVX_3_P_Ex)   //�ⲿ����ɳ��汾
	 {
		 if(device_info.waibu_pwr_24 == 1 ) //|| device_info.waibu_pwr_usb == 1
			 Dis_abc[cnt++] = ZT_TB[0];
	 }
/***************״ָ̬ʾͼ����ʾ*******************/ 
	 if(device_info.gps_satues == 1)
		 Dis_abc[cnt++] = LCD_DW_DW;
//	 if(device_info.out_power == 1)
//		 Dis_abc[cnt++] = LCD_DW_CD;
/***************��������*******************/			 
	 for(i=0;i<4;i++)
	 {
		 Dis_abc[cnt++] = CL_TB[i];
	 }
/***************ʱ����ʾ*******************/			 
	  tmp=sMyTime.buff[1]/10;	    //min
    tmp1=Num_0_9[tmp];
    for(i=0;i<7;i++)
    {
        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[7][i];    //�ʻ���Ӧ��ֵ
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
      if(tmp)//����Ƿ�0ֵ
      {
        if(tmp1&0x80)
					Dis_abc[cnt++]=Num_A[5][i];    //�ʻ���Ӧ��ֵ
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
//					Dis_abc[cnt++]=Num_A[0][i];    //�ʻ���Ӧ��ֵ
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
					Dis_abc[cnt++]=Num_A[1][i];    //�ʻ���Ӧ��ֵ
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
					Dis_abc[cnt++]=Num_A[3][i];    //�ʻ���Ӧ��ֵ
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
    /*****ת����HT1623����ʾ�ڴ�***********/
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



