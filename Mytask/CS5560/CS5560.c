#include "CS5560.h"


#define CS5560_ADD   0xd0

#define CS5560_IIC_PORT  GPIOB
#define CS5560_SDA_PIN   LL_GPIO_PIN_2    //
#define CS5560_SCL_PIN   LL_GPIO_PIN_1    //

#define  CS5560_SCL_H         LL_GPIO_SetOutputPin(CS5560_IIC_PORT,CS5560_SCL_PIN)
#define  CS5560_SCL_L         LL_GPIO_ResetOutputPin(CS5560_IIC_PORT,CS5560_SCL_PIN)

#define  CS5560_SDA_H         LL_GPIO_SetOutputPin(CS5560_IIC_PORT,CS5560_SDA_PIN)
#define  CS5560_SDA_L         LL_GPIO_ResetOutputPin(CS5560_IIC_PORT,CS5560_SDA_PIN)

#define CS5560_SDA_read       LL_GPIO_IsInputPinSet(CS5560_IIC_PORT,CS5560_SDA_PIN)


/*********************************************
 * 函数名：CS5560_I2Cdelay
 * 描  述：I2C延时函数
 * 输  入：无
 * 输  出：无
 ********************************************/
static void CS5560_I2Cdelay(void)
{	
   uint8_t i=100; //这里可以优化速度，通常延时3~10us，可以用示波器看波形来调试
   while(i) 
   { 
     i--; 
   }
}
/**********************************************************/
void CS5560_IIC_init(void )
{
	  LL_GPIO_InitTypeDef GPIO_Initure;

    GPIO_Initure.Pin=CS5560_SCL_PIN|CS5560_SDA_PIN;
    GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
    GPIO_Initure.Pull=DISABLE;          
		GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;   
    LL_GPIO_Init(CS5560_IIC_PORT,&GPIO_Initure);   
	CS5560_SCL_H;
	CS5560_SDA_H;
}
/**********************************************************/
void CS5560_IIC_deinit(void )    //有外部上拉电阻
{
	  LL_GPIO_InitTypeDef GPIO_Initure;

    GPIO_Initure.Pin=CS5560_SCL_PIN|CS5560_SDA_PIN;
    GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT;   
    GPIO_Initure.Pull=ENABLE;          
		GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;   
    LL_GPIO_Init(CS5560_IIC_PORT,&GPIO_Initure);
   CS5560_SCL_H;
	CS5560_SDA_H;
}
/**********************************************************/
static void gpio_set_SDA_mode(uint8_t mode)
{
	LL_GPIO_InitTypeDef GPIO_Initure;
	if(mode == gpio_input)
		GPIO_Initure.Mode=LL_GPIO_MODE_INPUT;
	else if(mode == gpio_output)
	{
		GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT; 		
	}
	
	GPIO_Initure.Pin = CS5560_SDA_PIN;                       //SDA  
	GPIO_Initure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_Initure.Pull=DISABLE;          //上拉	    
	LL_GPIO_Init(CS5560_IIC_PORT,&GPIO_Initure);
}
/**********************************************************/
/*********************************************
 * 函数名：I2CStart
 * 描  述：开启I2C总线
 * 输  入：无
 * 输  出：TRUE:操作成功，FALSE:操作失败
 ********************************************/
static uint8_t IICStart(void)
{
    CS5560_SDA_H;
		CS5560_I2Cdelay();
    CS5560_SCL_H;
    CS5560_I2Cdelay();
	  gpio_set_SDA_mode(gpio_input);
    if(!CS5560_SDA_read)
		{
			Trace_log("AD启动无应答\r\n");
			gpio_set_SDA_mode(gpio_output);
			return FALSE;	//SDA线为低电平则总线忙,退出
		}
	  gpio_set_SDA_mode(gpio_output);
    CS5560_SDA_L;
    CS5560_I2Cdelay();
    CS5560_SCL_L;
    CS5560_I2Cdelay();
    return TRUE;
}
/*********************************************
 * 函数名：I2CStop
 * 描  述：释放I2C总线
 * 输  入：无
 * 输  出：无
 ********************************************/
