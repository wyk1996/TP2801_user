#include "main.h"
#include "DS18B20.h"

//#define ds18b20_de1_2_port   GPIOC
//#define ds18b20_de1_pin      LL_GPIO_PIN_4
//#define ds18b20_de2_pin      LL_GPIO_PIN_5

#define ds18b20_de3_port     GPIOA
#define ds18b20_de3_pin      LL_GPIO_PIN_4

//#define ds1     0x01
//#define ds2     0x02
//#define ds3     0x03

//#define  DWT_CYCCNT  *(volatile unsigned int *)0xE0001004
/******************************************************************************
Func:8位循环冗余校验算法
Date：2016-5-6
Note:
******************************************************************************/
uint8_t Tools_GetCRC8(uint8_t *ptr,uint16_t len)
{
	uint8_t crc;
	uint8_t i;
	crc = 0;
	while(len--)
	{
		crc ^= *ptr++;
		for(i = 0;i < 8;i++)
		{
			 if(crc & 0x01)
			 {
					 crc = (crc>>1)^0x8C;
			 }else crc >>= 1;
		}
	}
	return crc;
}
/*****************************************************************/
static void init_gpio_out(GPIO_Type *gpiox,uint32_t gpiop)   //IO输入输出模式设置
{

	LL_GPIO_InitTypeDef GPIO_Initure;

	GPIO_Initure.Mode=LL_GPIO_MODE_OUTPUT; 
	GPIO_Initure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;		
	
	GPIO_Initure.Pin=gpiop;                       //SDA  	
	GPIO_Initure.Pull=DISABLE;          //上拉	    
	LL_GPIO_Init(gpiox,&GPIO_Initure);
}
/*****************************************************************/
static void init_gpio_input(GPIO_Type *gpiox,uint32_t gpiop)   //IO输入输出模式设置
{
	
	LL_GPIO_EnablePinInput(gpiox,gpiop);
	LL_GPIO_SetPinMode(gpiox, gpiop, LL_GPIO_MODE_INPUT);
}

/*****************************************************************/
/*****************************************************************/
static uint8_t DS18B20_init(GPIO_Type *gpiox,uint32_t gpiop)
{
	uint16_t time=0;
	
	init_gpio_out(gpiox,gpiop);
	LL_GPIO_SetOutputPin(gpiox,gpiop);
	LL_GPIO_ResetOutputPin(gpiox,gpiop);
	delay_10pus(480);
	LL_GPIO_SetOutputPin(gpiox,gpiop);	
	init_gpio_input(gpiox,gpiop);
	delay_10pus(10);
	while(LL_GPIO_IsInputPinSet(gpiox,gpiop))
	{
		time++;
		delay_10pus(1);
		if(time>100)
			return FALSE;
	}
	time = 0;
	while(!LL_GPIO_IsInputPinSet(gpiox,gpiop))
	{
		time++;
		delay_10pus(1);
		if(time>300)
			return FALSE;
	}
//	delay_10pus(50);
	init_gpio_out(gpiox,gpiop);
	LL_GPIO_SetOutputPin(gpiox,gpiop);
	delay_10pus(50);
	return TRUE;
}
/*****************************************************************/
static uint8_t DS18B20_send_byte(GPIO_Type *gpiox,uint32_t gpiop,uint8_t data)
{
	uint8_t i;
	
	init_gpio_out(gpiox,gpiop);
	LL_GPIO_SetOutputPin(gpiox,gpiop);
	for(i=0;i<8;i++)
	{
		LL_GPIO_ResetOutputPin(gpiox,gpiop);
		delay_10pus(1);
		if(data&0x01)
			LL_GPIO_SetOutputPin(gpiox,gpiop);
		else
			LL_GPIO_ResetOutputPin(gpiox,gpiop);
		delay_10pus(50);
		LL_GPIO_SetOutputPin(gpiox,gpiop);
		delay_10pus(1);
		data>>=1;
	}
}
/*****************************************************************/
static uint8_t DS18B20_read_byte(GPIO_Type *gpiox,uint32_t gpiop)
{
	uint16_t time=0;
	uint8_t i,data;
		
	data = 0;
	for(i=0;i<8;i++)
	{
		init_gpio_out(gpiox,gpiop);
		data>>=1;
		LL_GPIO_ResetOutputPin(gpiox,gpiop);
		
	  LL_GPIO_SetPinMode(gpiox, gpiop, LL_GPIO_MODE_INPUT);
		LL_GPIO_EnablePinInput(gpiox,gpiop);
		delay_10pus(1);
		if(LL_GPIO_IsInputPinSet(gpiox,gpiop))
			data |= 0x80;
		delay_10pus(10);
	}
	return data;
}
/*****************************************************************/

