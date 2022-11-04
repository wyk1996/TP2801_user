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

int IntToBCD(int i) //十进制转BCD
{
	return (((i / 10) << 4) + ((i % 10) & 0x0f));
}
int BCDToInt(int bcd) //BCD转十进制
{
	return (0xff & (bcd >> 4)) * 10 + (0xf & bcd);
}
 
//比较两个字符串指定长度的内容是否相等
//参数:s1,s2要比较的两个字符串;len,比较长度
//返回值:1,相等;0,不相等
uint8_t my_strcmp(uint8_t*s1,uint8_t*s2,uint8_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)if((*s1++)!=*s2++)return 0;
	return 1;	   
}

/*********************************************
 * 函数名：I2Cdelay
 * 描  述：I2C延时函数
 * 输  入：无
 * 输  出：无
 ********************************************/
static void I2Cdelay(void)
{	
   uint8_t i=100; //这里可以优化速度，通常延时3~10us，可以用示波器看波形来调试
   while(i) 
   { 
     i--; 
   }
}

/*********************************************
 * 函数名：IIC_Init
 * 描  述：I2C端口初始化
 * 输  入：无
 * 输  出：无
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
void SD3078_IIC3_deinit(void )    //有外部上拉电阻
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
	GPIO_Initure.Pull=DISABLE;          //上拉	    
	LL_GPIO_Init(GPIOA,&GPIO_Initure);
}
//rtc中断引脚
//void rtc_extioinit(void)
//{
//    LL_GPIO_InitTypeDef gpioInitStruct = {0};	

//    LL_CMU_SetEXTIClockSource(CMU_OPCCR1_EXTICKS);      // 外部中断时钟选择LSCLK
//    LL_CMU_EnableGroup3OperationClock(LL_CMU_GROUP3_OPCLK_EXTI);    // 使能外部中断时钟		

//    // PB0中断输入
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
//    // 中断优先级配置
//    NVIC_DisableIRQ(GPIO_IRQn);
//    NVIC_SetPriority(GPIO_IRQn, 2);
//    NVIC_EnableIRQ(GPIO_IRQn);
//}


/*********************************************
 * 函数名：I2CStart
 * 描  述：开启I2C总线
 * 输  入：无
 * 输  出：TRUE:操作成功，FALSE:操作失败
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
			return FALSE;	//SDA线为低电平则总线忙,退出
		}
	  gpio_set_SDA_mode(gpio_output);
    SDA_L;
    I2Cdelay();
    SCL_L;
    I2Cdelay();
    return TRUE;
}

/*********************************************
 * 函数名：I2CStop
 * 描  述：释放I2C总线
 * 输  入：无
 * 输  出：无
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
 * 函数名：I2CAck
 * 描  述：发送ASK
 * 输  入：无
 * 输  出：无
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
 * 函数名：I2CNoAck
 * 描  述：发送NOASK
 * 输  入：无
 * 输  出：无
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
 * 函数名：I2CWaitAck
 * 描  述：读取ACK信号
 * 输  入：无
 * 输  出：TRUE=有ACK,FALSE=无ACK
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
 * 函数名：I2CSendByte
 * 描  述：MCU发送一个字节
 * 输  入：无
 * 输  出：无
 ********************************************/
static void I2CSendByte(uint8_t SendByte) //数据从高位到低位
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
 * 函数名：I2CReceiveByte
 * 描  述：MCU读入一个字节
 * 输  入：无
 * 输  出：ReceiveByte
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
 * 函数名：WriteRTC_Enable
 * 描  述：RTC写允许程序
 * 输  入：无
 * 输  出：TRUE:操作成功，FALSE:操作失败
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
    I2CSendByte(0x80);//置WRTC1=1      
    I2CWaitAck();
    I2CStop(); 
										
    I2CStart();
    I2CSendByte(RTC_Address);      
    I2CWaitAck();   
    I2CSendByte(CTR1);
    I2CWaitAck();	
    I2CSendByte(0x84);//置WRTC2,WRTC3=1      
    I2CWaitAck();
    I2CStop(); 
//		portENABLE_INTERRUPTS();
    return TRUE;
}

/*********************************************
 * 函数名：WriteRTC_Disable
 * 描  述：RTC写禁止程序
 * 输  入：无
 * 输  出：TRUE:操作成功，FALSE:操作失败
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
    I2CSendByte(CTR1);//设置写地址0FH      
    I2CWaitAck();	
    I2CSendByte(0x0) ;//置WRTC2,WRTC3=0      
    I2CWaitAck();
    I2CSendByte(0x0) ;//置WRTC1=0(10H地址)      
    I2CWaitAck();
    I2CStop(); 
//		portENABLE_INTERRUPTS();
    return TRUE; 
}

/*********************************************
 * 函数名：RTC_WriteDate
 * 描  述：写RTC实时数据寄存器
 * 输  入：时间结构体指针
 * 输  出：TRUE:操作成功，FALSE:操作失败
 ********************************************/
