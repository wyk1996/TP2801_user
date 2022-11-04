#include "main.h"
#include "QMA7981.h"



#define  SCL_H        LL_GPIO_SetOutputPin(shock_port,shock_scl_pin)
#define  SCL_L        LL_GPIO_ResetOutputPin(shock_port,shock_scl_pin)  

#define  SDA_H         LL_GPIO_SetOutputPin(shock_port,shock_sda_pin)
#define  SDA_L         LL_GPIO_ResetOutputPin(shock_port,shock_sda_pin)

#define	SDA_read      LL_GPIO_IsInputPinSet(shock_port,shock_sda_pin)

void iic7_init(void)
{
	LL_GPIO_InitTypeDef GPIO_Initure;

	GPIO_Initure.Pin=shock_sda_pin|shock_scl_pin;                       //SDA
	GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
	GPIO_Initure.Pull=ENABLE;          //����
	GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;    
	LL_GPIO_Init(shock_port,&GPIO_Initure);

 
//	GPIO_Initure.Pin=shock_scl_pin;                      //SCL
//	GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
//	GPIO_Initure.Pull=ENABLE;       
//	GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;    
//	LL_GPIO_Init(shock_port,&GPIO_Initure); 
}
/*********************************************************************************************/
static void gpio_set_shock_SDA_mode(uint8_t mode)
{
	LL_GPIO_InitTypeDef GPIO_Initure;
	if(mode == gpio_input)
		GPIO_Initure.Mode=LL_GPIO_MODE_INPUT;
	else if(mode == gpio_output)
	{
		GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT; 		
	}
	
	GPIO_Initure.Pin=shock_sda_pin;                       //SDA  
	GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_Initure.Pull=ENABLE;          //����	    
	LL_GPIO_Init(shock_port,&GPIO_Initure);
}
/*********************************************************************************************/
/*********************************************
 * ��������ICdelay
 * ��  ����IC��ʱ����
 * ��  �룺��
 * ��  ������
 ********************************************/
static void ICdelay()
{	
   uint8_t i=200; //��������Ż��ٶȣ�ͨ����ʱ3~10us��������ʾ����������������
   while(i) 
   { 
     i--; 
   }
}
/**************************************************************/
/*********************************************
 * ��������ICStart
 * ��  ��������IC����
 * ��  �룺��
 * ��  ����TRUE:�����ɹ���FALSE:����ʧ��
 ********************************************/
static uint8_t ICStart(void)
{
    SDA_H;
		ICdelay();
    SCL_H;
    ICdelay();
	  gpio_set_shock_SDA_mode(gpio_input);
    if(!SDA_read) 
		{
			gpio_set_shock_SDA_mode(gpio_output);
			return FALSE;	//SDA��Ϊ�͵�ƽ������æ,�˳�
		}
		gpio_set_shock_SDA_mode(gpio_output);
    SDA_L;
    ICdelay();
    SCL_L;
    ICdelay();
    return TRUE;
}

/*********************************************
 * ��������ICStop
 * ��  �����ͷ�IC����
 * ��  �룺��
 * ��  ������
 ********************************************/
static void ICStop(void)
{
    SCL_L;
    ICdelay();
    SDA_L;
    ICdelay();
    SCL_H;
    ICdelay();
    SDA_H;
    ICdelay();
}

/*********************************************
 * ��������ICAck
 * ��  ��������ASK
 * ��  �룺��
 * ��  ������
 ********************************************/
static void ICAck(void)
{
    SCL_L;
    ICdelay();
    SDA_L;
    ICdelay();
    SCL_H;
    ICdelay();
    SCL_L;
    ICdelay();
}

/*********************************************
 * ��������ICNoAck
 * ��  ��������NOASK
 * ��  �룺��
 * ��  ������
 ********************************************/
static void ICNoAck(void)
{
    SCL_L;
    ICdelay();
    SDA_H;
    ICdelay();
    SCL_H;
    ICdelay();
    SCL_L;
    ICdelay();
}

/*********************************************
 * ��������ICWaitAck
 * ��  ������ȡACK�ź�
 * ��  �룺��
 * ��  ����TRUE=��ACK,FALSE=��ACK
 ********************************************/
