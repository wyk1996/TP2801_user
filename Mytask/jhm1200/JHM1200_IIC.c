#include "JHM1200_IIC.h"
#include "main.h"
//#include <stdlib.h>

#define PMIN    0      // 20000  //Full range sensor2data for example 20Kpa
#define PMAX    1.6   //  120000 //Zero Point sensor2data Value, for example 120Kpa
#define DMIN    1677722      // 1677722 //AD value corresponding to sensor2data zero, for example 10%AD  16777216  =2^24
#define DMAX    15099494      // 15099494 //AD Value Corresponding to Full sensor2data Range, for example 90%AD
///pt100�������¶ȵ�ʱ����
#define R0     100.0
#define R1     1000.0
#define COEFF_A                 3.9083e-3f
#define COEFF_B                 -5.775e-7f
#define COEFF_A_A               (COEFF_A*COEFF_A)
#define COEFF_4B_R0             (4*COEFF_B/R0)
#define COEFF_4B_R1             (4*COEFF_B/R1)
#define COEFF_2B                (2*COEFF_B)

#define IIC1_PORT  GPIOA
#define SDA1_PIN   LL_GPIO_PIN_6    //GPIOA
#define SCL1_PIN   LL_GPIO_PIN_5    //GPIOA

#define IIC2_PORT  GPIOA
#define SDA2_PIN   LL_GPIO_PIN_8    //GPIOA
#define SCL2_PIN   LL_GPIO_PIN_7    //GPIOA

#define IIC3_PORT  GPIOA
#define SDA3_PIN   LL_GPIO_PIN_10    //GPIOA
#define SCL3_PIN   LL_GPIO_PIN_9    //GPIOA

#define IIC4_PORT  GPIOB
#define SDA4_PIN   LL_GPIO_PIN_2    //
#define SCL4_PIN   LL_GPIO_PIN_1    //

#define IIC5_PORT  GPIOB
#define SDA5_PIN   LL_GPIO_PIN_4    //
#define SCL5_PIN   LL_GPIO_PIN_3    //

#define IIC6_PORT  GPIOB
#define SDA6_PIN   LL_GPIO_PIN_6   //
#define SCL6_PIN   LL_GPIO_PIN_5   //

//#define IIC1_SCL_H()   LL_GPIO_SetOutputPin(IIC1_PORT,SCL1_PIN) //���
//#define IIC1_SCL_L()   LL_GPIO_ResetOutputPin(IIC1_PORT,SCL1_PIN) //���
//#define IIC1_SDA_H()   LL_GPIO_SetOutputPin(IIC1_PORT,SDA1_PIN)  //��� 
//#define IIC1_SDA_L()   LL_GPIO_ResetOutputPin(IIC1_PORT,SDA1_PIN)   
//#define SDA1_read      LL_GPIO_IsInputPinSet(IIC1_PORT, SDA1_PIN)

//#define IIC2_SCL_H()   LL_GPIO_SetOutputPin(IIC2_PORT,SCL2_PIN) //���
//#define IIC2_SCL_L()   LL_GPIO_ResetOutputPin(IIC2_PORT,SCL2_PIN) //���
//#define IIC2_SDA_H()   LL_GPIO_SetOutputPin(IIC2_PORT,SDA2_PIN)  //��� 
//#define IIC2_SDA_L()   LL_GPIO_ResetOutputPin(IIC2_PORT,SDA2_PIN)   
//#define SDA2_read      LL_GPIO_IsInputPinSet(IIC2_PORT, SDA2_PIN)

//#define IIC3_SCL_H()   LL_GPIO_SetOutputPin(IIC3_PORT,SCL3_PIN) //���
//#define IIC3_SCL_L()   LL_GPIO_ResetOutputPin(IIC3_PORT,SCL3_PIN) //���
//#define IIC3_SDA_H()   LL_GPIO_SetOutputPin(IIC3_PORT,SDA3_PIN)  //��� 
//#define IIC3_SDA_L()   LL_GPIO_ResetOutputPin(IIC3_PORT,SDA3_PIN)   
//#define SDA3_read      LL_GPIO_IsInputPinSet(IIC3_PORT, SDA3_PIN)

//#define IIC4_SCL_H()   LL_GPIO_SetOutputPin(IIC4_PORT,SCL4_PIN) //���
//#define IIC4_SCL_L()   LL_GPIO_ResetOutputPin(IIC4_PORT,SCL4_PIN) //���
//#define IIC4_SDA_H()   LL_GPIO_SetOutputPin(IIC4_PORT,SDA4_PIN)  //��� 
//#define IIC4_SDA_L()   LL_GPIO_ResetOutputPin(IIC4_PORT,SDA4_PIN) 
//#define SDA4_read      LL_GPIO_IsInputPinSet(IIC4_PORT, SDA4_PIN)

