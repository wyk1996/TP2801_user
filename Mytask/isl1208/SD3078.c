#include "main.h"
#include "SD3078.h"

#define SCL_PIN    LL_GPIO_PIN_11
#define SDA_PIN    LL_GPIO_PIN_12

#define  SCL_H         LL_GPIO_SetOutputPin(GPIOA,SCL_PIN)
#define  SCL_L         LL_GPIO_ResetOutputPin(GPIOA,SCL_PIN)

#define  SDA_H         LL_GPIO_SetOutputPin(GPIOA,SDA_PIN)
#define  SDA_L         LL_GPIO_ResetOutputPin(GPIOA,SDA_PIN)

#define SDA_read       LL_GPIO_IsInputPinSet(GPIOA,SDA_PIN)

MY_TIME sMyTime;

int IntToBCD(int i) //ʮ����תBCD
{
	return (((i / 10) << 4) + ((i % 10) & 0x0f));
}
int BCDToInt(int bcd) //BCDתʮ����
{
	return (0xff & (bcd >> 4)) * 10 + (0xf & bcd);
}
 
//�Ƚ������ַ���ָ�����ȵ������Ƿ����
//����:s1,s2Ҫ�Ƚϵ������ַ���;len,�Ƚϳ���
//����ֵ:1,���;0,�����
uint8_t my_strcmp(uint8_t*s1,uint8_t*s2,uint8_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)if((*s1++)!=*s2++)return 0;
	return 1;	   
}

/*********************************************
 * ��������I2Cdelay
 * ��  ����I2C��ʱ����
 * ��  �룺��
 * ��  ������
 ********************************************/
static void I2Cdelay(void)
{	
   uint8_t i=100; //��������Ż��ٶȣ�ͨ����ʱ3~10us��������ʾ����������������
   while(i) 
   { 
     i--; 
   }
}

/*********************************************
 * ��������IIC_Init
 * ��  ����I2C�˿ڳ�ʼ��
 * ��  �룺��
 * ��  ������
 ********************************************/

void SD3078_IIC3_init(void )
{
	  LL_GPIO_InitTypeDef GPIO_Initure;

    GPIO_Initure.Pin=SCL_PIN;
    GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
    GPIO_Initure.Pull=DISABLE;          
		GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;   
    LL_GPIO_Init(GPIOA,&GPIO_Initure);
   
	  GPIO_Initure.Pin=SDA_PIN;
    GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
    GPIO_Initure.Pull=DISABLE;       
		GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;    
    LL_GPIO_Init(GPIOA,&GPIO_Initure);
   
	SCL_H;
	SDA_H;
}
void SD3078_IIC3_deinit(void )    //���ⲿ��������
{
	  LL_GPIO_InitTypeDef GPIO_Initure;

    GPIO_Initure.Pin=SCL_PIN;
    GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
    GPIO_Initure.Pull=ENABLE;          
		GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;   
    LL_GPIO_Init(GPIOA,&GPIO_Initure);
   
	  GPIO_Initure.Pin=SDA_PIN;
    GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
    GPIO_Initure.Pull=ENABLE;       
		GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;    
    LL_GPIO_Init(GPIOA,&GPIO_Initure);
   
	SCL_H;
	SDA_H;
}
static void gpio_set_SDA_mode(uint8_t mode)
{
	LL_GPIO_InitTypeDef GPIO_Initure;
	if(mode == gpio_input)
		GPIO_Initure.Mode=LL_GPIO_MODE_INPUT;
	else if(mode == gpio_output)
	{
		GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT; 		
	}
	
	GPIO_Initure.Pin = SDA_PIN;                       //SDA  
	GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_Initure.Pull=DISABLE;          //����	    
	LL_GPIO_Init(GPIOA,&GPIO_Initure);
}
//rtc�ж�����
//void rtc_extioinit(void)
//{
//    LL_GPIO_InitTypeDef gpioInitStruct = {0};	