static uint8_t ICWaitAck(void)
{	
    SCL_L;
    ICdelay();
    SDA_H;
    gpio_set_shock_SDA_mode(gpio_input);	
    ICdelay();
    SCL_H;
    ICdelay();	  
    if(!SDA_read)    //�͵�ƽӦ��
    {
			SCL_L;
			gpio_set_shock_SDA_mode(gpio_output);
			return TRUE;
    }
		gpio_set_shock_SDA_mode(gpio_output);
    SCL_L;
    return FALSE;
}

/*********************************************
 * ��������ICSendByte
 * ��  ����MCU����һ���ֽ�
 * ��  �룺��
 * ��  ������
 ********************************************/
static void ICSendByte(uint8_t SendByte) //���ݴӸ�λ����λ
{
    uint8_t i=8;
		while(i--)
		{
			SCL_L;
			ICdelay();
			if(SendByte&0x80)
			SDA_H;  
			else 
			SDA_L;   
			SendByte<<=1;
			ICdelay();
			SCL_H;
			ICdelay();
		}
    SCL_L;
}
/*********************************************
 * ��������ICReceiveByte
 * ��  ����MCU����һ���ֽ�
 * ��  �룺��
 * ��  ����ReceiveByte
 ********************************************/
static uint8_t ICReceiveByte(unsigned char ack)
{
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    SDA_H;	
		gpio_set_shock_SDA_mode(gpio_input);	
    while(i--)
    {
      ReceiveByte<<=1;         
      SCL_L;
      ICdelay();
      SCL_H;
      ICdelay();	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
		gpio_set_shock_SDA_mode(gpio_output);
    SCL_L;
		
		if (!ack)
        ICNoAck();//����nACK
    else
        ICAck(); //����ACK
    return ReceiveByte;   
}
//IICдһ���ֽ� 
//addr:������ַ 
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//    ����,�������
static uint8_t IIC_Write_Byte(uint8_t addr,uint8_t reg,uint8_t data) 				 
{ 
    ICStart(); 
	ICSendByte((uint8_t)(addr<<1)|0);//����������ַ+д����	
	if(ICWaitAck() == FALSE)	//�ȴ�Ӧ��
	{
		ICStop();		 
		return 1;		
	}
    ICSendByte(reg);	//д�Ĵ�����ַ
    ICWaitAck();		//�ȴ�Ӧ�� 
	ICSendByte(data);//��������
	if(ICWaitAck() == FALSE)	//�ȴ�ACK
	{
		ICStop();	 
		return 1;		 
	}		 
  ICStop();	 
	return 0;
}

//IIC��һ���ֽ� 
//addr:������ַ 
//reg:�Ĵ�����ַ 
//����ֵ:����������
static uint8_t IIC_Read_Byte(uint8_t addr,uint8_t reg)
{
	uint8_t res_b;
    ICStart(); 
	ICSendByte((uint8_t)(addr<<1)|0);//����������ַ+д����	
	ICWaitAck();		//�ȴ�Ӧ�� 
    ICSendByte(reg);	//д�Ĵ�����ַ
    ICWaitAck();		//�ȴ�Ӧ��
    ICStart();
	ICSendByte((uint8_t)(addr<<1)|1);//����������ַ+������	
    ICWaitAck();		//�ȴ�Ӧ�� 
	res_b=ICReceiveByte(0);//��ȡ����,����nACK 
    ICStop();			//����һ��ֹͣ���� 
	return res_b;		
}

//IIC����д
//addr:������ַ 
//reg:�Ĵ�����ַ
//len:д�볤��
//buf:������
//����ֵ:0,����
//    ����,�������
static uint8_t IIC_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	uint8_t i; 
    ICStart(); 
	ICSendByte((addr<<1)|0);//����������ַ+д����	
	if(ICWaitAck() == FALSE)	//�ȴ�Ӧ��
	{
		ICStop();		 
		return 1;		
	}
    ICSendByte(reg);	//д�Ĵ�����ַ
    ICWaitAck();		//�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		ICSendByte(buf[i]);	//��������
		if(ICWaitAck() == FALSE)		//�ȴ�ACK
		{
			ICStop();	 
			return 1;		 
		}		
	}    
    ICStop();	 
	return 0;	
} 

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t IICreadByte(uint8_t dev, uint8_t reg, uint8_t *data)
*��������:	    ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
����  addr:������ַ 
     reg	   �Ĵ�����ַ
		*data  ���������ݽ�Ҫ��ŵĵ�ַ
