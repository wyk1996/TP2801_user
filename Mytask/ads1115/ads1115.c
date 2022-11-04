#include "ads1115.h"
#include "main.h"

extern uart_t uart2;
//��ͨ����
#define  AIN0      0XC5    //����ת�����ػ�ת��
#define  AIN1      0XD5
#define  AIN2      0XE5  
#define  AIN3      0XF5


//˫ͨ������
#define  AIN01      0X85    // ����ת�����ػ�ת��
#define  AIN03      0X95
#define  AIN13      0XA5
#define  AIN23      0XB5


#define TYPE_V_0   0
#define TYPE_V_1   1
#define TYPE_A     2 

ad_t ad_info[3];

uint8_t ads1115_write(uint8_t id,uint8_t regaddr,uint8_t byte1,uint8_t byte2)
{
	IIC1_Start();
	
	IIC1_Send_Byte(id);
	if(IIC1_Wait_Ack())
	{
		Trace_log("����д��ģʽʧ��\r\n");
//		uart_transmit_str(&uart2,"����д��ģʽʧ��\r\n");
		IIC1_Stop();
		return 1;
	}
	IIC1_Send_Byte(regaddr);
	if(IIC1_Wait_Ack())
	{
		Trace_log("���üĴ�����ַʧ��\r\n");
//		uart_transmit_str(&uart2,"���üĴ�����ַʧ��\r\n");
		IIC1_Stop();
		return 1;
	}
	IIC1_Send_Byte(byte1);
	if(IIC1_Wait_Ack())
	{
		Trace_log("д�����1ʧ��\r\n");
//			uart_transmit_str(&uart2,"д�����1ʧ��\r\n");
		IIC1_Stop();
		return 1;
	}
	/*��������*/
	IIC1_Send_Byte(byte2);
	if(IIC1_Wait_Ack())
	{
		Trace_log("д�����2ʧ��\r\n");
//		uart_transmit_str(&uart2,"д�����2ʧ��\r\n");
		IIC1_Stop();
		return 1;
	}
	IIC1_Stop();
	IIC1_delay(4);
	return 0;
}
/**********************************************************************************************************
*	�� �� ��: ads1115_read
*	����˵��: ��ȡads1115�Ĵ���ֵ
*	��    ��: �Ĵ�����ַ���Ĵ�������ָ��
*	�� �� ֵ: ��
**********************************************************************************************************/
void ads1115_read(uint8_t regaddr,uint8_t *ad_raw)
{
	//vTaskDelay(50);
	IIC1_delay(4);
	IIC1_Start();
	
	IIC1_Send_Byte(0X90);
	if(IIC1_Wait_Ack())
	{
			Trace_log("����д��ģʽʧ��\r\n");
	}	
	
	IIC1_Send_Byte(regaddr);
	if(IIC1_Wait_Ack())
	{
			Trace_log("���ö�ȡ��ַʧ��");
	}
	IIC1_Stop();
	
	IIC1_delay(4);	
	IIC1_delay(4);	
	IIC1_delay(4);
	
	IIC1_Start();
	
	IIC1_Send_Byte(0X91);
	
	if(IIC1_Wait_Ack())
	{
		Trace_log("���ö�ȡģʧ��");
	}
	ad_raw[0] = IIC1_Read_Byte(1);
//LL_GPIO_SetPinMode(GPIOB, SDA1_PIN, LL_GPIO_MODE_INPUT);
//LL_GPIO_SetPinOutputType(GPIOB, SDA1_PIN,LL_GPIO_OUTPUT_OPENDRAIN);
///LL_mDelay(50);
	ad_raw[1] = IIC1_Read_Byte(1);	
	//LL_GPIO_SetPinMode(GPIOB, SDA1_PIN,LL_GPIO_MODE_OUTPUT);
//	LL_GPIO_SetPinOutputType(GPIOB, SDA1_PIN,LL_GPIO_OUTPUT_OPENDRAIN);
	IIC1_Stop();
	
}