//    LL_CMU_SetEXTIClockSource(CMU_OPCCR1_EXTICKS);      // �ⲿ�ж�ʱ��ѡ��LSCLK
//    LL_CMU_EnableGroup3OperationClock(LL_CMU_GROUP3_OPCLK_EXTI);    // ʹ���ⲿ�ж�ʱ��		

//    // PB0�ж�����
//    gpioInitStruct.Pin = LL_GPIO_PIN_10;				
//    gpioInitStruct.Mode = LL_GPIO_MODE_INPUT;
//    gpioInitStruct.Pull = DISABLE;
//    LL_GPIO_Init(GPIOA, &gpioInitStruct);

//    LL_GPIO_ClearFlag_EXTI(GPIO, LL_GPIO_EXTI_LINE_2);
//    LL_GPIO_EnableDigitalFilter(GPIO, LL_GPIO_EXTI_LINE_2);
//    LL_GPIO_SetExtiLine4(GPIO, LL_GPIO_EXTI_LINE_2_PA10);
//    LL_GPIO_SetTriggerEdge0(GPIO, LL_GPIO_EXTI_LINE_2, LL_GPIO_EXTI_TRIGGER_EDGE_FALLING);
//		
//    
//    // �ж����ȼ�����
//    NVIC_DisableIRQ(GPIO_IRQn);
//    NVIC_SetPriority(GPIO_IRQn, 2);
//    NVIC_EnableIRQ(GPIO_IRQn);
//}


/*********************************************
 * ��������I2CStart
 * ��  ��������I2C����
 * ��  �룺��
 * ��  ����TRUE:�����ɹ���FALSE:����ʧ��
 ********************************************/
static uint8_t I2CStart(void)
{
    SDA_H;
		I2Cdelay();
    SCL_H;
    I2Cdelay();
	  gpio_set_SDA_mode(gpio_input);
    if(!SDA_read)
		{
			gpio_set_SDA_mode(gpio_output);
			return FALSE;	//SDA��Ϊ�͵�ƽ������æ,�˳�
		}
	  gpio_set_SDA_mode(gpio_output);
    SDA_L;
    I2Cdelay();
    SCL_L;
    I2Cdelay();
    return TRUE;
}

/*********************************************
 * ��������I2CStop
 * ��  �����ͷ�I2C����
 * ��  �룺��
 * ��  ������
 ********************************************/
static void I2CStop(void)
{
    SCL_L;
    I2Cdelay();
    SDA_L;
    I2Cdelay();
    SCL_H;
    I2Cdelay();
    SDA_H;
    I2Cdelay();
}

/*********************************************
 * ��������I2CAck
 * ��  ��������ASK
 * ��  �룺��
 * ��  ������
 ********************************************/
static void I2CAck(void)
{
    SCL_L;
    I2Cdelay();
    SDA_L;
    I2Cdelay();
    SCL_H;
    I2Cdelay();
    SCL_L;
    I2Cdelay();
}

/*********************************************
 * ��������I2CNoAck
 * ��  ��������NOASK
 * ��  �룺��
 * ��  ������
 ********************************************/
static void I2CNoAck(void)
{
    SCL_L;
    I2Cdelay();
    SDA_H;
    I2Cdelay();
    SCL_H;
    I2Cdelay();
    SCL_L;
    I2Cdelay();
}

/*********************************************
 * ��������I2CWaitAck
 * ��  ������ȡACK�ź�
 * ��  �룺��
 * ��  ����TRUE=��ACK,FALSE=��ACK
 ********************************************/
static uint8_t I2CWaitAck(void)
{
    SCL_L;
    I2Cdelay();
    SDA_H;	
    gpio_set_SDA_mode(gpio_input);	
    I2Cdelay();
    SCL_H;
    I2Cdelay();
    if(SDA_read)
    {
			SCL_L;
			gpio_set_SDA_mode(gpio_output);
			return FALSE;
    }
		gpio_set_SDA_mode(gpio_output);
    SCL_L;
    return TRUE;
}

/*********************************************
 * ��������I2CSendByte
 * ��  ����MCU����һ���ֽ�
 * ��  �룺��
 * ��  ������
 ********************************************/