uint8_t RTC_WriteDate(rtc_t	*psRTC)	
{
	//写时间操作要求一次对实时时间寄存器(00H~06H)依次写入，
	//不可以单独对7个时间数据中的某一位进行写操作,否则可能会引起时间数据的错误进位. 
	//要修改其中某一个数据 , 应一次性写入全部 7 个实时时钟数据.
	uint8_t time_buf[7];
	time_buf[6]=IntToBCD(psRTC->year);
	time_buf[5]=IntToBCD(psRTC->month);
	time_buf[4]=IntToBCD(psRTC->day);
	time_buf[3]=IntToBCD(psRTC->week );
	time_buf[2]=IntToBCD( psRTC->hour);
	time_buf[1]=IntToBCD(psRTC->min);
	time_buf[0]=IntToBCD(psRTC->sec );
	WriteRTC_Enable();				//使能，开锁
	I2CStart();
	I2CSendByte(RTC_Address); 
	if(I2CWaitAck() == FALSE) {
		I2CStop(); 
		return FALSE;
	}
	I2CSendByte(0);			//设置写起始地址      
	I2CWaitAck();	
	I2CSendByte(time_buf[0]);		 //second     
	I2CWaitAck();	
	I2CSendByte(time_buf[1]);		 //minute      
	I2CWaitAck();	
	I2CSendByte(time_buf[2]|0x80);//hour ,同时设置小时寄存器最高位（0：为12小时制，1：为24小时制）
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
	WriteRTC_Disable();				  //关锁
	return	TRUE;
}

/*********************************************
 * 函数名：RTC_ReadDate
 * 描  述：读RTC实时数据寄存器
 * 输  入：时间结构体指针
 * 输  出：TRUE:操作成功，FALSE:操作失败
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
	
	sMyTime.buff[0]=BCDToInt(sMyTime.sec);    //十进制
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
 * 函数名     ：I2CWriteSerial
 * 描  述     ：I2C在指定地址写一字节数据
 * Device_Addr：I2C设备地址
 * Address    ：内部地址
 * length     ：字节长度
 * ps         ：缓存区指针
 * 输出       ：TRUE 成功，FALSE 失败
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
 * 函数名     ：I2CReadSerial
 * 描  述     ：I2C在指定地址读一字节数据
 * Device_Addr：I2C设备地址
 * Address    ：内部地址
 * length     ：字节长度
 * ps         ：缓存区指针
 * 输出       ：TRUE 成功，FALSE 失败
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
 * 函数名：Set_CountDown
 * 描  述：设置倒计时中断
 * 输  入：CountDown_Init 倒计时中断结构体指针 
 * 输  出：无
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
 * 函数名：Set_Alarm
 * 描  述：设置报警中断（闹钟功能）
 * Enable_config：使能设置  
 * psRTC：报警时间的时间结构体指针
 * 输  出：无
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
 * 函数名：SetFrq
 * 描  述：设置RTC频率中断，从INT脚输出频率方波
 * 输  入：频率值
 * 输  出：无
 ********************************************/
void SetFrq(enum Freq F_Out)					
{
		uint8_t buf[2];
		buf[0] = 0xA1;
		buf[1] = F_Out;
		I2CWriteSerial(RTC_Address,CTR2,2,buf);
}

/*********************************************
 * 函数名：ClrINT
 * 描  述：禁止中断
 * int_EN：中断类型 INTDE、INTDE、INTDE
 * 输  出：无
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
///**************** 频率中断设置演示 ****************/
//	SetFrq(F2Hz);//输出2Hz频率方波
//#elif (INT_TYPE == ALARM)
///**************** 报警中断设置演示 ****************/
//	Set_Alarm(sec_ALM|min_ALM|hor_ALM, &Alarm_init);//报警使能：时、分、秒
//#elif (INT_TYPE == COUNTDOWN)	
/*************** 倒计时中断设置演示***************/
	cdInit.IM = 1;				//设置为周期性中断
	cdInit.d_clk = S_1s;		//倒计时中断源选择1s
//	if(device_info.NOSLEEP_MODE == 1)
//		cdInit.init_val = 1;	//不休眠模式下，中断定时间隔为1秒
//	else
		cdInit.init_val = 10;	//倒计时初值设置为10
//	device_info.heart_time = 10;
	Set_CountDown(&cdInit);
//#elif (INT_TYPE == DISABLE)	
///****************** 禁止中断演示 *****************/
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
	device_info.alarm_barck_flag++;   //10秒闹钟定时器
	device_info.wait_min++;	  //上报计时时间点累加
	device_info.vabt_timer++;    //电量更新时间点累加
//	device_info.gps_ec200s_time++;    //4g gps定位时间间隔计数
//	xTaskNotifyFromISR(sleepHandle, 0x00, eSetBits, &xHigherPriorityTaskWoken);	
//  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
/****************************************************************************************/
uint8_t sd3078_ram_set(uint8_t Address, uint32_t setbuf)
{
	uint8_t i;
	uint8_t ram_setbuf[4];//转换为字节
  for(i=0;i<4;i++)
  {
    ram_setbuf[i]=(setbuf>>((3-i)*8))& 0xFF;       //高  低 
  }
 	i=I2CWriteSerial(RTC_Address,Address,4,ram_setbuf);
	return i;
}

/****************************************************************************************/
uint32_t sd3078_ram_get(uint8_t Address)
{
	uint8_t ram_getbuf[4];
  uint32_t getbuf;
//转换为字节
 I2CReadSerial(RTC_Address, Address, 4, ram_getbuf);
 getbuf=(uint32_t)(ram_getbuf[0]<<24)|(uint32_t)(ram_getbuf[1]<<16)|(uint32_t)(ram_getbuf[2]<<8)|ram_getbuf[3];
 return  getbuf;
}

/*********************************************END OF FILE**********************/