//float  raw_voltage22;
void ads1115_get_value(uint8_t channel, uint8_t type)
{
	  char adc_value_buf_f[13];
    uint8_t ch_reg;
    uint8_t raw_value[2];
    uint16_t temp;
    float  raw_voltage;
	
//	if( channel&0x01 )   //ֻ֧�ֵ�ͨ����  ���ö�ͨ����ѯ��ȡ��
//	{
//	 ch_reg = AIN1;
//		type=TYPE_V_0;
//	}
//	else if( (channel>>1 )&0x01 )
//	{
//	  ch_reg = AIN0;
//		type=TYPE_V_1;
//	}
//	else if( (channel>>2 )&0x01 )
//	{
//	 ch_reg  = AIN23; 
//		type=TYPE_A;
//	}
//	

	for( uint8_t i=0; i<3;i++)
	{   
		 if(channel&0x01 )
		 { 
			
			 switch(i)
			 {
				case  0:  	 
	      ch_reg = AIN1;
       	type=TYPE_V_0;
				break;
				case  1:			 
					ch_reg = AIN0;
		      type=TYPE_V_1;	
				break;				
				case  2:		
					ch_reg  = AIN23; 
	      	type=TYPE_A;	
				break;
				default:break;				
			 }
	 		if(ads1115_write(0X90, 0X01, ch_reg, 0X43))  //32SPS  
			{
				return ;
			}
			if(ads1115_write(0X90, 0X03, 0X80, 0X00))
			{
			return ;
			}
			if(ads1115_write(0X90, 0X02, 0X7F, 0X00))
			{
			return ;
			}	
//			LL_mDelay(100);
			ads1115_read(0X00, raw_value);
			temp = raw_value[0] << 8 | raw_value[1];		
			if(temp > 0X8000)
			{ temp = 0;
			}
			raw_voltage = temp * 0.0000625;		
			if(type == TYPE_A)
			{			
					MPAbuf.sensor2data =   ad_info[type].value[0] = raw_voltage / 0.091;  /* ����ת�� */
			}
			else 
			{ 
					MPAbuf.sensor2data = ad_info[type].value[0] = raw_voltage*2; 
				//  ad_info[channel].value[type] = raw_voltage / (5.6/15.6);  /* ��ѹת�� */
			}	
			if(device_info.debug_en) 
			{
			sprintf(adc_value_buf_f, "%d��%0.5f\r\n" , type,ad_info[type].value[0]);
			uart_transmit_buf(&uart0, (uint8_t *)adc_value_buf_f,12);  
			}
		 }
		 

		 
	 channel=channel>>1;
	}
 	
//		if(ads1115_write(0X90, 0X01, ch_reg, 0X43))  //32SPS  
//		{
//			return ;
//		}
//  	if(ads1115_write(0X90, 0X03, 0X80, 0X00))
//  	{
//		return ;
//	  }
//    if(ads1115_write(0X90, 0X02, 0X7F, 0X00))
//	  {
//		return ;
//	  }	
//    LL_mDelay(100);
//    ads1115_read(0X00, raw_value);
//    temp = raw_value[0] << 8 | raw_value[1];		
//    if(temp > 0X8000)
//    { temp = 0;
//    }
//    raw_voltage = temp * 0.0000625;		
//    if(type == TYPE_A)
//    {			
//      	MPAbuf.Pressure =   ad_info[type].value[0] = raw_voltage / 0.091;  /* ����ת�� */
//    }
//    else 
//    { 
//      	MPAbuf.Pressure = ad_info[type].value[0] = raw_voltage*2; 
//      //  ad_info[channel].value[type] = raw_voltage / (5.6/15.6);  /* ��ѹת�� */
//    }	
//		if(device_info.debug_en) 
//		{
//    sprintf(adc_value_buf_f, "%d��%0.5f\r\n" , type,ad_info[type].value[0]);
//    uart_transmit_buf(&uart2, (uint8_t *)adc_value_buf_f,12);  
//		}

}