static void I2CSendByte(uint8_t SendByte) //���ݴӸ�λ����λ
{
    uint8_t i=8;
		while(i--)
		{
			SCL_L;
			I2Cdelay();
			if(SendByte&0x80)
			SDA_H;  
			else 
			SDA_L;   
			SendByte<<=1;
			I2Cdelay();
			SCL_H;
			I2Cdelay();
		}
    SCL_L;
}

/*********************************************
 * ��������I2CReceiveByte
 * ��  ����MCU����һ���ֽ�
 * ��  �룺��
 * ��  ����ReceiveByte
 ********************************************/
static uint8_t I2CReceiveByte(void)
{
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    SDA_H;
    gpio_set_SDA_mode(gpio_input);	
    while(i--)
    {
      ReceiveByte<<=1;         
      SCL_L;
      I2Cdelay();
      SCL_H;
      I2Cdelay();	
      if(SDA_read)
      {
        ReceiveByte|=0x1;
      }
    }
		gpio_set_SDA_mode(gpio_output);
    SCL_L;
    return ReceiveByte;   
}

/*********************************************
 * ��������WriteRTC_Enable
 * ��  ����RTCд�������
 * ��  �룺��
 * ��  ����TRUE:�����ɹ���FALSE:����ʧ��
 ********************************************/
uint8_t WriteRTC_Enable(void)
{
//	portDISABLE_INTERRUPTS();
    if(FALSE == I2CStart()) 
			return FALSE;
    I2CSendByte(RTC_Address); 
    if(I2CWaitAck()== FALSE)   
    {
			I2CStop();
			return FALSE;
		}
    I2CSendByte(CTR2);      
    I2CWaitAck();	
    I2CSendByte(0x80);//��WRTC1=1      
    I2CWaitAck();
    I2CStop(); 
										
    I2CStart();
    I2CSendByte(RTC_Address);      
    I2CWaitAck();   
    I2CSendByte(CTR1);
    I2CWaitAck();	
    I2CSendByte(0x84);//��WRTC2,WRTC3=1      
    I2CWaitAck();
    I2CStop(); 
//		portENABLE_INTERRUPTS();
    return TRUE;
}

/*********************************************
 * ��������WriteRTC_Disable
 * ��  ����RTCд��ֹ����
 * ��  �룺��
 * ��  ����TRUE:�����ɹ���FALSE:����ʧ��
 ********************************************/
uint8_t WriteRTC_Disable(void)
{
//	portDISABLE_INTERRUPTS();
    if(FALSE == I2CStart())
			return FALSE;
    I2CSendByte(RTC_Address); 
    if(I2CWaitAck() == FALSE)
    { 
			I2CStop(); 
		  return FALSE;
		}
    I2CSendByte(CTR1);//����д��ַ0FH      
    I2CWaitAck();	
    I2CSendByte(0x0) ;//��WRTC2,WRTC3=0      
    I2CWaitAck();
    I2CSendByte(0x0) ;//��WRTC1=0(10H��ַ)      
    I2CWaitAck();
    I2CStop(); 
//		portENABLE_INTERRUPTS();
    return TRUE; 
}

/*********************************************
 * ��������RTC_WriteDate
 * ��  ����дRTCʵʱ���ݼĴ���
 * ��  �룺ʱ��ṹ��ָ��
 * ��  ����TRUE:�����ɹ���FALSE:����ʧ��
 ********************************************/
