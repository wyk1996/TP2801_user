//#include "JHM1200.h"
//#include "JHM1200_IIC.h"
//// Define the upper and lower limits of the calibration pressure

//// Define the upper and lower limits of the calibration pressure
//#define PMIN    0      // 20000  //Full range pressure for example 20Kpa
//#define PMAX    2.5   //  120000 //Zero Point Pressure Value, for example 120Kpa
//#define DMIN    1677722      // 1677722 //AD value corresponding to pressure zero, for example 10%AD
//#define DMAX    14260633      // 15099494 //AD Value Corresponding to Full Pressure Range, for example 90%AD

////The 7-bit IIC address of the JHM1200 is 0x78
////uint8_t Device_Address = 0xF0;

//////Delay function needs to be defined
////void DelayMs(uint8_t count)
////{
////			for(uint8_t i=0;i<count;i++)
////	{
////		__nop();		__nop();		__nop();		__nop();
////		__nop();		__nop();		__nop();		__nop();
////	}
////}

// extern MPA MPAbuf;
//unsigned char  BSP_IIC_Write(unsigned char address, unsigned char *buf, unsigned short count)
//{
//	// Prepare IIC address with read bit 0
////	address &= 0xFE;
//	IIC2_Start();
//		delay(5);
//	IIC2_Send_Byte(address);
//		delay(5);	
//  IIC2_Wait_Ack();	
//	//IIC2_Ack();
////		
////		// IIC master Send data bytes & check ACK
//	while (count)
//	{
//		IIC2_Send_Byte(*buf);
//		delay(5);
//    IIC2_Wait_Ack();
//	//	IIC2_Ack();	
//		buf++;
//		count--;
//	}
//	 delay(5);
//	 IIC2_Stop();
//	
//	return 1;
//}
//uint8_t BSP_IIC_Read(uint8_t address, uint8_t *buf, uint16_t count)
//{	  	
//	uint8_t i;
////	address |= 0x01;
//	IIC2_Start();
//		delay(5);
//	IIC2_Send_Byte(address);
//		delay(5);	
//  IIC2_Wait_Ack();	
//	for(i=0;i<(count-1);i++)  //5
//	{
//		*buf++=IIC2_Read_Byte(1);
//	}
//	  *buf=IIC2_Read_Byte(0);
//	  delay(5);
//	 IIC2_Stop();
//	
//	return 1;

//}
//////Read the status of IIC and judge whether IIC is busy
//uint8_t JHM1200_IsBusy(void)
//{
//	uint8_t status;
//	BSP_IIC_Read(0xF1, &status, 1);
//	status = (status >> 5) & 0x01;
//	return status;
//}

///**
//  * @brief Using the 0xAC command to calculate the actual pressure and temperature using the JHM1200 internal algorithm
//  * @note  Send 0xAC, read IIC status until IIC is not busy
//  *	@note  The returned data is a total of six bytes, in order: status word, three-byte pressure value, two-byte temperature value
//  * @note  The returned three-byte pressure value is proportional to the 24-bit maximum value 16777216. According to this ratio, 
//           the actual pressure value is again converted according to the calibration range.
//  * @note  The returned two-byte temperature value is proportional to the 16-bit maximum value 65536. According to this ratio, 
//           the actual pressure value is again converted according to the calibration range.
//  * @note  Zero pressure point and full pressure point of calibration pressure correspond to 20kpa and 120Kpa, respectively   
//  * @note  The zero point of the calibration temperature is -40°C and the full point is 150°C
//  * @note  The pressure actual value is calculated according to the span pressure unit is Pa, temperature actual value temp unit is 0.01°C
//  */
///**
//	*	0x00-0x1F	从OTP中读取数据；地址和指令是相同的
//	*	0x40-0x5F 向OTP中写数据地址是指令减去0x40
//	*	0xA0-0xA7	执行一次传感器测量，将测量到为经过校准的ADC数据写入寄存器中
//	*	0xA8			退出命令模式，进入正常模式
//	*	0xA9			退出正常模式，进入命令模式
//	*	0xAA			如果CRC校验值还没有写入OTP，指令对OTP的数据进行校验并将校验码写入OTP中
//	*	0xAC			将24位校准后的电桥值和16位的温度值，使用OTP中的配置进行校准并将校准后的值写入接口
//	*	0xB0-0xBF 24位校准后的电桥值和16位校准后的温度值 和 0XAC指令大体相同，单过采样率不由OTP指定，而有指令直接指定
//	* 0xA0,0x0000			电桥测量	AFE设置按地址0x14OTP地址设置
//	*	0xA1,0ssss			电桥测量	AFE的设置按ssss的内容设置,ssss的内容和格式参见0x14OTP地址进行设置
//	*	0xA2,0x0000			经过校准过的电桥测量	AFE设置按地址0x14OTP地址设置
//	*	0xA3,0xssss			经过校准过的电桥测量	AFE的设置按ssss的内容设置,ssss的内容和格式参见0x14OTP地址进行设置
//	*	0xA4,0x0000			温度测量	AFE设置按地址0x14OTP地址设置
//	×	0xA5,0xssss			温度测量	AFE的设置按ssss的内容设置,ssss的内容和格式参见0x14OTP地址进行设置
//	×	0xA6,0x0000			经过校准过的温度测量	AFE设置按地址0x14OTP地址设置
//	×	0xA7,0xssss			经过校准过的温度测量 	AFE的设置按ssss的内容设置,ssss的内容和格式参见0x14OTP地址进行设置
//	*/

//void JHM1200_get_cal(void)
//{
//	 	 uint8_t buffer[6] = {0};
//	uint32_t press_raw = 0;
//	uint16_t temp_raw = 0;
//	float pressure = 0.0, temp = 0.0;

//	buffer[0] = 0xAC;
//	BSP_IIC_Write(0xf0, buffer, 1);
//	delay(5);  
//	LL_mDelay(250);
//	while (1)
//	{
//		if (JHM1200_IsBusy())
//		{
//			LL_mDelay(1);
//  
//		}
//		else
//			break;
//	}
//	BSP_IIC_Read(0xf1, buffer, 6);
//	//The returned pressure and temperature values are converted into actual values according to the calibration range
//	press_raw = ((uint32_t)buffer[1] << 16) | ((uint16_t)buffer[2] << 8) | buffer[3];
//	temp_raw = ((uint16_t)buffer[4] << 8) | (buffer[5] << 0);
//	pressure = (PMAX-PMIN)/(DMAX-DMIN)*(press_raw-DMIN)+PMIN;
//	temp = (float)temp_raw / 65536;
//	temp = temp * 190 - 40;

//}


