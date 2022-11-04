#include "myiic.h"
#include "main.h"

//ADS1115ʹ�õ�iic
////M0 û��λ����  ���޷�ʹ��λ������ 
#define SDA1_PIN   LL_GPIO_PIN_9
#define SCL1_PIN   LL_GPIO_PIN_8

#define IIC1_SCL_H()  LL_GPIO_SetOutputPin(GPIOA,SCL1_PIN)  //���
#define IIC1_SCL_L()  LL_GPIO_ResetOutputPin(GPIOA,SCL1_PIN)  //���

#define IIC1_SDA_H()  LL_GPIO_SetOutputPin(GPIOA,SDA1_PIN)   //��� 
#define IIC1_SDA_L()  LL_GPIO_ResetOutputPin(GPIOA,SDA1_PIN)



void IIC1_delay(uint8_t us)
{
		for(uint8_t i=0;i<20;i++)
	{
		__nop();
	}
}


void ADS1115_IIC1_init(void)
{
    LL_GPIO_InitTypeDef GPIO_Initure;

    GPIO_Initure.Pin=SDA1_PIN;
    GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
    GPIO_Initure.Pull=DISABLE;          //����
		GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;   
    LL_GPIO_Init(GPIOA,&GPIO_Initure);
   
	  GPIO_Initure.Pin=SCL1_PIN;
    GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
    GPIO_Initure.Pull=DISABLE;       
		GPIO_Initure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;    
    LL_GPIO_Init(GPIOA,&GPIO_Initure);
   
	  IIC1_SDA_H();
	  IIC1_SCL_H();
}
void ADS1115_IIC1_deinit(void)
{
    LL_GPIO_InitTypeDef GPIO_Initure;

    GPIO_Initure.Pin=SDA1_PIN;
    GPIO_Initure.Mode=LL_GPIO_MODE_ANALOG;   
    GPIO_Initure.Pull=DISABLE;          //����
		GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;   
    LL_GPIO_Init(GPIOA,&GPIO_Initure);
   
	  GPIO_Initure.Pin=SCL1_PIN;
    GPIO_Initure.Mode=LL_GPIO_MODE_ANALOG;   
    GPIO_Initure.Pull=DISABLE;       
		GPIO_Initure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;    
    LL_GPIO_Init(GPIOA,&GPIO_Initure);
}

void IIC1_Start(void)
{
		IIC1_SDA_H();
	  IIC1_SCL_H();
	  IIC1_delay(4);
	  IIC1_SDA_L();//START:when CLK is high,DATA change form high to low 
  	IIC1_delay(4);
	  IIC1_SCL_L();//ǯסI2C���ߣ�׼�����ͻ�������� 
}
void IIC1_Stop(void)
{
	//sda�����
  IIC1_SCL_L();
  IIC1_SDA_L();//STOP:when CLK is high DATA change form low to high
 	IIC1_delay(4);
	IIC1_SCL_H();
	IIC1_SDA_H();//����I2C���߽����ź�
	IIC1_delay(4);		
}
void IIC1_Send_Byte(uint8_t txd)
{    uint8_t t;   
	 	    
    IIC1_SCL_L();     //����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
       // IIC_SDA=(txd&0x80)>>7;
			  if(txd&0x80)
					  IIC1_SDA_H();
				else 
					  IIC1_SDA_L();
      
				txd<<=1; 	  
		IIC1_delay(2);   //��TEA5767��������ʱ���Ǳ����
	  IIC1_SCL_H();  
		IIC1_delay(2); 
    IIC1_SCL_L();  
		IIC1_delay(2);
    }	
}
void IIC1_Ack(void)
{
 IIC1_SCL_L();	
//	IIC1_delay(2);
 IIC1_SDA_L();
 IIC1_delay(2);
 IIC1_SCL_H();
// IIC1_delay(2);
 //IIC1_SDA_H();	
	IIC1_delay(2);
 IIC1_SCL_L();
	IIC1_SDA_H();	
}
void IIC1_NAck(void)
{
  IIC1_SCL_L();	
	IIC1_delay(2);
  IIC1_SDA_H();
	IIC1_delay(2);
  IIC1_SCL_H();
	IIC1_delay(2);
  IIC1_SCL_L();
	
	IIC1_delay(2);	

}

uint8_t IIC1_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	//SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC1_SCL_L();  
        IIC1_delay(2);
        IIC1_SCL_H();  
        receive<<=1;IIC1_delay(2); 
        if(READ_BIT(GPIOA->DIN, SDA1_PIN))//= (SDA_PIN))
					receive++;   
	  	
    }					 
    if (!ack)
        IIC1_NAck();//����nACK
    else
		{   
			IIC1_Ack(); //����ACK 
	  //	LL_GPIO_SetPinMode(GPIOB, SDA1_PIN,LL_GPIO_MODE_INPUT);
		}
		IIC1_SCL_L();  
    return receive;
}
uint8_t IIC1_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	//SDA_IN();      //SDA����Ϊ����  
	IIC1_SDA_H();
	IIC1_delay(1);	   
	IIC1_SCL_H();
	IIC1_delay(1);	
	//LL_GPIO_SetPinMode(GPIOB, SDA1_PIN,LL_GPIO_MODE_INPUT);
	while(READ_BIT(GPIOA->DIN, SDA1_PIN) )//== (SDA_PIN))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC1_Stop();
			return 1;
		}
	}
	IIC1_SCL_L();//ʱ�����0 	   
	return 0;  
}