uint8_t RTC_WriteDate(rtc_t	*psRTC)	
{
	//дʱ�����Ҫ��һ�ζ�ʵʱʱ��Ĵ���(00H~06H)����д�룬
	//�����Ե�����7��ʱ�������е�ĳһλ����д����,������ܻ�����ʱ�����ݵĴ����λ. 
	//Ҫ�޸�����ĳһ������ , Ӧһ����д��ȫ�� 7 ��ʵʱʱ������.
	uint8_t time_buf[7];
	time_buf[6]=IntToBCD(psRTC->year);
	time_buf[5]=IntToBCD(psRTC->month);
	time_buf[4]=IntToBCD(psRTC->day);
	time_buf[3]=IntToBCD(psRTC->week );
	time_buf[2]=IntToBCD( psRTC->hour);
	time_buf[1]=IntToBCD(psRTC->min);
	time_buf[0]=IntToBCD(psRTC->sec );
	WriteRTC_Enable();				//ʹ�ܣ�����
	I2CStart();
	I2CSendByte(RTC_Address); 
	if(I2CWaitAck() == FALSE) {
		I2CStop(); 
		return FALSE;
	}
	I2CSendByte(0);			//����д��ʼ��ַ      
	I2CWaitAck();	
	I2CSendByte(time_buf[0]);		 //second     
	I2CWaitAck();	
	I2CSendByte(time_buf[1]);		 //minute      
	I2CWaitAck();	
	I2CSendByte(time_buf[2]|0x80);//hour ,ͬʱ����Сʱ�Ĵ������λ��0��Ϊ12Сʱ�ƣ�1��Ϊ24Сʱ�ƣ�
	I2CWaitAck();	
	I2CSendByte(time_buf[3]);		//week      
	I2CWaitAck();	
	I2CSendByte(time_buf[4]);		//day      
	I2CWaitAck();	
	I2CSendByte(time_buf[5]);		//month      
	I2CWaitAck();	
	I2CSendByte(time_buf[6]);		//year      
	I2CWaitAck();	
	I2CStop();
	WriteRTC_Disable();				  //����
	return	TRUE;
}

/*********************************************
 * ��������RTC_ReadDate
 * ��  ������RTCʵʱ���ݼĴ���
 * ��  �룺ʱ��ṹ��ָ��
 * ��  ����TRUE:�����ɹ���FALSE:����ʧ��
 ********************************************/
uint8_t RTC_ReadDate(rtc_t	*psRTC)
{
	portDISABLE_INTERRUPTS();
	I2CStart();
	I2CSendByte(RTC_Address);      
	if(I2CWaitAck()== FALSE)
	{
		I2CStop(); 
		return FALSE;
	}
	I2CSendByte(0);
	I2CWaitAck();
	I2CStart();	
	I2CSendByte(RTC_Address+1);
	I2CWaitAck();
	sMyTime.sec=I2CReceiveByte();
	I2CAck();
	sMyTime.min=I2CReceiveByte();
	I2CAck();
	sMyTime.hour=I2CReceiveByte() & 0x7F;
	I2CAck();
	sMyTime.week=I2CReceiveByte();
	I2CAck();
	sMyTime.day=I2CReceiveByte();
	I2CAck();
	sMyTime.month=I2CReceiveByte();
	I2CAck();
	sMyTime.year=I2CReceiveByte();		
	I2CNoAck();
	I2CStop(); 
	
	sMyTime.buff[0]=BCDToInt(sMyTime.sec);    //ʮ����
	sMyTime.buff[1]=BCDToInt(sMyTime.min);
	sMyTime.buff[2]=BCDToInt(sMyTime.hour);

	sMyTime.buff[3]=BCDToInt(sMyTime.day);
	sMyTime.buff[4]=BCDToInt(sMyTime.month);
	sMyTime.buff[5]=BCDToInt(sMyTime.year);
	sMyTime.buff[6]=BCDToInt(sMyTime.week);
			
	psRTC->sec=	sMyTime.buff[0];
	psRTC->min=sMyTime.buff[1];
	psRTC->hour=sMyTime.buff[2];
	if(sMyTime.buff[3]!=0)
		psRTC->day=sMyTime.buff[3];
	if(sMyTime.buff[4]!=0)
		psRTC->month=sMyTime.buff[4];
	if(sMyTime.buff[5]!=0)
		psRTC->year=sMyTime.buff[5];	
	psRTC->week=sMyTime.buff[6];	
	portENABLE_INTERRUPTS();
	return	TRUE;
}
/*********************************************
 * ������     ��I2CWriteSerial
 * ��  ��     ��I2C��ָ����ַдһ�ֽ�����
 * Device_Addr��I2C�豸��ַ
 * Address    ���ڲ���ַ
 * length     ���ֽڳ���
 * ps         ��������ָ��
 * ���       ��TRUE �ɹ���FALSE ʧ��
 ********************************************/	