����   1
*******************************************************************************/ 
static uint8_t IICreadByte(uint8_t addr,uint8_t reg, uint8_t *data)
{
	*data=IIC_Read_Byte(addr,reg);
    return 1;
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t IICwriteBits(uint8_t reg,uint8_t bitStart,uint8_t length,uint8_t data)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �еĶ��λ
���� addr:������ַ	
    reg	   �Ĵ�����ַ
		bitStart  Ŀ���ֽڵ���ʼλ
		length   λ����
		data    ��Ÿı�Ŀ���ֽ�λ��ֵ
����   �ɹ� Ϊ1 
 		ʧ��Ϊ0
*******************************************************************************/ 
static uint8_t IICwriteBits(uint8_t addr,uint8_t reg,uint8_t bitStart,uint8_t length,uint8_t data)
{
    uint8_t b;
    if(IICreadByte(addr,reg, &b) != 0) 
		{
        uint8_t mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
        data <<= (8 - length);
        data >>= (7 - bitStart);
        b &= mask;
        b |= data;
        return IIC_Write_Byte(addr,reg, b);
    } 
		else 
        return 0;    
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t IIC5writeBit(uint8_t reg, uint8_t bitNum, uint8_t data)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �е�1��λ
����	addr:������ַ
    reg	   �Ĵ�����ַ
		bitNum  Ҫ�޸�Ŀ���ֽڵ�bitNumλ
		data  Ϊ0 ʱ��Ŀ��λ������0 ���򽫱���λ
����   �ɹ� Ϊ0 
 		ʧ��Ϊ1
*******************************************************************************/ 
static uint8_t IICwriteBit(uint8_t addr,uint8_t reg, uint8_t bitNum, uint8_t data){
    uint8_t b;
    IICreadByte(addr,reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return IIC_Write_Byte(addr,reg, b);
}

//IIC������
//addr:������ַ
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�ĳ���
//buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//    ����,�������
static uint8_t IIC_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
 	ICStart(); 
	ICSendByte((addr<<1)|0);//����������ַ+д����	
	if(ICWaitAck() == FALSE)	//�ȴ�Ӧ��
	{
		ICStop();		 
		return 1;		
	}
    ICSendByte(reg);	//д�Ĵ�����ַ
    ICWaitAck();		//�ȴ�Ӧ��
    ICStart();
	ICSendByte((addr<<1)|1);//����������ַ+������	
    ICWaitAck();		//�ȴ�Ӧ�� 
	while(len)
	{
		if(len==1)
			*buf=ICReceiveByte(0);//������,����nACK 
		else 
			*buf=ICReceiveByte(1);		//������,����ACK  
		len--;
		buf++; 
	}    
    ICStop();	//����һ��ֹͣ���� 
	return 0;	
}
/*******************************************************************************/
/*******************************************************************************/
void QMA7981_init(void)
{
	uint8_t shock_da;
	uint32_t dd;
	iic7_init();
	dd = (uint32_t)device_info.save.shock_data*1000/244/16;
	shock_da = (uint8_t)dd;
	IIC_Write_Byte(QMA7981_ADDRESS,QMA7981_RESET,QMA7981_RESET_DAT);     //��λ������
	IIC_Write_Byte(QMA7981_ADDRESS,QMA7981_RESET,QMA7981_RESET_DEF);    //�����λ��־
	IIC_Write_Byte(QMA7981_ADDRESS,QMA7981_PM,QMA7981_PM_DEF|QMA7981_PM_ACTIVE|QMA7981_PM_MCLK_25);    //���ù���ģʽ  //
	IIC_Write_Byte(QMA7981_ADDRESS,QMA7981_INT_STO,QMA7981_INT_STO_DEF|QMA7981_ANY_MOT_FIRST_Z|QMA7981_ANY_MOT_FIRST_Y|QMA7981_ANY_MOT_FIRST_X);     //�����жϴ���Դ  
	IIC_Write_Byte(QMA7981_ADDRESS,QMA7981_INT_EN2,QMA7981_INT_EN2_DEF|QMA7981_ANY_MOT_EN_X|QMA7981_ANY_MOT_EN_Y|QMA7981_ANY_MOT_EN_Z);          //ʹ���˶��ж�  
	IIC_Write_Byte(QMA7981_ADDRESS,QMA7981_INT_MAP1,QMA7981_INT_MAP1_DEF|QMA7981_INT1_ANY_MOT);       //�����ж��������   
	IIC_Write_Byte(QMA7981_ADDRESS,QMA7981_MOT_CONF0,0X00);
	IIC_Write_Byte(QMA7981_ADDRESS,QMA7981_MOF_CONF2,shock_da);             //�����˶��ж���ֵ	
}

void QMA7981_read_AC(void)
{
	uint8_t data_Register[6]={0};
	float SHOCK_AX,SHOCK_AY,SHOCK_AZ;
	short   data_Register1[3]={0};
  IIC_Read_Len(QMA7981_ADDRESS,QMA7981_DATA_ADD,6,data_Register);
	
	data_Register1[0]  = (short)((data_Register[1]<<8) |( data_Register[0]));
	data_Register1[1]  = (short)((data_Register[3]<<8) |( data_Register[2]));
	data_Register1[2]  = (short)((data_Register[5]<<8) |( data_Register[4]));			
	data_Register1[0] = data_Register1[0]>>2;     //X��ԭʼ����
	data_Register1[1] = data_Register1[1]>>2;     //Y��ԭʼ����
	data_Register1[2] = data_Register1[2]>>2;     //Z��ԭʼ����				  
  
	SHOCK_AX = (float)data_Register1[0]*244/1000000.0;
	SHOCK_AY = (float)data_Register1[1]*244/1000000.0;
	SHOCK_AZ = (float)data_Register1[2]*244/1000000.0;
	
	char mb_addr[50];
	sprintf(mb_addr, "X����ٶ�[g]:%.3f\r\n", SHOCK_AX);
	Trace_log(mb_addr);
	sprintf(mb_addr, "Y����ٶ�[g]:%.3f\r\n", SHOCK_AY);
	Trace_log(mb_addr);
	sprintf(mb_addr, "Z����ٶ�[g]:%.3f\r\n", SHOCK_AZ);
	Trace_log(mb_addr);
}

////�����ж�
//void GPIO_IRQHandler(void)
//{  
//    if(SET == LL_EXTI_IsActiveFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_5))
//    {
//      LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_5);
//			device_info.sensor_stime = 0;  //��ֹ����������
//			device_info.MPU_SHOCK_DATA = 1;    //�豸����
//			xTaskNotify(sensoe_handler, NONE, eSetBits);   //���������ݶ�ȡ
//    } 
//}
//    

//// �ⲿ�����жϳ�ʼ��
//void shock_interrupt_init(void)
//{
//    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};    
//    
//    //���ű��ش���ѡ��
//    LL_RCC_Enable_SleepmodeExternalInterrupt();//����ʹ���ⲿ�жϲ���
//    LL_RCC_Group1_EnableOperationClock(LL_RCC_OPERATION1_CLOCK_EXTI);//EXTI����ʱ��ʹ��    
//    
//    //�õ���GPIO����    ��Ϊ���� 
//    GPIO_InitStruct.Pin        = shock_int1_pin;
//		GPIO_InitStruct.Mode       = LL_GPIO_MODE_INPUT;
//		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
//		LL_GPIO_Init(shock_port, &GPIO_InitStruct);

//    LL_EXTI_SetTrigEdge(GPIO_COMMON, LL_GPIO_EXTI_LINE_5, LL_GPIO_EXTI_TRIGGER_NONE);//�رձ��ش���
//    //ÿ��LINE ֻ��ѡһ������ 
//    LL_GPIO_SetExitLine0(GPIO_COMMON, LL_GPIO_EXTI_LINE_5_PB4);//�ж�����ѡ��
//    LL_EXTI_EnableDigitalFilter(GPIO_COMMON, LL_GPIO_EXTI_LINE_5);//EXTI�����˲�����
//    LL_EXTI_SetTrigEdge(GPIO_COMMON, LL_GPIO_EXTI_LINE_5, LL_GPIO_EXTI_TRIGGER_RISING);//����ѡ��  ������  
//    LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_5);//����жϱ�־
//    
//    /*NVIC�ж�����*/
//    NVIC_DisableIRQ(GPIO_IRQn);
//    NVIC_SetPriority(GPIO_IRQn,2);//�ж����ȼ�����
//    NVIC_EnableIRQ(GPIO_IRQn);
//}
