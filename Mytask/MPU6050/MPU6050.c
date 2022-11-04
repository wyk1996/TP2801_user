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
    GPIO_Initure.Pull=DISABLE;          //上拉
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
//	GPIO_Initure.Pull=DISABLE;          //上拉
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
	GPIO_Initure.Pull=DISABLE;          //上拉	    
	LL_GPIO_Init(GPIOB,&GPIO_Initure);
}

/*********************************************
 * 函数名：I5Cdelay
 * 描  述：I5C延时函数
 * 输  入：无
 * 输  出：无
 ********************************************/
static void I5Cdelay()
{	
   uint8_t i=100; //这里可以优化速度，通常延时3~10us，可以用示波器看波形来调试
   while(i) 
   { 
     i--; 
   }
}
/**************************************************************/
/*********************************************
 * 函数名：I5CStart
 * 描  述：开启I5C总线
 * 输  入：无
 * 输  出：TRUE:操作成功，FALSE:操作失败
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
			return FALSE;	//SDA线为低电平则总线忙,退出
		}
		gpio_set_SDA_mode(gpio_output);
    SDA5_L;
    I5Cdelay();
    SCL5_L;
    I5Cdelay();
    return TRUE;
}

/*********************************************
 * 函数名：I5CStop
 * 描  述：释放I5C总线
 * 输  入：无
 * 输  出：无
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
 * 函数名：I5CAck
 * 描  述：发送ASK
 * 输  入：无
 * 输  出：无
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
 * 函数名：I5CNoAck
 * 描  述：发送NOASK
 * 输  入：无
 * 输  出：无
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
 * 函数名：I5CWaitAck
 * 描  述：读取ACK信号
 * 输  入：无
 * 输  出：TRUE=有ACK,FALSE=无ACK
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
 * 函数名：I5CSendByte
 * 描  述：MCU发送一个字节
 * 输  入：无
 * 输  出：无
 ********************************************/
static void I5CSendByte(uint8_t SendByte) //数据从高位到低位
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
 * 函数名：I5CReceiveByte
 * 描  述：MCU读入一个字节
 * 输  入：无
 * 输  出：ReceiveByte
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
        I5CNoAck();//发送nACK
    else
        I5CAck(); //发送ACK
    return ReceiveByte;   
}
//IIC写一个字节 
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
uint8_t MPU_Write_Byte(uint8_t reg,uint8_t data) 				 
{ 
    I5CStart(); 
	I5CSendByte((uint8_t)(MPU6050_DEFAULT_ADDRESS<<1)|0);//发送器件地址+写命令	
	if(I5CWaitAck() == FALSE)	//等待应答
	{
		I5CStop();		 
		return 1;		
	}
    I5CSendByte(reg);	//写寄存器地址
    I5CWaitAck();		//等待应答 
	I5CSendByte(data);//发送数据
	if(I5CWaitAck() == FALSE)	//等待ACK
	{
		I5CStop();	 
		return 1;		 
	}		 
  I5CStop();	 
	return 0;
}

//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
uint8_t MPU_Read_Byte(uint8_t reg)
{
	uint8_t res_b;
    I5CStart(); 
	I5CSendByte((uint8_t)(MPU6050_DEFAULT_ADDRESS<<1)|0);//发送器件地址+写命令	
	I5CWaitAck();		//等待应答 
    I5CSendByte(reg);	//写寄存器地址
    I5CWaitAck();		//等待应答
    I5CStart();
	I5CSendByte((uint8_t)(MPU6050_DEFAULT_ADDRESS<<1)|1);//发送器件地址+读命令	
    I5CWaitAck();		//等待应答 
	res_b=I5CReceiveByte(0);//读取数据,发送nACK 
    I5CStop();			//产生一个停止条件 
	return res_b;		
}

//IIC连续写
//addr:器件地址 
//reg:寄存器地址
//len:写入长度
//buf:数据区
//返回值:0,正常
//    其他,错误代码
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	uint8_t i; 
    I5CStart(); 
	I5CSendByte((addr<<1)|0);//发送器件地址+写命令	
	if(I5CWaitAck() == FALSE)	//等待应答
	{
		I5CStop();		 
		return 1;		
	}
    I5CSendByte(reg);	//写寄存器地址
    I5CWaitAck();		//等待应答
	for(i=0;i<len;i++)
	{
		I5CSendByte(buf[i]);	//发送数据
		if(I5CWaitAck() == FALSE)		//等待ACK
		{
			I5CStop();	 
			return 1;		 
		}		
	}    
    I5CStop();	 
	return 0;	
} 

