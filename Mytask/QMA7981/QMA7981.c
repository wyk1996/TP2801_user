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
	GPIO_Initure.Pull=ENABLE;          //上拉
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
	GPIO_Initure.Pull=ENABLE;          //上拉	    
	LL_GPIO_Init(shock_port,&GPIO_Initure);
}
/*********************************************************************************************/
/*********************************************
 * 函数名：ICdelay
 * 描  述：IC延时函数
 * 输  入：无
 * 输  出：无
 ********************************************/
static void ICdelay()
{	
   uint8_t i=200; //这里可以优化速度，通常延时3~10us，可以用示波器看波形来调试
   while(i) 
   { 
     i--; 
   }
}
/**************************************************************/
/*********************************************
 * 函数名：ICStart
 * 描  述：开启IC总线
 * 输  入：无
 * 输  出：TRUE:操作成功，FALSE:操作失败
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
			return FALSE;	//SDA线为低电平则总线忙,退出
		}
		gpio_set_shock_SDA_mode(gpio_output);
    SDA_L;
    ICdelay();
    SCL_L;
    ICdelay();
    return TRUE;
}

/*********************************************
 * 函数名：ICStop
 * 描  述：释放IC总线
 * 输  入：无
 * 输  出：无
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
 * 函数名：ICAck
 * 描  述：发送ASK
 * 输  入：无
 * 输  出：无
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
 * 函数名：ICNoAck
 * 描  述：发送NOASK
 * 输  入：无
 * 输  出：无
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
 * 函数名：ICWaitAck
 * 描  述：读取ACK信号
 * 输  入：无
 * 输  出：TRUE=有ACK,FALSE=无ACK
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
    if(!SDA_read)    //低电平应答
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
 * 函数名：ICSendByte
 * 描  述：MCU发送一个字节
 * 输  入：无
 * 输  出：无
 ********************************************/
static void ICSendByte(uint8_t SendByte) //数据从高位到低位
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
 * 函数名：ICReceiveByte
 * 描  述：MCU读入一个字节
 * 输  入：无
 * 输  出：ReceiveByte
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
        ICNoAck();//发送nACK
    else
        ICAck(); //发送ACK
    return ReceiveByte;   
}
//IIC写一个字节 
//addr:器件地址 
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
static uint8_t IIC_Write_Byte(uint8_t addr,uint8_t reg,uint8_t data) 				 
{ 
    ICStart(); 
	ICSendByte((uint8_t)(addr<<1)|0);//发送器件地址+写命令	
	if(ICWaitAck() == FALSE)	//等待应答
	{
		ICStop();		 
		return 1;		
	}
    ICSendByte(reg);	//写寄存器地址
    ICWaitAck();		//等待应答 
	ICSendByte(data);//发送数据
	if(ICWaitAck() == FALSE)	//等待ACK
	{
		ICStop();	 
		return 1;		 
	}		 
  ICStop();	 
	return 0;
}

//IIC读一个字节 
//addr:器件地址 
//reg:寄存器地址 
//返回值:读到的数据
static uint8_t IIC_Read_Byte(uint8_t addr,uint8_t reg)
{
	uint8_t res_b;
    ICStart(); 
	ICSendByte((uint8_t)(addr<<1)|0);//发送器件地址+写命令	
	ICWaitAck();		//等待应答 
    ICSendByte(reg);	//写寄存器地址
    ICWaitAck();		//等待应答
    ICStart();
	ICSendByte((uint8_t)(addr<<1)|1);//发送器件地址+读命令	
    ICWaitAck();		//等待应答 
	res_b=ICReceiveByte(0);//读取数据,发送nACK 
    ICStop();			//产生一个停止条件 
	return res_b;		
}