uint8_t I2CWriteSerial(uint8_t DeviceAddress, uint8_t Address, uint8_t length, uint8_t *ps)
{
	portDISABLE_INTERRUPTS();
		if(DeviceAddress == RTC_Address)  WriteRTC_Enable();

		I2CStart();
		I2CSendByte(DeviceAddress);   
		if(I2CWaitAck()== FALSE)
		{
			I2CStop(); 
			return FALSE;
		}
		I2CSendByte(Address);			
		I2CWaitAck();
		for(;(length--)>0;)
		{ 	
			I2CSendByte(*(ps++));		
			I2CAck();			
		}
		I2CStop(); 

		if(DeviceAddress == RTC_Address)  WriteRTC_Disable();
		portENABLE_INTERRUPTS();
		return	TRUE;
}

/*********************************************
 * ������     ��I2CReadSerial
 * ��  ��     ��I2C��ָ����ַ��һ�ֽ�����
 * Device_Addr��I2C�豸��ַ
 * Address    ���ڲ���ַ
 * length     ���ֽڳ���
 * ps         ��������ָ��
 * ���       ��TRUE �ɹ���FALSE ʧ��
 ********************************************/	
uint8_t I2CReadSerial(uint8_t DeviceAddress, uint8_t Address, uint8_t length, uint8_t *ps)
{
	portDISABLE_INTERRUPTS();
		I2CStart();
		I2CSendByte(DeviceAddress);      
		if(I2CWaitAck()== FALSE)
		{I2CStop(); return FALSE;}
		I2CSendByte(Address);
		I2CWaitAck();
		I2CStart();	
		I2CSendByte(DeviceAddress+1);
		I2CWaitAck();
		for(;--length>0;ps++)
		{
			*ps = I2CReceiveByte();
			I2CAck();
		}
		*ps = I2CReceiveByte();	
		I2CNoAck();
		I2CStop(); 
		portENABLE_INTERRUPTS();
		return	TRUE;
}

/*********************************************
 * ��������Set_CountDown
 * ��  �������õ���ʱ�ж�
 * ��  �룺CountDown_Init ����ʱ�жϽṹ��ָ�� 
 * ��  ������
 ********************************************/
void Set_CountDown(CountDown_Def *CountDown_Init)					
{
		uint8_t buf[6];
		uint8_t tem=0xF0;
		buf[0] = (CountDown_Init->IM<<6)|0xB4;							 //10H
		buf[1] = CountDown_Init->d_clk<<4;									 //11H
		buf[2] = 0;																					 //12H
		buf[3] = CountDown_Init->init_val & 0x0000FF;				 //13H
		buf[4] = (CountDown_Init->init_val & 0x00FF00) >> 8; //14H
		buf[5] = (CountDown_Init->init_val & 0xFF0000) >> 16;//15H
		I2CWriteSerial(RTC_Address,CTR2,1,&tem);
		I2CWriteSerial(RTC_Address,CTR2,6,buf);
}

/*********************************************
 * ��������Set_Alarm
 * ��  �������ñ����жϣ����ӹ��ܣ�
 * Enable_config��ʹ������  
 * psRTC������ʱ���ʱ��ṹ��ָ��
 * ��  ������
 ********************************************/
void Set_Alarm(uint8_t Enable_config, rtc_t *psRTC)					
{
		uint8_t buf[10];
		buf[0] = psRTC->sec;
		buf[1] = psRTC->min;
		buf[2] = psRTC->hour;
		buf[3] = 0;
		buf[4] = psRTC->day;
		buf[5] = psRTC->month;
		buf[6] = psRTC->year;
		buf[7] = Enable_config;
		buf[8] = 0xFF;
		buf[9] = 0x92;	
		I2CWriteSerial(RTC_Address,Alarm_SC,10,buf);
}