//#define IIC5_SCL_H()   LL_GPIO_SetOutputPin(IIC5_PORT,SCL5_PIN) //���
//#define IIC5_SCL_L()   LL_GPIO_ResetOutputPin(IIC5_PORT,SCL5_PIN) //���
//#define IIC5_SDA_H()   LL_GPIO_SetOutputPin(IIC5_PORT,SDA5_PIN)  //��� 
//#define IIC5_SDA_L()   LL_GPIO_ResetOutputPin(IIC5_PORT,SDA5_PIN) 
//#define SDA5_read      LL_GPIO_IsInputPinSet(IIC5_PORT, SDA5_PIN)

//#define IIC6_SCL_H()   LL_GPIO_SetOutputPin(IIC6_PORT,SCL6_PIN) //���
//#define IIC6_SCL_L()   LL_GPIO_ResetOutputPin(IIC6_PORT,SCL6_PIN) //���
//#define IIC6_SDA_H()   LL_GPIO_SetOutputPin(IIC6_PORT,SDA6_PIN)  //��� 
//#define IIC6_SDA_L()   LL_GPIO_ResetOutputPin(IIC6_PORT,SDA6_PIN) 
//#define SDA6_read      LL_GPIO_IsInputPinSet(IIC6_PORT, SDA6_PIN)

//PT100���Ը��¶ȵ�ʱ����

GPIO_Type *GPIO_PORT;
uint32_t IIC_SCL;
uint32_t IIC_SDA;

const float cPPolyCoeff[7] ={
    -242.02f, 2.2228f, 0.0025859f, -0.0000048260f, -0.000000028183f, 0.00000000015243f, 1.0f
};
#define POLY_CALC(retVal, inVal, coeff_array) \
{ \
    float expVal = 1.0f; \
    const float* coeff = coeff_array; \
    retVal = 0.0f; \
    while(*coeff != 1.0f)\
    { \
        retVal += *coeff * expVal; \
        expVal *= inVal; \
        coeff++; \
    }\
}

/***********************************************************/
void delay(uint8_t us)
{
		//1000 WT+����104��      //te120-170  //ȥ���� TE������
   uint16_t i=200; //��������Ż��ٶȣ�ͨ����ʱ3~10us��������ʾ����������������
	uint8_t j;
	for(j=0;j<us;j++)
	{
   while(i)     //���Ż���i=100
   { 
     i--; 
   }
 }	 
}
/***********************************************************/
void delay_10pus(uint16_t us)   //8MHZ��   10+us��ʱus
{
	uint16_t i;
	for(i=0;i<(us);i++)
	{ __nop();	
		__nop();
		__nop();
  	__nop();
	}	 
}
/***********************************************************/
static void CHANNEL_IIC_init(void)
{
	LL_GPIO_InitTypeDef GPIO_Initure;

	GPIO_Initure.Pin=IIC_SCL|IIC_SDA;
	GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
	GPIO_Initure.Pull=DISABLE;          //����
	GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;    
	LL_GPIO_Init(GPIO_PORT,&GPIO_Initure);	
	LL_GPIO_SetOutputPin(GPIO_PORT,IIC_SCL|IIC_SDA);			
}
/***********************�͹���ģʽ�¸�λIO************************************/
static void CHANNEL_IIC_deinit(void)   
{
	LL_GPIO_InitTypeDef GPIO_Initure;
 
	GPIO_Initure.Pin= IIC_SCL|IIC_SDA;
	GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
	GPIO_Initure.Pull=DISABLE;       
	GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;    
	LL_GPIO_Init(GPIO_PORT,&GPIO_Initure);		
}
/***********************************************************/
static void set_SDA_mode(uint32_t GPIO_MODE)
{
	LL_GPIO_InitTypeDef GPIO_Initure;
	
	GPIO_Initure.Pin=IIC_SDA;                       //SDA  
	GPIO_Initure.Mode	= GPIO_MODE;
	GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_Initure.Pull=DISABLE;          //����	    
	LL_GPIO_Init(GPIO_PORT,&GPIO_Initure);
}
/***********************************************************/
static void IIC_SCL_H(void)   
{
	LL_GPIO_SetOutputPin(GPIO_PORT,IIC_SCL); //���H
}
/***********************************************************/
static void IIC_SCL_L(void)   
{
	LL_GPIO_ResetOutputPin(GPIO_PORT,IIC_SCL); //���L
}
/***********************************************************/
static void IIC_SDA_H(void)   
{
	LL_GPIO_SetOutputPin(GPIO_PORT,IIC_SDA);  //���
}	
/***********************************************************/
static void IIC_SDA_L(void)   
{
	LL_GPIO_ResetOutputPin(GPIO_PORT,IIC_SDA);
}
/***********************************************************/
static uint32_t SDA_read_data(void)      
{
	return LL_GPIO_IsInputPinSet(GPIO_PORT,IIC_SDA);
}
/***********************************************************/
/***********************************************************/
uint8_t check_sum(uint8_t *sum,uint8_t sum_lang)   //�ۼӺ�У��
{
	uint8_t checksum=0;
	uint8_t i;
	for(i=0;i<sum_lang;i++)
	{
		checksum += sum[i];
	}
	return checksum;
}
/***********************************************************/
static uint8_t IIC_Start(void)
{
		IIC_SDA_H();
	  delay(5);	  
	  IIC_SCL_H();
	  delay(10);
	  set_SDA_mode(LL_GPIO_MODE_INPUT);
	  if(!SDA_read_data())
		{
			set_SDA_mode(LL_GPIO_MODE_OUTPUT);
			return FALSE;	//SDA��Ϊ�͵�ƽ������æ,�˳�
		}
		set_SDA_mode(LL_GPIO_MODE_OUTPUT);
	  IIC_SDA_L();//START:when CLK is high,DATA change form high to low 
  	delay(10);
	  IIC_SCL_L();//ǯסI2C���ߣ�׼�����ͻ�������� 
	  delay(5);
    return TRUE;
}
/***********************************************************/
static void IIC_Stop()
{
	//sda�����
  IIC_SCL_L();
	delay(5);
  IIC_SDA_L();//STOP:when CLK is high DATA change form low to high
 	delay(10);
	IIC_SCL_H();
	delay(10);
	IIC_SDA_H();//����I2C���߽����ź�
	delay(10);		
}
/***********************************************************/
static void IIC_Send_Byte(uint8_t txd)
{    uint8_t t;   
	 	       
    for(t=0;t<8;t++)
    {              
      IIC_SCL_L();     //����ʱ�ӿ�ʼ���ݴ���
	    delay(10);
			if(txd&0x80)
					IIC_SDA_H();
			else 
					IIC_SDA_L();     
			txd<<=1; 	  
			delay(10);  
			IIC_SCL_H();  
			delay(10); 
    }
		IIC_SCL_L();				
}
/***********************************************************/
static void IIC_Ack()
{
	 IIC_SCL_L();
		delay(10);
	 IIC_SDA_L();
	 delay(10);
	 IIC_SCL_H();
	 delay(10);
	 IIC_SCL_L();
	 delay(10);
	 IIC_SDA_H();
	
}
/***********************************************************/
static void IIC_NAck()
{
		IIC_SCL_L();
		delay(10);
		IIC_SDA_H();
		delay(10);
		IIC_SCL_H();
		delay(10);
		IIC_SCL_L();
		delay(10);
}