/**************************实现函数********************************************
*函数原型:		uint8_t IICreadByte(uint8_t dev, uint8_t reg, uint8_t *data)
*功　　能:	    读取指定设备 指定寄存器的一个值
输入	reg	   寄存器地址
		*data  读出的数据将要存放的地址
返回   1
*******************************************************************************/ 
static uint8_t IIC5readByte(uint8_t reg, uint8_t *data)
{
	*data=MPU_Read_Byte(reg);
    return 1;
}
/**************************实现函数********************************************
*函数原型:		uint8_t IICwriteBits(uint8_t reg,uint8_t bitStart,uint8_t length,uint8_t data)
*功　　能:	    读 修改 写 指定设备 指定寄存器一个字节 中的多个位
输入	reg	   寄存器地址
		bitStart  目标字节的起始位
		length   位长度
		data    存放改变目标字节位的值
返回   成功 为1 
 		失败为0
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

/**************************实现函数********************************************
*函数原型:		uint8_t IIC5writeBit(uint8_t reg, uint8_t bitNum, uint8_t data)
*功　　能:	    读 修改 写 指定设备 指定寄存器一个字节 中的1个位
输入	reg	   寄存器地址
		bitNum  要修改目标字节的bitNum位
		data  为0 时，目标位将被清0 否则将被置位
返回   成功 为0 
 		失败为1
*******************************************************************************/ 
static uint8_t IIC5writeBit(uint8_t reg, uint8_t bitNum, uint8_t data){
    uint8_t b;
    IIC5readByte(reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return MPU_Write_Byte(reg, b);
}



//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
 	I5CStart(); 
	I5CSendByte((addr<<1)|0);//发送器件地址+写命令	
	if(I5CWaitAck() == FALSE)	//等待应答
	{
		I5CStop();		 
		return 1;		
	}
    I5CSendByte(reg);	//写寄存器地址
    I5CWaitAck();		//等待应答
    I5CStart();
	I5CSendByte((addr<<1)|1);//发送器件地址+读命令	
    I5CWaitAck();		//等待应答 
	while(len)
	{
		if(len==1)
			*buf=I5CReceiveByte(0);//读数据,发送nACK 
		else 
			*buf=I5CReceiveByte(1);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
    I5CStop();	//产生一个停止条件 
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


/**************************实现函数********************************************
*函数原型:		void  MPU6050_newValues(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz)
*功　　能:	    将新的ADC数据更新到 FIFO数组，进行滤波处理
*******************************************************************************/
void  MPU6050_newValues(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz)
{
	unsigned char i ;
	int32_t sum=0;
	for(i=1;i<10;i++)
	{	//FIFO 操作
		MPU6050_FIFO[0][i-1]=MPU6050_FIFO[0][i];
		MPU6050_FIFO[1][i-1]=MPU6050_FIFO[1][i];
		MPU6050_FIFO[2][i-1]=MPU6050_FIFO[2][i];
		MPU6050_FIFO[3][i-1]=MPU6050_FIFO[3][i];
		MPU6050_FIFO[4][i-1]=MPU6050_FIFO[4][i];
		MPU6050_FIFO[5][i-1]=MPU6050_FIFO[5][i];
	}
	MPU6050_FIFO[0][9]=ax;//将新的数据放置到 数据的最后面
	MPU6050_FIFO[1][9]=ay;
	MPU6050_FIFO[2][9]=az;
	MPU6050_FIFO[3][9]=gx;
	MPU6050_FIFO[4][9]=gy;
	MPU6050_FIFO[5][9]=gz;

	sum=0;
	for(i=0;i<10;i++)
	{	//求当前数组的合，再取平均值
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

/**************************实现函数********************************************
*函数原型:		void MPU6050_setClockSource(uint8_t source)
*功　　能:	    设置  MPU6050 的时钟源
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

/**************************实现函数********************************************
*函数原型:		void MPU6050_setFullScaleAccelRange(uint8_t range)
*功　　能:	    设置  MPU6050 加速度计的最大量程
*******************************************************************************/
void MPU6050_setFullScaleAccelRange(uint8_t range) {
    IIC5writeBits(MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setSleepEnabled(uint8_t enabled)
*功　　能:	    设置  MPU6050 是否进入睡眠模式
				enabled =1   睡觉
			    enabled =0   工作
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
/**************************实现函数********************************************
*函数原型:		uint8_t MPU6050_getDeviceID(void)
*功　　能:	    读取  MPU6050 WHO_AM_I 标识	 将返回 0x68
*******************************************************************************/
uint8_t MPU6050_getDeviceID(void) {

    MPU_Read_Len(devAddr, MPU6050_RA_WHO_AM_I, 1, buffer);
    return buffer[0];
}

/**************************实现函数********************************************
*函数原型:		uint8_t MPU6050_testConnection(void)
*功　　能:	    检测MPU6050 是否已经连接
*******************************************************************************/
uint8_t MPU6050_testConnection(void) {
   if(MPU6050_getDeviceID() == 0x68)  //0b01101000;
   return 1;
   	else return 0;
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
*功　　能:	    设置 MPU6050 是否为AUX I2C线的主机
*******************************************************************************/
void MPU6050_setI2CMasterModeEnabled(uint8_t enabled) {
    IIC5writeBit(MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setI2CBypassEnabled(uint8_t enabled)
*功　　能:	    设置 MPU6050 是否为AUX I2C线的主机
*******************************************************************************/
void MPU6050_setI2CBypassEnabled(uint8_t enabled) {
    IIC5writeBit(MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_initialize(void)
*功　　能:	    初始化 	MPU6050 以进入可用状态。
*******************************************************************************/
void MPU6050_initialize(void) 
{
    MPU6050_setClockSource(MPU6050_CLOCK_PLL_XGYRO); //设置时钟
    MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);//陀螺仪最大量程 +-1000度每秒
    MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_2);	//加速度度最大量程 +-2G
    MPU6050_setSleepEnabled(0); //进入工作状态
	 MPU6050_setI2CMasterModeEnabled(0);	 //不让MPU6050 控制AUXI2C
	 MPU6050_setI2CBypassEnabled(0);	 //主控制器的I2C与	MPU6050的AUXI2C	直通。控制器可以直接访问HMC5883L
}

void Free_Fall_Interrupt(void)          //自由落体中断
{
    MPU_Write_Byte(MPU6050_RA_FF_THR,0x01);             //自由落体阈值 
    MPU_Write_Byte(MPU6050_RA_FF_DUR,0x01);             //自由落体检测时间20ms 单位1ms 寄存器0X20
}
void Motion_Interrupt(void)             //运动中断
{
    MPU_Write_Byte(MPU6050_RA_MOT_THR,0x01);            //运动阈值 
//	  MPU_Write_Byte(MPU6050_RA_MOT_DETECT_CTRL,0x00);
    MPU_Write_Byte(MPU6050_RA_MOT_DUR,0x01);            //检测时间20ms 单位1ms 寄存器0X20
}
void Zero_Motion_Interrupt(void)        //静止中断
{
    MPU_Write_Byte(MPU6050_RA_ZRMOT_THR,0x20);          //静止阈值 
    MPU_Write_Byte(MPU6050_RA_ZRMOT_DUR,0x20);          //静止检测时间32ms 单位1ms 寄存器0X20
}
/******************************************************************/
void MPU_INT_Init(void)
{
//数据就绪中断
//    MPU_Write_Byte(MPU6050_RA_CONFIG,0x06);         //配置外部引脚采样和DLPF数字低通滤波器
//    MPU_Write_Byte(MPU6050_RA_INT_PIN_CFG,0X1C);    //INT引脚低电平平时
//    MPU_Write_Byte(MPU6050_RA_INT_ENABLE,0x01);     //中断使能寄存器
 
  
//自由落体、运动、静止中断 三选一  
//    Free_Fall_Interrupt();                          //自由落体中断
//    Motion_Interrupt();                               //运动中断
    Zero_Motion_Interrupt();                        //静止中断
    MPU_Write_Byte(MPU6050_RA_CONFIG,0x04);           //配置外部引脚采样和DLPF数字低通滤波器
    MPU_Write_Byte(MPU6050_RA_ACCEL_CONFIG,0x1C);     //加速度传感器量程和高通滤波器配置
    MPU_Write_Byte(MPU6050_RA_INT_PIN_CFG,0X10);      //INT引脚低电平平时
    MPU_Write_Byte(MPU6050_RA_INT_ENABLE,0x50);       //中断使能寄存器
}
/******************************************************************/
void MPU_INTR_WORK(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	  device_info.sensor_sam_timer = 2;   //采样+上报
		 Trace_log("振动唤醒\r\n");
		xTaskNotifyFromISR(sensorHandle, 0x00, eSetBits, &xHigherPriorityTaskWoken);
}
//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:1,设置成功
//    其他,设置失败 
uint8_t MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU6050_RA_CONFIG,data);//设置数字低通滤波器  
}
//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:1,设置成功
//    其他,设置失败 
uint8_t MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU6050_RA_SMPLRT_DIV,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}
/******************************************************************/
char MPU6050_Init(void)
{
	uint8_t res;
	IIC5_init(); 		
		                                      //初始化IIC总线
		MPU_Write_Byte(MPU6050_RA_PWR_MGMT_1,0X80);           //复位MPU6050
		delay_vtask(100);
		MPU_Write_Byte(MPU6050_RA_PWR_MGMT_1,0X00);           //唤醒MPU6050
//		MPU_Set_Gyro_Fsr(3);                              //陀螺仪传感器,±2000dps
	  MPU_Write_Byte(MPU6050_RA_GYRO_CONFIG,3<<3);
//		MPU_Set_Accel_Fsr(0);                             //加速度传感器 ±2g
	  MPU_Write_Byte(MPU6050_RA_ACCEL_CONFIG,0<<3);
		MPU_Set_Rate(200);                                //设置采样率200-50HZ
		MPU_Write_Byte(MPU6050_RA_INT_ENABLE,0X00);              //关闭所有中断
		MPU_Write_Byte(MPU6050_RA_USER_CTRL,0X00);           //I2C主模式关闭
		MPU_Write_Byte(MPU6050_RA_FIFO_EN,0X00);             //关闭FIFO
//		MPU_Write_Byte(MPU6050_RA_INT_PIN_CFG,0X80);         //中断的逻辑电平模式,设置为0，中断信号为高电；设置为1，中断信号为低电平时。
		res=MPU_Read_Byte(MPU6050_RA_WHO_AM_I);
//		if(res==MPU6050_DEFAULT_ADDRESS)                                 //器件ID正确
//		{
					MPU_Write_Byte(MPU6050_RA_PWR_MGMT_1,0X02);     //设置CLKSEL,PLL X 轴为参考
					MPU_Write_Byte(MPU6050_RA_PWR_MGMT_2,0X00);     //加速度陀螺仪都工作
					MPU_Set_Rate(200);                          //设置采样率为200-50HZ
			MPU_INT_Init();
			MPU6050_setLowPower();
//		}else return 1;
		return 0;
}
/**************************************************************************
函数功能：MPU6050内置DMP的初始化
入口参数：无
返回  值：无
作    者：平衡小车之家
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
		Trace_log("MPU6050初始化失败\r\n");
		return FALSE;
	}
//	MPU_INT_Init();
//	set_int_enable(1);
//	MPU6050_setLowPower();
}
/**************************************************************************
函数功能：读取MPU6050内置DMP的姿态信息
入口参数：无
返回  值：无
作    者：平衡小车之家
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
			 device_info.MPU_GX = (float)asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; 	   //Pitch   俯仰角
			 device_info.MPU_GY = (float)atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll    翻滚角
			 device_info.MPU_GZ = (float)atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;    //Yaw   航向角
			 
//			MPU6050_setLowPower();
			 return 0;
		}	
		else 
			return 2;
				
}
/**************************************************************************
函数功能：读取MPU6050内置温度传感器数据
入口参数：无
返回  值：摄氏温度
作    者：平衡小车之家
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

//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
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
//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
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
		//检测角度姿态是否发生变化
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
		
		//连续多次检测，判断设备是在静止状态还是运动状态
		device_info.MPU_AX=ax1;
		device_info.MPU_AY=ay1;
		device_info.MPU_AZ=az1;		
	} 	
    return res;
}
/************************************************************************/
//检测震动数据

uint8_t Get_shock_mpu6050(void)
{
    uint8_t buf[6],res,cishu;
  short	ax,ay,az;
	float ax1,ay1,az1;
	float ax2,ay2,az2;

	cishu = 5;
	Trace_log("震动检测\r\n");
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
			//检测角度姿态是否发生变化
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
			
			//连续多次检测，判断设备是在静止状态还是运动状态
		}
		if(device_info.MPU_SHOCK_DATA == 1)
			break;
	}
	return res;
}
//------------------End of File----------------------------
