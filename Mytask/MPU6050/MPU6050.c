#include "main.h"
#include "MPU6050.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "inv_mpu.h"

/**************************************************************************/
#define PRINT_ACCEL     (0x01)
#define PRINT_GYRO      (0x02)
#define PRINT_QUAT      (0x04)
#define ACCEL_ON        (0x01)
#define GYRO_ON         (0x02)
#define MOTION          (0)
#define NO_MOTION       (1)
#define DEFAULT_MPU_HZ  (200)
#define FLASH_SIZE      (512)
#define FLASH_MEM_START ((void*)0x1800)
#define q30  1073741824.0f

static signed char gyro_orientation[9] = {-1, 0, 0,
                                           0,-1, 0,
                                           0, 0, 1};
#define SDA5_PIN      LL_GPIO_PIN_6
#define SCL5_PIN      LL_GPIO_PIN_7
																					 
#define  SCL5_H         LL_GPIO_SetOutputPin(GPIOB,SCL5_PIN)
#define  SCL5_L         LL_GPIO_ResetOutputPin(GPIOB,SCL5_PIN)

#define  SDA5_H         LL_GPIO_SetOutputPin(GPIOB,SDA5_PIN)
#define  SDA5_L         LL_GPIO_ResetOutputPin(GPIOB,SDA5_PIN)

#define SDA5_read       LL_GPIO_IsInputPinSet(GPIOB, SDA5_PIN)

/**************************************************************/
void IIC5_init(void )
{
	  LL_GPIO_InitTypeDef GPIO_Initure;

    GPIO_Initure.Pin=LL_GPIO_PIN_6;                       //SDA
    GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
    GPIO_Initure.Pull=DISABLE;          //����
		GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;    
    LL_GPIO_Init(GPIOB,&GPIO_Initure);
   
	  GPIO_Initure.Pin=LL_GPIO_PIN_7;                      //SCL
    GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
    GPIO_Initure.Pull=DISABLE;       
		GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;    
    LL_GPIO_Init(GPIOB,&GPIO_Initure);  
	
}
/**************************************************************/
void IIC5_deinit(void)
{
	LL_GPIO_InitTypeDef GPIO_Initure;

//	GPIO_Initure.Pin= SDA6_PIN | SCL6_PIN;
//	GPIO_Initure.Mode=LL_GPIO_MODE_ANALOG;   
//	GPIO_Initure.Pull=DISABLE;          //����
//	GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;    
//	LL_GPIO_Init(GPIOC,&GPIO_Initure);
 
	GPIO_Initure.Pin= SDA5_PIN | SCL5_PIN;
	GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
  GPIO_Initure.Pull=DISABLE;       
	GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;    
	LL_GPIO_Init(GPIOC,&GPIO_Initure);
  LL_GPIO_ResetOutputPin(GPIOC, SDA5_PIN | SCL5_PIN);	
}
/**************************************************************/
static void gpio_set_SDA_mode(uint8_t mode)
{
	LL_GPIO_InitTypeDef GPIO_Initure;
	if(mode == gpio_input)
	{
		GPIO_Initure.Mode=LL_GPIO_MODE_INPUT;
	  GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	}
	else if(mode == gpio_output)
	{
		GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT; 
		GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;		
	}
	
	GPIO_Initure.Pin=SDA5_PIN;                       //SDA  	
	GPIO_Initure.Pull=DISABLE;          //����	    
	LL_GPIO_Init(GPIOB,&GPIO_Initure);
}

/*********************************************
 * ��������I5Cdelay
 * ��  ����I5C��ʱ����
 * ��  �룺��
 * ��  ������
 ********************************************/
static void I5Cdelay()
{	
   uint8_t i=100; //��������Ż��ٶȣ�ͨ����ʱ3~10us��������ʾ����������������
   while(i) 
   { 
     i--; 
   }
}
/**************************************************************/
/*********************************************
 * ��������I5CStart
 * ��  ��������I5C����
 * ��  �룺��
 * ��  ����TRUE:�����ɹ���FALSE:����ʧ��
 ********************************************/