/***********************************************************/
static uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	
	IIC_SDA_H();	
  set_SDA_mode(LL_GPIO_MODE_INPUT);
  for(i=0;i<8;i++ )
	{
    IIC_SCL_L();  
    delay(10);
    IIC_SCL_H();  
    receive<<=1;
		delay(10);
		if(SDA_read_data())
		{
			receive |= 0x01;   
    }		
	}	
  set_SDA_mode(LL_GPIO_MODE_OUTPUT);	
	IIC_SCL_L();
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}

/***********************************************************/
static uint8_t IIC_Wait_Ack(void)
{
	uint16_t ucErrTime=0;      
	IIC_SCL_L();
	delay(10);
	IIC_SDA_H();
	delay(10);	
  set_SDA_mode(LL_GPIO_MODE_INPUT);	 //SDA����Ϊ����
	IIC_SCL_H();
	delay(10);	
	
	while(SDA_read_data())//== (SDA_PIN))GPIOC->DIN
	{
		ucErrTime++;
		if(ucErrTime>500)
		{
			set_SDA_mode(LL_GPIO_MODE_OUTPUT);
			IIC_Stop();
//			Trace_log("222\r\n");
			return FALSE;
		}
	}
	set_SDA_mode(LL_GPIO_MODE_OUTPUT);
	  IIC_SCL_L();//ʱ�����0 	   
	//	IIC2_SDA_H();
	return TRUE;  
	
}
/***********************************************************/
uint8_t read_D032(uint8_t port)
{	
	uint8_t i=0;
	uint8_t data[4];
	uint16_t da1,da2;
	float  P1=1638;        //P1=10%*16383-A type
	float  P2=13106.4;  			//P2=80%*16383-A type
	float sennsor_mode_flag;
	
	IIC_Start();
	IIC_Send_Byte(0x51);	
	if(IIC_Wait_Ack() == FALSE)
	{
		return FALSE;
	}	
  portDISABLE_INTERRUPTS();	
	for(i=0;i<3;i++)  //3
	{
		data[i]=IIC_Read_Byte(1);
	}
	data[i]=IIC_Read_Byte(0);
	delay(1);
	IIC_Stop();
	portENABLE_INTERRUPTS();
	
	if(data[0] == 0x00 && data[1] == 0x00 && data[2] == 0x00 && data[3] == 0x00)
		return FALSE;
	else if(data[0] == 0xff && data[1] == 0xff && data[2] == 0xff && data[3] == 0xff)
		return FALSE;
	
	
	//ԭʼֵ
		da2 =((uint16_t)(data[3]&0x3f)<<8)+(data[2]);       //0x3f->0011 1111
		da1 =((uint16_t)(data[1]<<3))+(data[0]>>5);
	sennsor_mode_flag = device_info.save.channel_range_h[port*2+4]-device_info.save.channel_range_l[port*2+4];   //��ȡ����
	device_info.channel_data[port*2+4] = (float)(((float) da2-P1)/ P2*sennsor_mode_flag);
	device_info.channel_data[port*2+4] = device_info.channel_data[port*2+4]  + device_info.save.channel_range_l[port*2+4];

	device_info.channel_data[port*2+5] =(float)da1*200/2047-51;
//	if(device_info.channel_data[port*2+4] > device_info.save.channel_range_h[port*2+4])    //���ݳ�����������ȡ����ֵ
//		device_info.channel_data[port*2+4] = device_info.save.channel_range_h[port*2+4];
//	
//	if(device_info.channel_data[port*2+4] < device_info.save.channel_range_l[port*2+4])      //���ݳ�����������ȡ����ֵ
//		device_info.channel_data[port*2+4] = device_info.save.channel_range_l[port*2+4];
	
	return TRUE;
}
/***********************************************************/
uint8_t read_D020(uint8_t port)     //����40MPa���������ݶ�ȡ
{	
	uint8_t i=0;
	uint8_t buf[4];
	uint16_t data1,data2;
	IIC_Start();
	IIC_Send_Byte(0x7F);	
	if(IIC_Wait_Ack() == FALSE)
	{
//		portENABLE_INTERRUPTS();
		return FALSE;
	}	
  portDISABLE_INTERRUPTS();	
	for(i=0;i<3;i++)  //3
	{
		buf[i]=IIC_Read_Byte(1);
	}
	buf[i]=IIC_Read_Byte(0);
	delay(1);
	IIC_Stop();
	portENABLE_INTERRUPTS();
	
	data1 = (buf[0]<<8)+buf[1];   //ѹ��
	data2 = (buf[2]<<8)+buf[3];   //�¶�

//	if(data1<10000)
//		MPAbuf.sensor2data = 0;
//	else
//		MPAbuf.sensor2data = (float)(data1-10000);      //(data1-10000)/40000*40;
//	
//	MPAbuf.sensor1data = (float)(data2-10000)/200-50;     //(data2-10000)/40000*200-50;

	
	return TRUE;
}
/***********************************************************/
/***********************************************************/
uint8_t read_D033(uint8_t port)   //�����´��������ݶ�ȡ   2021.06.07
{
	unsigned char buf_D033[6];
//	char bu[10];
	  uint8_t i=0;
	uint32_t da1,da2;
	float sennsor_mode_flag;
//	uint8_t busyflag,count;
	Trace_log("read D033\r\n");
	IIC_Start();
	IIC_Send_Byte(0xDA);	
	if(IIC_Wait_Ack() == FALSE)
	{
//		Trace_log("11\r\n");
		return FALSE;
	}	
	IIC_Send_Byte(0x06);
	IIC_Wait_Ack();
	IIC_Stop();
		
			
	IIC_Start();
	IIC_Send_Byte(0xDB);
	IIC_Wait_Ack();
	portDISABLE_INTERRUPTS();
	for(i=0;i<5;i++)  //
	{
		buf_D033[i] = IIC_Read_Byte(1);
		delay(5);
	}
	buf_D033[i] = IIC_Read_Byte(0);
	delay(5);
	IIC_Stop();
	portENABLE_INTERRUPTS();
	
	if(device_info.debug_en)
		uart_transmit_buf(&COM0,buf_D033,6);
	da1	= 0;
	da2 = 0;
	da1 = buf_D033[0];     //ѹ��
	da1 <<= 8;
	da1 += buf_D033[1];
	da1 <<= 8;
	da1 += buf_D033[2];
	
	da2 = buf_D033[3];    //�¶�
	da2 <<= 8;
	da2 += buf_D033[4];
	da2 <<= 8;
	da2 += buf_D033[5];
	
	if(da1 == 0 || da1 == 0xffffff || da2 == 0 || da2 == 0xffffff)
		return FALSE;
	sennsor_mode_flag = device_info.save.channel_range_h[port*2+4]-device_info.save.channel_range_l[port*2+4];   //��ȡ����	
//	sprintf(bu,"%d",da1);
//	Trace_log("ԭʼֵ:");
//	Trace_log(bu);
//	Trace_log("\r\n");
	device_info.channel_data[port*2+4] = (sennsor_mode_flag*((float)da1-838860.8)/6710886.4)+device_info.save.channel_range_l[port*2+4];
//	sprintf(bu,"%.4f",device_info.channel_data[port*2+4]);
//	Trace_log("ֵ:");
//	Trace_log(bu);
//	Trace_log("\r\n");
	if(da2>8388608)
	{
		device_info.channel_data[port*2+5] = (float)da2-16777216;
		device_info.channel_data[port*2+5] = device_info.channel_data[port*2+5]/65536+25;
	}
	else
		device_info.channel_data[port*2+5] =(float)da2/65536+25;
	
//	if(device_info.channel_data[port*2+4] > device_info.save.channel_range_h[port*2+4])    //���ݳ�����������ȡ����ֵ
//		device_info.channel_data[port*2+4] = device_info.save.channel_range_h[port*2+4];
//	
//	if(device_info.channel_data[port*2+4] < device_info.save.channel_range_l[port*2+4])      //���ݳ�����������ȡ����ֵ
//		device_info.channel_data[port*2+4] = device_info.save.channel_range_l[port*2+4];
	
	if(device_info.channel_data[port*2+5] > 100 || device_info.channel_data[port*2+5] < -40)    //���ݳ����������޻������ޣ����ݱ���
		return FALSE;
	return TRUE;
}
/***********************************************************/
/***********************************************************/
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
uint8_t read0_D020(void)    /////////////////�Ͼ�����
{
//	if(read45x6DO_CQ_D020() == TRUE)
//	{
//		if( MPAbuf.sensor1data==0 && MPAbuf.sensor2data==0)
//	  {
//			return FALSE;
//	  }

//		uint_to_unit();   //����ת��	
////    if(	MPAbuf.sensor1data < device_info.save.sensor1_range_l|| MPAbuf.sensor1data > device_info.save.sensor1_range_h)  //�¶������쳣   �򴫸�������
////		{
////			MPAbuf.sensor2data = 0;
////			MPAbuf.sensor1data = 0;
////			return FALSE;
////		}
//		return TRUE;
//	}	
//	MPAbuf.sensor2data = 0;
//	MPAbuf.sensor1data = 0;
	return FALSE;
}
/*******************************************************************************/
uint8_t read_D036(void)    /////////////////�°汾ҺλD036
{
//	if(read45x6DO_D06() == TRUE)  
//	{
////		MPAbuf.Dsensor1 =0x030A;  //�̶�����
//		
//		if( MPAbuf.sensor1data==0 && MPAbuf.sensor2data==0)
//	  {
//			return FALSE;
//	  }
//		uint_to_unit();   //����ת��	
////    if(	MPAbuf.sensor1data < device_info.save.sensor1_range_l || MPAbuf.sensor1data > device_info.save.sensor1_range_h)
////		{
////			MPAbuf.sensor2data = 0;
////	    MPAbuf.sensor1data = 0;
////			return FALSE;
////		}		
//		return TRUE;
//	}	
//	MPAbuf.sensor2data = 0;
//	MPAbuf.sensor1data = 0;
	return FALSE;
}
/*******************************************************************************/
/*******************************************************/	
static void uint_to_unit(uint8_t port)//��׼ΪMpa	
{
	float sennsor_mode_flag;
	
	sennsor_mode_flag = device_info.save.channel_range_h[port*2+4]-device_info.save.channel_range_l[port*2+4];   //��ȡ����
	
	if(device_info.save.eliminate_signal ==ON)     //С�ź��г�
	{
		if(device_info.save.channel_range_l[port*2+4]>=0 && device_info.channel_data[port*2+4] <(sennsor_mode_flag*0.005))  //
		{
			device_info.channel_data[port*2+4] =0;
		}
		else
		{
			if(device_info.channel_data[port*2+4]>0 && device_info.channel_data[port*2+4] <(sennsor_mode_flag*0.005))
				device_info.channel_data[port*2+4] =0;
			else if(device_info.channel_data[port*2+4]<0 && (-device_info.channel_data[port*2+4])<(sennsor_mode_flag*0.005))
				device_info.channel_data[port*2+4] =0;
		}
	}
  	
	if(device_info.save.channel_unit[port*2+4]==UNIT_MPa)     //ѹ���¶Ƚ��е�λת��
		device_info.channel_data[port*2+4] = device_info.channel_data[port*2+4]/1000;
	if(device_info.save.channel_unit[port*2+5]==UNIT_F)
		device_info.channel_data[port*2+5] = 32+device_info.channel_data[port*2+5]*1.8;
			
	//������������ֵ
	device_info.channel_data[port*2+4] = device_info.channel_data[port*2+4] + device_info.save.channel_c[port*2+4];	//data1����
	device_info.channel_data[port*2+5] = device_info.channel_data[port*2+5] + device_info.save.channel_c[port*2+5];  //data2����
	
	if(device_info.channel_data[port*2+4] > device_info.save.channel_range_h[port*2+4])    //���ݳ�����������ȡ����ֵ
		device_info.channel_data[port*2+4] = device_info.save.channel_range_h[port*2+4];
	
	if(device_info.channel_data[port*2+4] < device_info.save.channel_range_l[port*2+4])      //���ݳ�����������ȡ����ֵ
		device_info.channel_data[port*2+4] = device_info.save.channel_range_l[port*2+4];	
	
	
}