static void IICStop(void)
{
    CS5560_SCL_L;
    CS5560_I2Cdelay();
    CS5560_SDA_L;
    CS5560_I2Cdelay();
    CS5560_SCL_H;
    CS5560_I2Cdelay();
    CS5560_SDA_H;
    CS5560_I2Cdelay();
}
/*********************************************
 * 函数名：I2CAck
 * 描  述：发送ASK
 * 输  入：无
 * 输  出：无
 ********************************************/
static void IICAck(void)
{
    CS5560_SCL_L;
    CS5560_I2Cdelay();
    CS5560_SDA_L;
    CS5560_I2Cdelay();
    CS5560_SCL_H;
    CS5560_I2Cdelay();
    CS5560_SCL_L;
    CS5560_I2Cdelay();
}
/*********************************************
 * 函数名：I2CNoAck
 * 描  述：发送NOASK
 * 输  入：无
 * 输  出：无
 ********************************************/
static void IICNoAck(void)
{
    CS5560_SCL_L;
    CS5560_I2Cdelay();
    CS5560_SDA_H;
    CS5560_I2Cdelay();
    CS5560_SCL_H;
    CS5560_I2Cdelay();
    CS5560_SCL_L;
    CS5560_I2Cdelay();
}
/*********************************************
 * 函数名：I2CWaitAck
 * 描  述：读取ACK信号
 * 输  入：无
 * 输  出：TRUE=有ACK,FALSE=无ACK
 ********************************************/
static uint8_t IICWaitAck(void)
{
    CS5560_SCL_L;
    CS5560_I2Cdelay();
    CS5560_SDA_H;	
    gpio_set_SDA_mode(gpio_input);	
    CS5560_I2Cdelay();
    CS5560_SCL_H;
    CS5560_I2Cdelay();
    if(CS5560_SDA_read)
    {
			CS5560_SCL_L;
			gpio_set_SDA_mode(gpio_output);
			return FALSE;
    }
		gpio_set_SDA_mode(gpio_output);
    CS5560_SCL_L;
    return TRUE;
}
/*********************************************
 * 函数名：I2CSendByte
 * 描  述：MCU发送一个字节
* 输  入：SendByte   发送数据内容
 * 输  出：无
 ********************************************/
static void IICSendByte(uint8_t SendByte) //数据从高位到低位
{
    uint8_t i=8;
		while(i--)
		{
			CS5560_SCL_L;
			CS5560_I2Cdelay();
			if(SendByte&0x80)
			CS5560_SDA_H;  
			else 
			CS5560_SDA_L;   
			SendByte<<=1;
			CS5560_I2Cdelay();
			CS5560_SCL_H;
			CS5560_I2Cdelay();
		}
    CS5560_SCL_L;
}
/*********************************************
 * 函数名：I2CReceiveByte
 * 描  述：MCU读入一个字节
 * 输  入：无
 * 输  出：ReceiveByte
 ********************************************/
static uint8_t IICReceiveByte(uint8_t ack)
{
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    CS5560_SDA_H;
    gpio_set_SDA_mode(gpio_input);	
    while(i--)
    {
      ReceiveByte<<=1;         
      CS5560_SCL_L;
      CS5560_I2Cdelay();
      CS5560_SCL_H;
      CS5560_I2Cdelay();	
      if(CS5560_SDA_read)
      {
        ReceiveByte|=0x1;
      }
    }
		gpio_set_SDA_mode(gpio_output);
    CS5560_SCL_L;
		
		if (!ack)
        IICNoAck();//发送nACK
    else
        IICAck(); //发送ACK
		
    return ReceiveByte;   
}
/*********************************************
 * 函数名：I2CSendByte
 * 描  述：MCU发送一个字节
 * 输  入：daddr  写入寄存器地址
					 wdata   要写入的数据
 * 输  出：无
 ********************************************/