static uint8_t I5CStart(void)
{
    SDA5_H;
		I5Cdelay();
    SCL5_H;
    I5Cdelay();
	  gpio_set_SDA_mode(gpio_input);
    if(!SDA5_read)
		{
			gpio_set_SDA_mode(gpio_output);
			return FALSE;	//SDA��Ϊ�͵�ƽ������æ,�˳�
		}
		gpio_set_SDA_mode(gpio_output);
    SDA5_L;
    I5Cdelay();
    SCL5_L;
    I5Cdelay();
    return TRUE;
}

/*********************************************
 * ��������I5CStop
 * ��  �����ͷ�I5C����
 * ��  �룺��
 * ��  ������
 ********************************************/
static void I5CStop(void)
{
    SCL5_L;
    I5Cdelay();
    SDA5_L;
    I5Cdelay();
    SCL5_H;
    I5Cdelay();
    SDA5_H;
    I5Cdelay();
}

/*********************************************
 * ��������I5CAck
 * ��  ��������ASK
 * ��  �룺��
 * ��  ������
 ********************************************/
static void I5CAck(void)
{
    SCL5_L;
    I5Cdelay();
    SDA5_L;
    I5Cdelay();
    SCL5_H;
    I5Cdelay();
    SCL5_L;
    I5Cdelay();
}

/*********************************************
 * ��������I5CNoAck
 * ��  ��������NOASK
 * ��  �룺��
 * ��  ������
 ********************************************/
static void I5CNoAck(void)
{
    SCL5_L;
    I5Cdelay();
    SDA5_H;
    I5Cdelay();
    SCL5_H;
    I5Cdelay();
    SCL5_L;
    I5Cdelay();
}

/*********************************************
 * ��������I5CWaitAck
 * ��  ������ȡACK�ź�
 * ��  �룺��
 * ��  ����TRUE=��ACK,FALSE=��ACK
 ********************************************/
static uint8_t I5CWaitAck(void)
{
    SCL5_L;
    I5Cdelay();
    SDA5_H;	
    gpio_set_SDA_mode(gpio_input);	
    I5Cdelay();
    SCL5_H;
    I5Cdelay();
    if(!SDA5_read)
    {
			SCL5_L;
			gpio_set_SDA_mode(gpio_output);
			return TRUE;
    }
		gpio_set_SDA_mode(gpio_output);
    SCL5_L;
    return FALSE;
}

/*********************************************
 * ��������I5CSendByte
 * ��  ����MCU����һ���ֽ�
 * ��  �룺��
 * ��  ������
 ********************************************/
static void I5CSendByte(uint8_t SendByte) //���ݴӸ�λ����λ
{
    uint8_t i=8;
		while(i--)
		{
			SCL5_L;
			I5Cdelay();
			if(SendByte&0x80)
			SDA5_H;  
			else 
			SDA5_L;   
			SendByte<<=1;
			I5Cdelay();
			SCL5_H;
			I5Cdelay();
		}
    SCL5_L;
}
/*********************************************
 * ��������I5CReceiveByte
 * ��  ����MCU����һ���ֽ�
 * ��  �룺��
 * ��  ����ReceiveByte
 ********************************************/
static uint8_t I5CReceiveByte(unsigned char ack)
{
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    SDA5_H;		
    gpio_set_SDA_mode(gpio_input);	
    while(i--)
    {
      ReceiveByte<<=1;         
      SCL5_L;
      I5Cdelay();
      SCL5_H;
      I5Cdelay();	
      if(SDA5_read)
      {
        ReceiveByte|=0x1;
      }
    }
		gpio_set_SDA_mode(gpio_output);
    SCL5_L;
		
		if (!ack)
        I5CNoAck();//����nACK
    else
        I5CAck(); //����ACK
    return ReceiveByte;   
}
//IICдһ���ֽ� 
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//    ����,�������
uint8_t MPU_Write_Byte(uint8_t reg,uint8_t data) 				 
{ 
    I5CStart(); 
	I5CSendByte((uint8_t)(MPU6050_DEFAULT_ADDRESS<<1)|0);//����������ַ+д����	
	if(I5CWaitAck() == FALSE)	//�ȴ�Ӧ��
	{
		I5CStop();		 
		return 1;		
	}
    I5CSendByte(reg);	//д�Ĵ�����ַ
    I5CWaitAck();		//�ȴ�Ӧ�� 
	I5CSendByte(data);//��������
	if(I5CWaitAck() == FALSE)	//�ȴ�ACK
	{
		I5CStop();	 
		return 1;		 
	}		 
  I5CStop();	 
	return 0;
}