/***********************************************************************************/
/***********************************************************************************/


#if 1      //�����߲��ֳ���
/***********************************************************************************/
unsigned char checksum(unsigned char*_ptrbuf, unsigned char len)   //�ۼƺ�
{
	unsigned char sum = 0;
	unsigned char i = 0;
	for(i = 0; i < len; i++)
	{
		sum+=_ptrbuf[i];
	}
	return sum;
}
/***********************************************************************************/
float Q_rsqrt( float number )
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;
    x2 = number * 0.5F;
    y   = number;
    i   = * ( long * ) &y;   // evil floating point bit level hacking
    i   = 0x5f3759df - ( i >> 1 ); // what the fuck?
    y   = * ( float * ) &i;
    y   = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
    y   = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed
    return 1/y;
} 
/***********************************************************************************/
float Pt100_RToT(float rRtd)//�ü���ķ�ʱ��Ϊ400us
{
  float Pt100Data;
  if(rRtd > R0)
  {
   Pt100Data = (-COEFF_A + Q_rsqrt(COEFF_A_A - COEFF_4B_R0*(R0 - rRtd)))/COEFF_2B;
  }
  else
  {
    POLY_CALC(Pt100Data, rRtd, &cPPolyCoeff[0]);  
  }
  return Pt100Data;
}