/*********************************************
 * ��������SetFrq
 * ��  ��������RTCƵ���жϣ���INT�����Ƶ�ʷ���
 * ��  �룺Ƶ��ֵ
 * ��  ������
 ********************************************/
void SetFrq(enum Freq F_Out)					
{
		uint8_t buf[2];
		buf[0] = 0xA1;
		buf[1] = F_Out;
		I2CWriteSerial(RTC_Address,CTR2,2,buf);
}

/*********************************************
 * ��������ClrINT
 * ��  ������ֹ�ж�
 * int_EN���ж����� INTDE��INTDE��INTDE
 * ��  ������
 ********************************************/
void ClrINT(uint8_t int_EN)         
{
		uint8_t buf;
		buf = 0x80 & (~int_EN);
		I2CWriteSerial(RTC_Address,CTR2,1,&buf);
}
/****************************************************************************************/
void sd3078_alarm(void)
{	
	CountDown_Def cdInit;
//	
// #if (INT_TYPE == FREQUENCY)
///**************** Ƶ���ж�������ʾ ****************/
//	SetFrq(F2Hz);//���2HzƵ�ʷ���
//#elif (INT_TYPE == ALARM)
///**************** �����ж�������ʾ ****************/
//	Set_Alarm(sec_ALM|min_ALM|hor_ALM, &Alarm_init);//����ʹ�ܣ�ʱ���֡���
//#elif (INT_TYPE == COUNTDOWN)	
/*************** ����ʱ�ж�������ʾ***************/
	cdInit.IM = 1;				//����Ϊ�������ж�
	cdInit.d_clk = S_1s;		//����ʱ�ж�Դѡ��1s
//	if(device_info.NOSLEEP_MODE == 1)
//		cdInit.init_val = 1;	//������ģʽ�£��ж϶�ʱ���Ϊ1��
//	else
		cdInit.init_val = 10;	//����ʱ��ֵ����Ϊ10
//	device_info.heart_time = 10;
	Set_CountDown(&cdInit);
//#elif (INT_TYPE == DISABLE)	
///****************** ��ֹ�ж���ʾ *****************/
//	ClrINT(INTDE|INTAE|INTFE);
//#endif

}
/****************************************************************************************/
void sd3078_dealarm(void)
{
 	ClrINT(INTDE|INTAE|INTFE);
}
	
/****************************************************************************************/
void sd3078_alarm_Callback(void)
{	
//  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	device_info.alarm_barck_flag++;   //10�����Ӷ�ʱ��
	device_info.wait_min++;	  //�ϱ���ʱʱ����ۼ�
	device_info.vabt_timer++;    //��������ʱ����ۼ�
//	device_info.gps_ec200s_time++;    //4g gps��λʱ��������
//	xTaskNotifyFromISR(sleepHandle, 0x00, eSetBits, &xHigherPriorityTaskWoken);	
//  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
/****************************************************************************************/
uint8_t sd3078_ram_set(uint8_t Address, uint32_t setbuf)
{
	uint8_t i;
	uint8_t ram_setbuf[4];//ת��Ϊ�ֽ�
  for(i=0;i<4;i++)
  {
    ram_setbuf[i]=(setbuf>>((3-i)*8))& 0xFF;       //��  �� 
  }
 	i=I2CWriteSerial(RTC_Address,Address,4,ram_setbuf);
	return i;
}

/****************************************************************************************/
uint32_t sd3078_ram_get(uint8_t Address)
{
	uint8_t ram_getbuf[4];
  uint32_t getbuf;
//ת��Ϊ�ֽ�
 I2CReadSerial(RTC_Address, Address, 4, ram_getbuf);
 getbuf=(uint32_t)(ram_getbuf[0]<<24)|(uint32_t)(ram_getbuf[1]<<16)|(uint32_t)(ram_getbuf[2]<<8)|ram_getbuf[3];
 return  getbuf;
}

/*********************************************END OF FILE**********************/