//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
uint8_t MPU_Read_Byte(uint8_t reg)
{
	uint8_t res_b;
    I5CStart(); 
	I5CSendByte((uint8_t)(MPU6050_DEFAULT_ADDRESS<<1)|0);//����������ַ+д����	
	I5CWaitAck();		//�ȴ�Ӧ�� 
    I5CSendByte(reg);	//д�Ĵ�����ַ
    I5CWaitAck();		//�ȴ�Ӧ��
    I5CStart();
	I5CSendByte((uint8_t)(MPU6050_DEFAULT_ADDRESS<<1)|1);//����������ַ+������	
    I5CWaitAck();		//�ȴ�Ӧ�� 
	res_b=I5CReceiveByte(0);//��ȡ����,����nACK 
    I5CStop();			//����һ��ֹͣ���� 
	return res_b;		
}

//IIC����д
//addr:������ַ 
//reg:�Ĵ�����ַ
//len:д�볤��
//buf:������
//����ֵ:0,����
//    ����,�������
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	uint8_t i; 
    I5CStart(); 
	I5CSendByte((addr<<1)|0);//����������ַ+д����	
	if(I5CWaitAck() == FALSE)	//�ȴ�Ӧ��
	{
		I5CStop();		 
		return 1;		
	}
    I5CSendByte(reg);	//д�Ĵ�����ַ
    I5CWaitAck();		//�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		I5CSendByte(buf[i]);	//��������
		if(I5CWaitAck() == FALSE)		//�ȴ�ACK
		{
			I5CStop();	 
			return 1;		 
		}		
	}    
    I5CStop();	 
	return 0;	
} 

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t IICreadByte(uint8_t dev, uint8_t reg, uint8_t *data)
*��������:	    ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
����	reg	   �Ĵ�����ַ
		*data  ���������ݽ�Ҫ��ŵĵ�ַ
����   1
*******************************************************************************/ 
static uint8_t IIC5readByte(uint8_t reg, uint8_t *data)
{
	*data=MPU_Read_Byte(reg);
    return 1;
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t IICwriteBits(uint8_t reg,uint8_t bitStart,uint8_t length,uint8_t data)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �еĶ��λ
����	reg	   �Ĵ�����ַ
		bitStart  Ŀ���ֽڵ���ʼλ
		length   λ����
		data    ��Ÿı�Ŀ���ֽ�λ��ֵ
����   �ɹ� Ϊ1 
 		ʧ��Ϊ0
*******************************************************************************/ 
static uint8_t IIC5writeBits(uint8_t reg,uint8_t bitStart,uint8_t length,uint8_t data)
{
    uint8_t b;
    if(IIC5readByte(reg, &b) != 0) 
		{
        uint8_t mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
        data <<= (8 - length);
        data >>= (7 - bitStart);
        b &= mask;
        b |= data;
        return MPU_Write_Byte(reg, b);
    } 
		else 
        return 0;    
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t IIC5writeBit(uint8_t reg, uint8_t bitNum, uint8_t data)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �е�1��λ
����	reg	   �Ĵ�����ַ
		bitNum  Ҫ�޸�Ŀ���ֽڵ�bitNumλ
		data  Ϊ0 ʱ��Ŀ��λ������0 ���򽫱���λ
����   �ɹ� Ϊ0 
 		ʧ��Ϊ1
*******************************************************************************/ 
static uint8_t IIC5writeBit(uint8_t reg, uint8_t bitNum, uint8_t data){
    uint8_t b;
    IIC5readByte(reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return MPU_Write_Byte(reg, b);
}



//IIC������
//addr:������ַ
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�ĳ���
//buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//    ����,�������
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
 	I5CStart(); 
	I5CSendByte((addr<<1)|0);//����������ַ+д����	
	if(I5CWaitAck() == FALSE)	//�ȴ�Ӧ��
	{
		I5CStop();		 
		return 1;		
	}
    I5CSendByte(reg);	//д�Ĵ�����ַ
    I5CWaitAck();		//�ȴ�Ӧ��
    I5CStart();
	I5CSendByte((addr<<1)|1);//����������ַ+������	
    I5CWaitAck();		//�ȴ�Ӧ�� 
	while(len)
	{
		if(len==1)
			*buf=I5CReceiveByte(0);//������,����nACK 
		else 
			*buf=I5CReceiveByte(1);		//������,����ACK  
		len--;
		buf++; 
	}    
    I5CStop();	//����һ��ֹͣ���� 
	return 0;	
}
/*********************************************************************************/
static  unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}


static  unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx)
{
    unsigned short scalar;
    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}