/***********************************************************************************/
float Bytes_ToFloat(float Data, uint8_t *Bytes, uint8_t index) 
{
	union F32_Sep
	{
		float x;
		unsigned char s[4];
	};
	union F32_Sep p;
	
	if(index == 0)
	{
		p.s[0] = Bytes[0];
		p.s[1] = Bytes[1];
		p.s[2] = Bytes[2];
		p.s[3] = Bytes[3];
	}
	else
	{
		p.s[0] = Bytes[3];
		p.s[1] = Bytes[2];
		p.s[2] = Bytes[1];
		p.s[3] = Bytes[0];
	}
//	Trace_log1((char*)p.s, 4);
	return p.x;
	//return 4;
}

/***********************************************************************************/
uint8_t  read_one_wire(uint8_t port)  //�����߶�ȡ
{		
	uint16_t waittime=0;
	uint8_t i,j;
	uint8_t t1=0,t2=0;
	uint8_t su;
	int16_t th_value;
	uint32_t dada=0;
  uint8_t redatabuf[12];	
	float  P1=1638;        //P1=10%*16383-A type
	float  P2=13106.4;  			//P2=80%*16383-A type
	
	set_SDA_mode(LL_GPIO_MODE_OUTPUT);
	IIC_SDA_L();
//  delay_10pus(50);
	delay_vtask(3);
	IIC_SDA_H();
	delay_10pus(30);
	
	set_SDA_mode(LL_GPIO_MODE_INPUT);
	while(SDA_read_data())
	{
	  if( ++waittime>300)
			goto custom_err;
		//one_delay(1);
	}
	waittime=0;
	
	while( SDA_read_data() ==0)
	{
	  if( ++waittime>250)
			goto custom_err;
	//	one_delay(1);
	}
	waittime=0;
		while(SDA_read_data())
	{
	  if( ++waittime>250)
			goto custom_err;
	//	one_delay(1);
	}
	
//	vTaskSuspendAll(); 
  portDISABLE_INTERRUPTS();		
	for( j=0; j<12;j++)
	{
		redatabuf[j]=0;
		for (i=0;i<8;i++)
		{
			redatabuf[j]<<=1;
			t1=0;
			t2=0;
			while(SDA_read_data() == 0)
			{
				if(++t1 > 250)	
				{				
				  portENABLE_INTERRUPTS();		
//						xTaskResumeAll();
					goto  custom_err;		
				}
	   	//one_delay(1)
			}		
			while(SDA_read_data())
			{
				if(++t2 > 250)
				{     
				  portENABLE_INTERRUPTS();	
//						xTaskResumeAll();
					goto  custom_err;
				}
				//one_delay(1);
			}		
			if(t2 > t1)
				redatabuf[j] |= 0x01;
		}

	}
//		xTaskResumeAll();
	portENABLE_INTERRUPTS();
	if(device_info.debug_en) 
	{
	uart_transmit_buf(&COM0, (uint8_t *)redatabuf,12);}
	j = check_sum(redatabuf,10);
	if(redatabuf[0] == 0x55 && redatabuf[11] == 0x0D && j == redatabuf[10])  //&& j == redatabuf[10]
  {
		switch(redatabuf[1])
		{
			case TP2305V2:
								device_info.save.port_one_wire_type[port] = TP2305V2;								
								th_value =  (int16_t)redatabuf[2] << 8 | redatabuf[3];
								device_info.channel_data[port*2+4] =  th_value/ 10.0;       //ʪ��
								device_info.save.channel_unit_p[port*2+4] = UNIT_0;
								th_value =  (int16_t)redatabuf[4] << 8 | redatabuf[5];
								device_info.channel_data[port*2+5] = th_value/ 10.0;        //�¶�
								device_info.save.channel_unit_p[port*2+5] = UNIT_C;
								break;
/*			case TP2302V2:					
								if(redatabuf[2] == 0x00 && redatabuf[3] == 0x00)
									return FALSE;
								MPAbuf.sensor1data = Pt100_RToT((redatabuf[2] * 256 + redatabuf[3]) / 100.0);   ////////////////
			          if(MPAbuf.sensor1data<-200)
									goto custom_err;
								break;
			case 0x03: goto custom_err;
//								MPAbuf.sensor2data = (redatabuf[2] * 256 + redatabuf[3])*0.02 - 273.15;
//								MPAbuf.sensor1data = (redatabuf[4] * 256 + redatabuf[5])*0.02 - 273.15;
//								break;
			case 0x06:
								MPAbuf.sensor1data = Bytes_ToFloat(MPAbuf.sensor2data, &redatabuf[2], 1)*10;
								MPAbuf.sensor2data = 0;
								break;			
			case 0x0c:   ///PPM  
								th_value =  (int16_t)redatabuf[2] << 8 | redatabuf[3];
								MPAbuf.sensor2data =  th_value;      //ppm
								th_value =  (int16_t)redatabuf[4] << 8 | redatabuf[5];
								MPAbuf.sensor1data = th_value/ 100.0;  //c
								break;
			case 0x0D:   ///PPM  
								th_value =  (int16_t)redatabuf[2] << 8 | redatabuf[3];
								MPAbuf.sensor2data =  th_value;      //ppm
								th_value =  (int16_t)redatabuf[4] << 8 | redatabuf[5];
								MPAbuf.sensor1data = th_value/ 100.0;  //c
								break;
			case 0x13 :    //////////������̼
								th_value =(int16_t )redatabuf[2]<<8 | redatabuf [3];     //�¶�    ʪ�Ȳ�����
								MPAbuf.sensor1data= th_value/10.0f;
								th_value = (uint16_t) redatabuf [6]<<8 |redatabuf [7];   //����ѹ��
								MPAbuf.sensor2data = th_value/10.0f;
								break  ;				
			case 0x14 :    ////������ѹ����ϵ��   
								MPAbuf.red45buf[3]=redatabuf[2];
								MPAbuf.red45buf[2]=redatabuf[3];
								MPAbuf.red45buf[1]=redatabuf[4];
								MPAbuf.red45buf[0]=redatabuf[5];
								if(( MPAbuf.red45buf[3]&0xc0)==0x00) // 0xc0 -> 1100 0000     00
								{
//									if( MPAbuf.red45buf[2]==0x00 && MPAbuf.red45buf[1]==0x00)
//									{				
//										return FALSE;
//									}
									MPAbuf.Dsensor2 =((uint16_t)(MPAbuf.red45buf[3]&0x3f)<<8)+(MPAbuf.red45buf[2]);       //0x3f->0011 1111
									MPAbuf.Dsensor1 =((uint16_t)(MPAbuf.red45buf[1]<<3))+(MPAbuf.red45buf[0]>>5);	
								}							
								break;
			case 0x15:
								dada = redatabuf[3];
								dada <<= 8;
								dada += redatabuf[4];
								dada <<= 8;
								dada += redatabuf[5];
								MPAbuf.sensor2data = (float)dada;
								dada = redatabuf[6];
								dada <<= 8;
								dada += redatabuf[7];
								MPAbuf.sensor1data = (float)dada;
								break;*/
			default : return FALSE;
		}		
    return TRUE;
  }
	custom_err:		
	return FALSE;
}