static uint8_t IIC_Write(uint8_t daddr,uint8_t wdata) //数据从高位到低位
{
	IICStart();
	IICSendByte(CS5560_ADD);
	if(IICWaitAck() == FALSE)
	{
		Trace_log("AD无应答\r\n");
		return FALSE;
	}
	IICSendByte(daddr);
	IICWaitAck();
	IICSendByte(wdata);
	IICWaitAck();
	IICStop();
	return TRUE;
}
/**********************************************************/
static uint32_t I2C_Read_byte(unsigned char RAddr) 
{
	uint8_t data;
	IICStart(); 
	IICSendByte(CS5560_ADD); 
	if(IICWaitAck() == FALSE)
		return FALSE; 
	IICSendByte(RAddr); 
	IICWaitAck();
	
	IICStart(); 
	IICSendByte(CS5560_ADD+1); 
	IICWaitAck(); 
	data = IICReceiveByte(0); 
	IICStop();
	
	return data;
}
/**********************************************************/
static uint32_t I2C_Read_data(unsigned char RAddr) 
{
	uint8_t data_h,data_m,data_l;
	uint32_t data = 0;
	IICStart(); 
	IICSendByte(CS5560_ADD); 
	if(IICWaitAck() == FALSE)
		return FALSE; 
	IICSendByte(RAddr); 
	IICWaitAck();
	
	IICStart(); 
	IICSendByte(CS5560_ADD+1); 
	IICWaitAck(); 
	data_l = IICReceiveByte(1);
	data_m = IICReceiveByte(1);
	data_h = IICReceiveByte(0); 
	IICStop();
	
	data = data_h;
	data<<=8;
	data |= data_m;
	data<<=8;
	data |= data_l;
	
	return data;
}
/**********************************************************/
static uint8_t wait_adc_ok(void)
{
	uint8_t da;
	uint16_t time=0;
	Trace_log("等待数据OK\r\n");
	while(1)
	{
		time++;
		da = I2C_Read_byte(0x08);
		if((da&0x02) == 0x02)
		{
			return TRUE;
		}
		if(time>2000)
			return FALSE;
	}
}
/**********************************************************/
void CS5560_init(void)
{
	uint8_t ADC_SPEED = 1;
	
	IIC_Write(0x0a,0xE7);          //固定数据
	IIC_Write(0x11,0x01); // HIRCC     使能RCC
	IIC_Write(0x00,0x28); // PWRC   电源控制寄存器
	IIC_Write(0x01,0x2e); // PGAC0   增益控制     差分基准1/2   
	IIC_Write(0x02,0x00); // PGAC1 
	IIC_Write(0x03,0x30); // PGACS     通道选择    adc0~vcm
	if(ADC_SPEED==1)
		{IIC_Write(0x07,0x05);} //ADCR0 20HZ
	else 
		{IIC_Write(0x07,0x03);} //ADCR0 10HZ 
	IIC_Write(0x09,0x1F); // ADCS 
	IIC_Write(0x08,0x00);
}
/**********************************************************/
void Read_AD_Value(void) 
{
	uint32_t adc0,adc1,adc2;
	char ADC_VALUE[40];	
	CS5560_IIC_init();
	CS5560_init();
	if(wait_adc_ok()==FALSE)
		Trace_log("数据未准备好");
	adc0 = I2C_Read_data(0x04);  
	IIC_Write(0x08,0x00);    //清除寄存器数据 

//	IIC_Write(0x03,0x03);      //通道选择    adc1~vcm
//	wait_adc_ok();
//	adc1 = I2C_Read_data(0x04);  
//	IIC_Write(0x08,0x00);    //清除寄存器数据 
//	
//	IIC_Write(0x03,0x31);      //通道选择    adc2~vcm
//	wait_adc_ok();
//	adc2 = I2C_Read_data(0x04);  
//	IIC_Write(0x08,0x00);    //清除寄存器数据 
	
	device_info.channel_data[1] = adc0+1.25;
	device_info.channel_data[2] = adc1+1.25;
	device_info.channel_data[3] = adc2+1.25;
	
	sprintf((char *)ADC_VALUE,"%.4f   %.4f  %.4f  ",device_info.channel_data[1],device_info.channel_data[2],device_info.channel_data[3]);
	Trace_log("PT100温度:");
	Trace_log(ADC_VALUE);
	Trace_log("℃\r\n");
}
/**********************************************************/

/**********************************************************/