static uint8_t DS18B20_read_temp(void)
{
	uint16_t temp;
	uint8_t i;
	uint8_t re;
	uint8_t tem_buff[10];
	float ds_data;
	GPIO_Type *gpiox;
	uint32_t gpiop;
//	switch(ds)
//	{
//		case ds1:gpiox = ds18b20_de1_2_port;gpiop = ds18b20_de1_pin;break;
//		case ds2:gpiox = ds18b20_de1_2_port;gpiop = ds18b20_de2_pin;break;
//		case ds3:gpiox = ds18b20_de3_port;gpiop = ds18b20_de3_pin;break;	
//		default:break;		
//	}
	gpiox = ds18b20_de3_port;
	gpiop = ds18b20_de3_pin;
	if(DS18B20_init(gpiox,gpiop) == FALSE)
	{
		re = FALSE;
		goto end;
	}
	
	portDISABLE_INTERRUPTS();
	DS18B20_send_byte(gpiox,gpiop,0xcc);
	DS18B20_send_byte(gpiox,gpiop,0x44);
	portENABLE_INTERRUPTS();
	delay_vtask(1);
	
	if(DS18B20_init(gpiox,gpiop) == FALSE)
	{
		re = FALSE;
		goto end;
	}
	portDISABLE_INTERRUPTS();
	DS18B20_send_byte(gpiox,gpiop,0xcc);
	DS18B20_send_byte(gpiox,gpiop,0xbe);
  delay_10pus(1);
  for	(i=0;i<9;i++)
	{
		tem_buff[i] = DS18B20_read_byte(gpiox,gpiop);
	}
	
//	teml = DS18B20_read_byte(ds);
//	temh = DS18B20_read_byte(ds);
	portENABLE_INTERRUPTS();
	if(device_info.debug_en)
		uart_transmit_buf(&COM0,tem_buff,10);
	tem_buff[9] = Tools_GetCRC8(tem_buff,8);
	if(device_info.debug_en)
		uart_transmit_buf(&COM0,tem_buff,10);
	if(tem_buff[9] != tem_buff[8])
	{
		Trace_log("DS18B20读取失败\r\n");
		re = FALSE;
		goto end;
	}
	else
		Trace_log("DS18B20读取成功\r\n");
	
	temp = tem_buff[1];
	temp<<=8;
	temp |= tem_buff[0];
	
	if(tem_buff[1]>0x07)
		ds_data = (float)((~temp)+1)*0.0625;
	else
		ds_data = (float)temp*0.0625;
	
	if(ds_data == 85 || ds_data<-60 || ds_data>130)
	{
		Trace_log("DS18B20数据异常\r\n");
		re = FALSE;
	}
	else
		re = TRUE;
	
	end:
	if(re == TRUE)
	{
//		MPAbuf.sensor1data = ds_data;
//		MPAbuf.sensor2data = 0;
	}
		
	return re;
}
/*****************************************************************/
uint8_t read_DS18B20(void)
{
//	uint8_t r_num;
//	if(device_info.save.sever_type>=1)
//	{
//		Trace_log("DS18B20读取1\r\n");
//		for(r_num=0;r_num<10;r_num++)
//		{
//			vTaskDelay(300);
//			if(DS18B20_read_temp(ds1) == TRUE)
//				break;
//		}
//	}
//	if(device_info.save.sever_type>=2)
//	{
//		Trace_log("DS18B20读取2\r\n");
//		for(r_num=0;r_num<10;r_num++)
//		{
//			vTaskDelay(300);
//			if(DS18B20_read_temp(ds2) == TRUE)
//				break;
//		}
//	}
//	if(device_info.save.sever_type==3)
//	{
		Trace_log("DS18B20读取\r\n");
//		for(r_num=0;r_num<10;r_num++)
//		{
//			vTaskDelay(300);
//			if(DS18B20_read_temp() == TRUE)
//				break;
//		}		
//	}
	return DS18B20_read_temp();
}
/*****************************************************************/