/***********************************************************************************/
uint8_t monobus_sensor(void)      //�������豸  ����ѹ���ӿ�iic4 �ӿ�
{
	uint8_t  i=0;
	uint8_t  redbuf=5;
//  danzong_init();
////	delay_vtask(1000);
//	while(redbuf--)                           
//	{	
//	  i= DQ_tp100();
////		i = is_two_sensor_tp100();
//		if(i == TRUE)
//		{
//			if(MPAbuf.sensor2data == 0 && MPAbuf.sensor1data == 0 && MPAbuf.Dsensor1 == 0 && MPAbuf.Dsensor2 == 0)
//			{
//         i = FALSE;
//			}
//			else
//			{
//				uint_to_unit();
//				break;
//			}
//		}
//	  delay_vtask(300);
//	}
//	if(i == FALSE)
//	{
//		MPAbuf.sensor2data = 0;
//	  MPAbuf.sensor1data = 0;
//	}		
  return  i;
}


#endif 
/***********************************************************************************/
uint8_t fangtiao(uint8_t ch)
{
	uint8_t re=TRUE;
	float da;
	
	if(device_info.channel_data_f[ch] == 0)
		return re;
	
	da = device_info.channel_data_f[ch]*0.1;
	if(fabs(device_info.channel_data[ch]-device_info.channel_data_f[ch])>da)   //������������  �豸�ض�
		re = FALSE;
	else
		device_info.channel_data_f[ch] = device_info.channel_data[ch];
	
	return re;
}
/***********************************************************************************/
void alarm_check(uint8_t ch)     //���ݳ����ж�
{	
	if(device_info.channel_data[ch] > device_info.save.channel_up[ch] && (device_info.alarm_flag_STATUS.ch_status[ch]&alarm_channel_up) != alarm_channel_up)    //����1������
	{
		device_info.alarm_flag_STATUS.ch_status[ch] |= alarm_channel_up;    //����״̬�ı�
		device_info.alarm_flag_ID.ch_status[ch] = 1;   //channel�б�������
		device_info.alarming_flag++;
		Trace_log("��������\r\n");
	}
	else if(device_info.channel_data[ch] < device_info.save.channel_down[ch] && (device_info.alarm_flag_STATUS.ch_status[ch]&alarm_channel_down) != alarm_channel_down)    //����1������
	{
		device_info.alarm_flag_STATUS.ch_status[ch] |= alarm_channel_down;
		device_info.alarm_flag_ID.ch_status[ch] = 1;
		device_info.alarming_flag++;
		Trace_log("��������\r\n");
	}
	else if(fabs(device_info.channel_data[ch]- device_info.channel_data_f[ch])> device_info.save.channel_f[ch] && (device_info.alarm_flag_STATUS.ch_status[ch]&alarm_channel_f) != alarm_channel_f)   //��������
	{
		device_info.alarm_flag_STATUS.ch_status[ch] |= alarm_channel_f;
		device_info.alarm_flag_ID.ch_status[ch] = 1;
		device_info.alarming_flag++;
		Trace_log("����������ֵ\r\n");
	}
		
	if((device_info.alarm_flag_STATUS.ch_status[ch]&alarm_channel_up) == alarm_channel_up)        //���ݳ����޻ָ�
	{
		if(device_info.channel_data[ch] < (device_info.save.channel_up[ch]-device_info.save.channel_hc[ch]))
		{			
			device_info.alarm_flag_STATUS.ch_status[ch] &= (~alarm_channel_up);
			device_info.alarm_flag_ID.ch_status[ch] = 1;
			device_info.alarming_flag++;
			Trace_log("�������޻ָ�\r\n");
		}
	}
	else if((device_info.alarm_flag_STATUS.ch_status[ch]&alarm_channel_down) == alarm_channel_down)   //���ݳ����޻ָ�
	{
		if(device_info.channel_data[ch] > (device_info.save.channel_down[ch]+device_info.save.channel_hc[ch]))
		{			
			device_info.alarm_flag_STATUS.ch_status[ch] &= (~alarm_channel_down);
			device_info.alarm_flag_ID.ch_status[ch] = 1;
			device_info.alarming_flag++;
			Trace_log("�������޻ָ�\r\n");
		}
	}
}
/***********************************************************************************/
static uint8_t port_read(uint8_t port)
{
	uint8_t re=FALSE;
	uint8_t i,j;
	
	switch(port)    //��ѯͨ������
	{
		case 0: GPIO_PORT=IIC1_PORT;IIC_SCL=SCL1_PIN;IIC_SDA=SDA1_PIN;break;
		case 1: GPIO_PORT=IIC2_PORT;IIC_SCL=SCL2_PIN;IIC_SDA=SDA2_PIN;break;
		case 2: GPIO_PORT=IIC3_PORT;IIC_SCL=SCL3_PIN;IIC_SDA=SDA3_PIN;break;
		case 3: GPIO_PORT=IIC4_PORT;IIC_SCL=SCL4_PIN;IIC_SDA=SDA4_PIN;break;
		case 4: GPIO_PORT=IIC5_PORT;IIC_SCL=SCL5_PIN;IIC_SDA=SDA5_PIN;break;
		case 5: GPIO_PORT=IIC6_PORT;IIC_SCL=SCL6_PIN;IIC_SDA=SDA6_PIN;break;
		default: return re;
	}
	i = 0;   
	j = 0;
	while(1)   //ÿ��ͨ����ȡʧ�ܻ��ض�5��
	{
		switch(device_info.save.port_sensor_type[port])    //��ȡͨ������
		{
			case One_wire: re=read_one_wire(port);break;
			case D032: re=read_D032(port);break;
			case D033: re=read_D033(port);break;
			case D020: re=read_D020(port);break;
			case DS18B20: re=read_DS18B20();break;
			default: return re;
		}
		if(re == TRUE)
		{
			uint_to_unit(port);   //���ݻ��㴦��
			if(fangtiao(port*2+4) == FALSE)
				j++;
			else
				break;
			if(j>3)   //��������3���ж�
				break;
		}
		else   //��ȡʧ��
			i++;
		
		if(i>5)    //ʧ���ض�5��
			break;
		delay_vtask(200);
	}
	return re;
}
/***********************************************************************************/
void port_data_get(void)
{
	uint8_t i;
	uint8_t res=TRUE;
	uint8_t buf[20];
	for(i=0;i<6;i++)
	{
		if(device_info.save.port_num>=(i+1))
			res = port_read(i);
		sprintf((char *)buf,"read port%d:",i+1);
		Trace_log(buf);
		if(res == FALSE)    //�ӿ�port��������ȡʧ��
		{
			Trace_log("error\r\n");
			device_info.port_err_num[i]++;
			if(device_info.port_err_num[i]>=device_info.save.sensorerr_num)   //�ӿڴ�������ȡʧ�ܴ�������Ԥ�������ʧ�ܴ���
			{
				device_info.port_err_num[i] = device_info.save.sensorerr_num;
				device_info.alarm_flag_STATUS.ch_status[i*2+4] = alarm_channel_err;                  //�ӿڴ���������״̬��־λ��
				device_info.alarm_flag_STATUS.ch_status[i*2+5] = alarm_channel_err;
			}
			else
			{
				device_info.channel_data[i*2+4] = device_info.channel_data_f[i*2+4];     //������һ������
				device_info.channel_data[i*2+5] = device_info.channel_data_f[i*2+5];
			}
		}
		else
		{
			Trace_log("ok\r\n");
			device_info.port_err_num[i] = 0;
			alarm_check(i*2+4);    //�����ж�
			alarm_check(i*2+5);    //�����ж�
		}
	}	
}