//IIC连续写
//addr:器件地址 
//reg:寄存器地址
//len:写入长度
//buf:数据区
//返回值:0,正常
//    其他,错误代码
static uint8_t IIC_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	uint8_t i; 
    ICStart(); 
	ICSendByte((addr<<1)|0);//发送器件地址+写命令	
	if(ICWaitAck() == FALSE)	//等待应答
	{
		ICStop();		 
		return 1;		
	}
    ICSendByte(reg);	//写寄存器地址
    ICWaitAck();		//等待应答
	for(i=0;i<len;i++)
	{
		ICSendByte(buf[i]);	//发送数据
		if(ICWaitAck() == FALSE)		//等待ACK
		{
			ICStop();	 
			return 1;		 
		}		
	}    
    ICStop();	 
	return 0;	
} 

/**************************实现函数********************************************
*函数原型:		uint8_t IICreadByte(uint8_t dev, uint8_t reg, uint8_t *data)
*功　　能:	    读取指定设备 指定寄存器的一个值
输入  addr:器件地址 
     reg	   寄存器地址
		*data  读出的数据将要存放的地址
返回   1
*******************************************************************************/ 
static uint8_t IICreadByte(uint8_t addr,uint8_t reg, uint8_t *data)
{
	*data=IIC_Read_Byte(addr,reg);
    return 1;
}
/**************************实现函数********************************************
*函数原型:		uint8_t IICwriteBits(uint8_t reg,uint8_t bitStart,uint8_t length,uint8_t data)
*功　　能:	    读 修改 写 指定设备 指定寄存器一个字节 中的多个位
输入 addr:器件地址	
    reg	   寄存器地址
		bitStart  目标字节的起始位
		length   位长度
		data    存放改变目标字节位的值
返回   成功 为1 
 		失败为0
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

/**************************实现函数********************************************
*函数原型:		uint8_t IIC5writeBit(uint8_t reg, uint8_t bitNum, uint8_t data)
*功　　能:	    读 修改 写 指定设备 指定寄存器一个字节 中的1个位
输入	addr:器件地址
    reg	   寄存器地址
		bitNum  要修改目标字节的bitNum位
		data  为0 时，目标位将被清0 否则将被置位
返回   成功 为0 
 		失败为1
*******************************************************************************/ 
static uint8_t IICwriteBit(uint8_t addr,uint8_t reg, uint8_t bitNum, uint8_t data){
    uint8_t b;
    IICreadByte(addr,reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return IIC_Write_Byte(addr,reg, b);
}

//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
static uint8_t IIC_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
 	ICStart(); 
	ICSendByte((addr<<1)|0);//发送器件地址+写命令	
	if(ICWaitAck() == FALSE)	//等待应答
	{
		ICStop();		 
		return 1;		
	}
    ICSendByte(reg);	//写寄存器地址
    ICWaitAck();		//等待应答
    ICStart();
	ICSendByte((addr<<1)|1);//发送器件地址+读命令	
    ICWaitAck();		//等待应答 
	while(len)
	{
		if(len==1)
			*buf=ICReceiveByte(0);//读数据,发送nACK 
		else 
			*buf=ICReceiveByte(1);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
    ICStop();	//产生一个停止条件 
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
	IIC_Write_Byte(QMA7981_ADDRESS,QMA7981_RESET,QMA7981_RESET_DAT);     //复位传感器
	IIC_Write_Byte(QMA7981_ADDRESS,QMA7981_RESET,QMA7981_RESET_DEF);    //清除复位标志
	IIC_Write_Byte(QMA7981_ADDRESS,QMA7981_PM,QMA7981_PM_DEF|QMA7981_PM_ACTIVE|QMA7981_PM_MCLK_25);    //设置工作模式  //
	IIC_Write_Byte(QMA7981_ADDRESS,QMA7981_INT_STO,QMA7981_INT_STO_DEF|QMA7981_ANY_MOT_FIRST_Z|QMA7981_ANY_MOT_FIRST_Y|QMA7981_ANY_MOT_FIRST_X);     //设置中断触发源  
	IIC_Write_Byte(QMA7981_ADDRESS,QMA7981_INT_EN2,QMA7981_INT_EN2_DEF|QMA7981_ANY_MOT_EN_X|QMA7981_ANY_MOT_EN_Y|QMA7981_ANY_MOT_EN_Z);          //使能运动中断  
	IIC_Write_Byte(QMA7981_ADDRESS,QMA7981_INT_MAP1,QMA7981_INT_MAP1_DEF|QMA7981_INT1_ANY_MOT);       //设置中断输出引脚   
	IIC_Write_Byte(QMA7981_ADDRESS,QMA7981_MOT_CONF0,0X00);
	IIC_Write_Byte(QMA7981_ADDRESS,QMA7981_MOF_CONF2,shock_da);             //设置运动中断阈值	
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
	data_Register1[0] = data_Register1[0]>>2;     //X轴原始数据
	data_Register1[1] = data_Register1[1]>>2;     //Y轴原始数据
	data_Register1[2] = data_Register1[2]>>2;     //Z轴原始数据				  
  
	SHOCK_AX = (float)data_Register1[0]*244/1000000.0;
	SHOCK_AY = (float)data_Register1[1]*244/1000000.0;
	SHOCK_AZ = (float)data_Register1[2]*244/1000000.0;
	
	char mb_addr[50];
	sprintf(mb_addr, "X轴加速度[g]:%.3f\r\n", SHOCK_AX);
	Trace_log(mb_addr);
	sprintf(mb_addr, "Y轴加速度[g]:%.3f\r\n", SHOCK_AY);
	Trace_log(mb_addr);
	sprintf(mb_addr, "Z轴加速度[g]:%.3f\r\n", SHOCK_AZ);
	Trace_log(mb_addr);
}

////引脚中断
//void GPIO_IRQHandler(void)
//{  
//    if(SET == LL_EXTI_IsActiveFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_5))
//    {
//      LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_5);
//			device_info.sensor_stime = 0;  //静止计数器清零
//			device_info.MPU_SHOCK_DATA = 1;    //设备有震动
//			xTaskNotify(sensoe_handler, NONE, eSetBits);   //传感器数据读取
//    } 
//}
//    

//// 外部引脚中断初始化
//void shock_interrupt_init(void)
//{
//    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};    
//    
//    //引脚边沿触发选择
//    LL_RCC_Enable_SleepmodeExternalInterrupt();//休眠使能外部中断采样
//    LL_RCC_Group1_EnableOperationClock(LL_RCC_OPERATION1_CLOCK_EXTI);//EXTI工作时钟使能    
//    
//    //用到的GPIO引脚    设为输入 
//    GPIO_InitStruct.Pin        = shock_int1_pin;
//		GPIO_InitStruct.Mode       = LL_GPIO_MODE_INPUT;
//		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
//		LL_GPIO_Init(shock_port, &GPIO_InitStruct);

//    LL_EXTI_SetTrigEdge(GPIO_COMMON, LL_GPIO_EXTI_LINE_5, LL_GPIO_EXTI_TRIGGER_NONE);//关闭边沿触发
//    //每条LINE 只能选一个引脚 
//    LL_GPIO_SetExitLine0(GPIO_COMMON, LL_GPIO_EXTI_LINE_5_PB4);//中断引脚选择
//    LL_EXTI_EnableDigitalFilter(GPIO_COMMON, LL_GPIO_EXTI_LINE_5);//EXTI数字滤波功能
//    LL_EXTI_SetTrigEdge(GPIO_COMMON, LL_GPIO_EXTI_LINE_5, LL_GPIO_EXTI_TRIGGER_RISING);//边沿选择  上升沿  
//    LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_5);//清除中断标志
//    
//    /*NVIC中断配置*/
//    NVIC_DisableIRQ(GPIO_IRQn);
//    NVIC_SetPriority(GPIO_IRQn,2);//中断优先级配置
//    NVIC_EnableIRQ(GPIO_IRQn);
//}