static void run_self_test(void)
{
    int result1;
    long gyro[3], accel[3];

    result1 = mpu_run_self_test(gyro, accel);
    if (result1 == 0x03) {
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
		Trace_log("setting bias succesfully ......\r\n");
    }
}



uint8_t buffer[14];

int16_t  MPU6050_FIFO[6][11];
int16_t Gx_offset=0,Gy_offset=0,Gz_offset=0;


/**************************ʵ�ֺ���********************************************
*����ԭ��:		void  MPU6050_newValues(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz)
*��������:	    ���µ�ADC���ݸ��µ� FIFO���飬�����˲�����
*******************************************************************************/
void  MPU6050_newValues(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz)
{
	unsigned char i ;
	int32_t sum=0;
	for(i=1;i<10;i++)
	{	//FIFO ����
		MPU6050_FIFO[0][i-1]=MPU6050_FIFO[0][i];
		MPU6050_FIFO[1][i-1]=MPU6050_FIFO[1][i];
		MPU6050_FIFO[2][i-1]=MPU6050_FIFO[2][i];
		MPU6050_FIFO[3][i-1]=MPU6050_FIFO[3][i];
		MPU6050_FIFO[4][i-1]=MPU6050_FIFO[4][i];
		MPU6050_FIFO[5][i-1]=MPU6050_FIFO[5][i];
	}
	MPU6050_FIFO[0][9]=ax;//���µ����ݷ��õ� ���ݵ������
	MPU6050_FIFO[1][9]=ay;
	MPU6050_FIFO[2][9]=az;
	MPU6050_FIFO[3][9]=gx;
	MPU6050_FIFO[4][9]=gy;
	MPU6050_FIFO[5][9]=gz;

	sum=0;
	for(i=0;i<10;i++)
	{	//��ǰ����ĺϣ���ȡƽ��ֵ
		 sum+=MPU6050_FIFO[0][i];
	}
	MPU6050_FIFO[0][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++)
	{
		 sum+=MPU6050_FIFO[1][i];
	}
	MPU6050_FIFO[1][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++)
	{
		 sum+=MPU6050_FIFO[2][i];
	}
	MPU6050_FIFO[2][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++)
	{
		 sum+=MPU6050_FIFO[3][i];
	}
	MPU6050_FIFO[3][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++)
	{
		 sum+=MPU6050_FIFO[4][i];
	}
	MPU6050_FIFO[4][10]=sum/10;

	sum=0;
	for(i=0;i<10;i++)
	{
		 sum+=MPU6050_FIFO[5][i];
	}
	MPU6050_FIFO[5][10]=sum/10;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setClockSource(uint8_t source)
*��������:	    ����  MPU6050 ��ʱ��Դ
 * CLK_SEL | Clock Source
 * --------+--------------------------------------
 * 0       | Internal oscillator
 * 1       | PLL with X Gyro reference
 * 2       | PLL with Y Gyro reference
 * 3       | PLL with Z Gyro reference
 * 4       | PLL with external 32.768kHz reference
 * 5       | PLL with external 19.2MHz reference
 * 6       | Reserved
 * 7       | Stops the clock and keeps the timing generator in reset
*******************************************************************************/
void MPU6050_setClockSource(uint8_t source)
{
    IIC5writeBits(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}

/** Set full-scale gyroscope range.
 * @param range New full-scale gyroscope range value
 * @see getFullScaleRange()
 * @see MPU6050_GYRO_FS_250
 * @see MPU6050_RA_GYRO_CONFIG
 * @see MPU6050_GCONFIG_FS_SEL_BIT
 * @see MPU6050_GCONFIG_FS_SEL_LENGTH
 */
void MPU6050_setFullScaleGyroRange(uint8_t range) {
    IIC5writeBits(MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setFullScaleAccelRange(uint8_t range)
*��������:	    ����  MPU6050 ���ٶȼƵ��������
*******************************************************************************/
void MPU6050_setFullScaleAccelRange(uint8_t range) {
    IIC5writeBits(MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setSleepEnabled(uint8_t enabled)
*��������:	    ����  MPU6050 �Ƿ����˯��ģʽ
				enabled =1   ˯��
			    enabled =0   ����
*******************************************************************************/
void MPU6050_setSleepEnabled(uint8_t enabled) 
{
    IIC5writeBit(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

void MPU6050_setLowPower(void)
{
	IIC5writeBit(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CYCLE_BIT , 1);
	IIC5writeBit(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, 0);
	IIC5writeBit(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_TEMP_DIS_BIT , 1);
//	MPU_Write_Byte(MPU6050_RA_PWR_MGMT_1,0x28);
	IIC5writeBit(MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_STBY_XG_BIT , 1);
	IIC5writeBit(MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_STBY_YG_BIT , 1);
	IIC5writeBit(MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_STBY_ZG_BIT , 1);
	IIC5writeBit(MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_LP_WAKE_CTRL_BIT , 0);
	IIC5writeBit(MPU6050_RA_PWR_MGMT_2, MPU6050_PWR2_LP_WAKE_CTRL_LENGTH , 0);
//	MPU_Write_Byte(MPU6050_RA_PWR_MGMT_2,0x07);
	
}
void MPU6050_ResetLowPower(void)
{
	MPU_Write_Byte(MPU6050_RA_PWR_MGMT_1,0x00);
	MPU_Write_Byte(MPU6050_RA_PWR_MGMT_2,0x00);
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t MPU6050_getDeviceID(void)
*��������:	    ��ȡ  MPU6050 WHO_AM_I ��ʶ	 ������ 0x68
*******************************************************************************/
uint8_t MPU6050_getDeviceID(void) {

    MPU_Read_Len(devAddr, MPU6050_RA_WHO_AM_I, 1, buffer);
    return buffer[0];
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t MPU6050_testConnection(void)
*��������:	    ���MPU6050 �Ƿ��Ѿ�����
*******************************************************************************/
uint8_t MPU6050_testConnection(void) {
   if(MPU6050_getDeviceID() == 0x68)  //0b01101000;
   return 1;
   	else return 0;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
*��������:	    ���� MPU6050 �Ƿ�ΪAUX I2C�ߵ�����
*******************************************************************************/
void MPU6050_setI2CMasterModeEnabled(uint8_t enabled) {
    IIC5writeBit(MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setI2CBypassEnabled(uint8_t enabled)
*��������:	    ���� MPU6050 �Ƿ�ΪAUX I2C�ߵ�����
*******************************************************************************/
void MPU6050_setI2CBypassEnabled(uint8_t enabled) {
    IIC5writeBit(MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_initialize(void)
*��������:	    ��ʼ�� 	MPU6050 �Խ������״̬��
*******************************************************************************/
void MPU6050_initialize(void) 
{
    MPU6050_setClockSource(MPU6050_CLOCK_PLL_XGYRO); //����ʱ��
    MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);//������������� +-1000��ÿ��
    MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_2);	//���ٶȶ�������� +-2G
    MPU6050_setSleepEnabled(0); //���빤��״̬
	 MPU6050_setI2CMasterModeEnabled(0);	 //����MPU6050 ����AUXI2C
	 MPU6050_setI2CBypassEnabled(0);	 //����������I2C��	MPU6050��AUXI2C	ֱͨ������������ֱ�ӷ���HMC5883L
}

void Free_Fall_Interrupt(void)          //���������ж�
{
    MPU_Write_Byte(MPU6050_RA_FF_THR,0x01);             //����������ֵ 
    MPU_Write_Byte(MPU6050_RA_FF_DUR,0x01);             //����������ʱ��20ms ��λ1ms �Ĵ���0X20
}
void Motion_Interrupt(void)             //�˶��ж�
{
    MPU_Write_Byte(MPU6050_RA_MOT_THR,0x01);            //�˶���ֵ 
//	  MPU_Write_Byte(MPU6050_RA_MOT_DETECT_CTRL,0x00);
    MPU_Write_Byte(MPU6050_RA_MOT_DUR,0x01);            //���ʱ��20ms ��λ1ms �Ĵ���0X20
}
void Zero_Motion_Interrupt(void)        //��ֹ�ж�
{
    MPU_Write_Byte(MPU6050_RA_ZRMOT_THR,0x20);          //��ֹ��ֵ 
    MPU_Write_Byte(MPU6050_RA_ZRMOT_DUR,0x20);          //��ֹ���ʱ��32ms ��λ1ms �Ĵ���0X20
}
/******************************************************************/
void MPU_INT_Init(void)
{
//���ݾ����ж�
//    MPU_Write_Byte(MPU6050_RA_CONFIG,0x06);         //�����ⲿ���Ų�����DLPF���ֵ�ͨ�˲���
//    MPU_Write_Byte(MPU6050_RA_INT_PIN_CFG,0X1C);    //INT���ŵ͵�ƽƽʱ
//    MPU_Write_Byte(MPU6050_RA_INT_ENABLE,0x01);     //�ж�ʹ�ܼĴ���
 
  
//�������塢�˶�����ֹ�ж� ��ѡһ  
//    Free_Fall_Interrupt();                          //���������ж�
//    Motion_Interrupt();                               //�˶��ж�
    Zero_Motion_Interrupt();                        //��ֹ�ж�
    MPU_Write_Byte(MPU6050_RA_CONFIG,0x04);           //�����ⲿ���Ų�����DLPF���ֵ�ͨ�˲���
    MPU_Write_Byte(MPU6050_RA_ACCEL_CONFIG,0x1C);     //���ٶȴ��������̺͸�ͨ�˲�������
    MPU_Write_Byte(MPU6050_RA_INT_PIN_CFG,0X10);      //INT���ŵ͵�ƽƽʱ
    MPU_Write_Byte(MPU6050_RA_INT_ENABLE,0x50);       //�ж�ʹ�ܼĴ���
}
/******************************************************************/
void MPU_INTR_WORK(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	  device_info.sensor_sam_timer = 2;   //����+�ϱ�
		 Trace_log("�񶯻���\r\n");
		xTaskNotifyFromISR(sensorHandle, 0x00, eSetBits, &xHigherPriorityTaskWoken);
}
//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:1,���óɹ�
//    ����,����ʧ�� 
uint8_t MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU6050_RA_CONFIG,data);//�������ֵ�ͨ�˲���  
}
//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:1,���óɹ�
//    ����,����ʧ�� 
uint8_t MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU6050_RA_SMPLRT_DIV,data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}
/******************************************************************/
char MPU6050_Init(void)
{
	uint8_t res;
	IIC5_init(); 		
		                                      //��ʼ��IIC����
		MPU_Write_Byte(MPU6050_RA_PWR_MGMT_1,0X80);           //��λMPU6050
		delay_vtask(100);
		MPU_Write_Byte(MPU6050_RA_PWR_MGMT_1,0X00);           //����MPU6050
//		MPU_Set_Gyro_Fsr(3);                              //�����Ǵ�����,��2000dps
	  MPU_Write_Byte(MPU6050_RA_GYRO_CONFIG,3<<3);
//		MPU_Set_Accel_Fsr(0);                             //���ٶȴ����� ��2g
	  MPU_Write_Byte(MPU6050_RA_ACCEL_CONFIG,0<<3);
		MPU_Set_Rate(200);                                //���ò�����200-50HZ
		MPU_Write_Byte(MPU6050_RA_INT_ENABLE,0X00);              //�ر������ж�
		MPU_Write_Byte(MPU6050_RA_USER_CTRL,0X00);           //I2C��ģʽ�ر�
		MPU_Write_Byte(MPU6050_RA_FIFO_EN,0X00);             //�ر�FIFO
//		MPU_Write_Byte(MPU6050_RA_INT_PIN_CFG,0X80);         //�жϵ��߼���ƽģʽ,����Ϊ0���ж��ź�Ϊ�ߵ磻����Ϊ1���ж��ź�Ϊ�͵�ƽʱ��
		res=MPU_Read_Byte(MPU6050_RA_WHO_AM_I);
//		if(res==MPU6050_DEFAULT_ADDRESS)                                 //����ID��ȷ
//		{
					MPU_Write_Byte(MPU6050_RA_PWR_MGMT_1,0X02);     //����CLKSEL,PLL X ��Ϊ�ο�
					MPU_Write_Byte(MPU6050_RA_PWR_MGMT_2,0X00);     //���ٶ������Ƕ�����
					MPU_Set_Rate(200);                          //���ò�����Ϊ200-50HZ
			MPU_INT_Init();
			MPU6050_setLowPower();
//		}else return 1;
		return 0;
}
/**************************************************************************
�������ܣ�MPU6050����DMP�ĳ�ʼ��
��ڲ�������
����  ֵ����
��    �ߣ�ƽ��С��֮��
**************************************************************************/
uint8_t DMP_Init(void)
{ 
//   uint8_t temp[1]={0};
//   i2cRead(0x68,0x75,1,temp);
//	 Trace_log("mpu_set_sensor complete ......\r\n");
//	if(temp[0]!=0x68)NVIC_SystemReset();
	IIC5_init();
	if(!mpu_init())
  {
	  if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))
	  	 Trace_log("mpu_set_sensor complete ......\r\n");
	  if(!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))
	  	 Trace_log("mpu_configure_fifo complete ......\r\n");
	  if(!mpu_set_sample_rate(DEFAULT_MPU_HZ))
	  	 Trace_log("mpu_set_sample_rate complete ......\r\n");
	  if(!dmp_load_motion_driver_firmware())
	  	Trace_log("dmp_load_motion_driver_firmware complete ......\r\n");
	  if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)))
	  	 Trace_log("dmp_set_orientation complete ......\r\n");
	  if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
	        DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
	        DMP_FEATURE_GYRO_CAL))
	  	 Trace_log("dmp_enable_feature complete ......\r\n");
	  if(!dmp_set_fifo_rate(DEFAULT_MPU_HZ))
	  	 Trace_log("dmp_set_fifo_rate complete ......\r\n");
	  run_self_test();
	  if(!mpu_set_dmp_state(1))
	  	 Trace_log("mpu_set_dmp_state complete ......\r\n");
		return TRUE;
  }
	else
	{
		Trace_log("MPU6050��ʼ��ʧ��\r\n");
		return FALSE;
	}
//	MPU_INT_Init();
//	set_int_enable(1);
//	MPU6050_setLowPower();
}
/**************************************************************************
�������ܣ���ȡMPU6050����DMP����̬��Ϣ
��ڲ�������
����  ֵ����
��    �ߣ�ƽ��С��֮��
**************************************************************************/
uint8_t Read_DMP()
{	
		short gyro[3], accel[3], sensors;
		float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	  unsigned long sensor_timestamp;
		unsigned char more;
		long quat[4];
		if(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more)) 
			return 1;		
		if (sensors & INV_WXYZ_QUAT)
		{    
			 q0=quat[0] / q30;
			 q1=quat[1] / q30;
			 q2=quat[2] / q30;
			 q3=quat[3] / q30;
			 device_info.MPU_GX = (float)asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; 	   //Pitch   ������
			 device_info.MPU_GY = (float)atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll    ������
			 device_info.MPU_GZ = (float)atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;    //Yaw   �����
			 
//			MPU6050_setLowPower();
			 return 0;
		}	
		else 
			return 2;
				
}
/**************************************************************************
�������ܣ���ȡMPU6050�����¶ȴ���������
��ڲ�������
����  ֵ�������¶�
��    �ߣ�ƽ��С��֮��
**************************************************************************/
int Read_Temperature(void)
{	   
//	  float Temp;
	  device_info.MPU_TE=(MPU_Read_Byte(MPU6050_RA_TEMP_OUT_H)<<8)+MPU_Read_Byte(MPU6050_RA_TEMP_OUT_L);
		if(device_info.MPU_TE>32768) device_info.MPU_TE-=65536;
		device_info.MPU_TE=(36.53+device_info.MPU_TE/340);
//	device_info.MPU_TE = device_info.MPU_TE;
	  return 0;
}

//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
uint8_t MPU_Get_Gyroscope(void)
{
    uint8_t buf[6],res;  
	res=MPU_Read_Len(MPU6050_DEFAULT_ADDRESS,MPU6050_RA_GYRO_XOUT_H,6,buf);
	if(res==0)
	{
		device_info.MPU_GX =((uint16_t)buf[0]<<8)|buf[1];  
		device_info.MPU_GY =((uint16_t)buf[2]<<8)|buf[3];  
		device_info.MPU_GZ =((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;
}
//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
uint8_t MPU_Get_Accelerometer(void)
{
    uint8_t buf[6],res;
  short	ax,ay,az;
	float ax1,ay1,az1;
	
	res=MPU_Read_Len(MPU6050_DEFAULT_ADDRESS,MPU6050_RA_ACCEL_XOUT_H,6,buf);
	if(res==0)
	{
		ax=((uint16_t)buf[0]<<8)|buf[1];
		ay=((uint16_t)buf[2]<<8)|buf[3];
		az=((uint16_t)buf[4]<<8)|buf[5];
		ax1=ax/16384.0;
		ay1=ay/16384.0;
		az1=az/16384.0;
		//���Ƕ���̬�Ƿ����仯
		if((ax1-device_info.MPU_AX)>0.1||(device_info.MPU_AX-ax1)>0.1)
		{
			device_info.MPU_MOT_DATA = 1;
		}
		else if((ay1-device_info.MPU_AY)>0.1||(device_info.MPU_AY-ay1)>0.1)
		{
			device_info.MPU_MOT_DATA = 1;
		}
		else if((az1-device_info.MPU_AZ)>0.1||(device_info.MPU_AZ-az1)>0.1)
		{
			device_info.MPU_MOT_DATA = 1;
		}
		else
			device_info.MPU_MOT_DATA = 0;
		
		//������μ�⣬�ж��豸���ھ�ֹ״̬�����˶�״̬
		device_info.MPU_AX=ax1;
		device_info.MPU_AY=ay1;
		device_info.MPU_AZ=az1;		
	} 	
    return res;
}
/************************************************************************/
//���������

uint8_t Get_shock_mpu6050(void)
{
    uint8_t buf[6],res,cishu;
  short	ax,ay,az;
	float ax1,ay1,az1;
	float ax2,ay2,az2;

	cishu = 5;
	Trace_log("�𶯼��\r\n");
	delay_vtask(100);
	res=MPU_Read_Len(MPU6050_DEFAULT_ADDRESS,MPU6050_RA_ACCEL_XOUT_H,6,buf);
	if(res==0)
	{
		ax=((uint16_t)buf[0]<<8)|buf[1];
		ay=((uint16_t)buf[2]<<8)|buf[3];
		az=((uint16_t)buf[4]<<8)|buf[5];
		ax1=ax/16384.0;
		ay1=ay/16384.0;
		az1=az/16384.0;
	}
	while(cishu--)
	{
		delay_vtask(100);
		res=MPU_Read_Len(MPU6050_DEFAULT_ADDRESS,MPU6050_RA_ACCEL_XOUT_H,6,buf);
		if(res==0)
		{
			ax=((uint16_t)buf[0]<<8)|buf[1];
			ay=((uint16_t)buf[2]<<8)|buf[3];
			az=((uint16_t)buf[4]<<8)|buf[5];
			ax2=ax/16384.0;
			ay2=ay/16384.0;
			az2=az/16384.0;
			//���Ƕ���̬�Ƿ����仯
			if(((ax1-ax2)*1000)>device_info.save.shock_data||((ax2-ax1)*1000)>device_info.save.shock_data)
			{
				device_info.MPU_SHOCK_DATA = 1;
			}
			else if(((ay1-ay2)*1000)>device_info.save.shock_data||((ay2-ay1)*1000)>device_info.save.shock_data)
			{
				device_info.MPU_SHOCK_DATA = 1;
			}
			else if(((az1-az2)*1000)>device_info.save.shock_data||((az2-az1)*1000)>device_info.save.shock_data)
			{
				device_info.MPU_SHOCK_DATA = 1;
			}
			else
				device_info.MPU_SHOCK_DATA = 0;
			
			//������μ�⣬�ж��豸���ھ�ֹ״̬�����˶�״̬
		}
		if(device_info.MPU_SHOCK_DATA == 1)
			break;
	}
	return res;
}
//------------------End of File----------------------------
