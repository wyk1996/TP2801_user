/*
 * @Author: your name
 * @Date: 2020-08-10 14:57:07
 * @LastEditTime: 2020-08-25 16:03:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \FM32_NB温湿度\BSP\Src\rtc.c
 */
/* Includes ------------------------------------------------------------------*/
#include "rtc.h"
#include "main.h"
/* 函数原型 ----------------------------------------------------------*/
/* 宏  ---------------------------------------------------------------*/
#define __LL_RTC_CONVERT_BCD2BIN(__VALUE__) (uint8_t)(((uint8_t)((__VALUE__) & (uint8_t)0xF0U) >> (uint8_t)0x4U) * 10U + ((__VALUE__) & (uint8_t)0x0FU))
#define __LL_RTC_CONVERT_BIN2BCD(__VALUE__) (uint8_t)((((__VALUE__) / 10U) << 4U) | ((__VALUE__) % 10U))

/* 变量 --------------------------------------------------------------*/
/* 类型定义 ----------------------------------------------------------*/
/* 函数声明 ----------------------------------------------------------*/
/* 函数代码 ----------------------------------------------------------*/
typedef struct
{     
    uint8_t            Hour;        /*!<RTC闹钟 时*/        
    uint8_t            Minute;        /*!<RTC闹钟 分*/
    uint8_t            Second;        /*!<RTC闹钟 秒*/

}RTC_AlarmTmieTypeDef;
RTC_AlarmTmieTypeDef      Alarm;
LL_RTC_InitTypeDef      TempTime;
/**
 * @description: RTC中断
 * @param  None
 * @return None
 */
// RTC读写时间和秒中断示例
//void RTC_IRQHandler(void)    
//{
//    if(ENABLE == LL_RTCA_IsEnabledIT_Alarm(RTCA) &&
//         SET == LL_RTCA_IsActiveFlag_Alarm(RTCA))//查询闹钟中断标志是否置起
//    {       
//        LL_RTCA_ClearFlag_Alarm(RTCA);        //清除闹钟中断标志
//			  iwdt_reload();  //喂狗
////			  Trace_log("喂狗\r\n");
//			  rtc_set_alarm(10);
//			  if(device_info.MCU_statues == 2)
//				{
//					DeepSleep();
//				}
//    }
//}
/**
 * @description: 设置闹钟
 * @param  sec: 下次闹钟唤醒的时间
 * @return 1: 写入失败 0:写入成功
 */

uint8_t rtc_read_time(LL_RTC_InitTypeDef* date_time)
{
	uint8_t n, i;
	uint8_t result = 1;

	LL_RTC_InitTypeDef temp_time1,temp_time2;
	
	for(n=0 ;n < 3; n++)
	{
		LL_RTC_GetTime(RTC,&temp_time1);//读一次时间
		LL_RTC_GetTime(RTC,&temp_time2);//再读一次时间
		
		for(i=0; i<7; i++)//两者一致, 表示读取成功
		{
			if(((uint32_t*)(&temp_time1))[i] != ((uint32_t*)(&temp_time2))[i]) break;
		}
		if(i == 7)
		{
			result = 0;
			memcpy((uint32_t*)(date_time), (uint32_t*)(&temp_time1), 7*sizeof(uint32_t));//读取正确则更新新的时间		
			break;
		}
	}
	date_time->Year = __LL_RTC_CONVERT_BCD2BIN(date_time->Year);
	date_time->Day = __LL_RTC_CONVERT_BCD2BIN(date_time->Day);
	date_time->Hour = __LL_RTC_CONVERT_BCD2BIN(date_time->Hour);
	date_time->Minute = __LL_RTC_CONVERT_BCD2BIN(date_time->Minute);
	date_time->Month = __LL_RTC_CONVERT_BCD2BIN(date_time->Month);
	date_time->Second = __LL_RTC_CONVERT_BCD2BIN(date_time->Second);
	date_time->Week = __LL_RTC_CONVERT_BCD2BIN(date_time->Week);
	return result;
}

void rtc_set_alarm(uint8_t sec)
{
//	Alarm.second += 0x13;
//	if(Alarm.second>=0x60)
//		Alarm.second = 0x00;
//	LL_RTCA_WriteSecondAlarm(RTCA,Alarm.second);
		uint8_t n;
	rtc_read_time(&TempTime);
    
	TempTime.Minute += (TempTime.Second + sec) / 60;

	if(TempTime.Minute >= 60)
	{
		TempTime.Minute -= 60;
		TempTime.Hour = TempTime.Hour >= 23 ? 0 : TempTime.Hour + 1;
	}
	TempTime.Second = (TempTime.Second + sec) % 60;
	
	for(n = 0 ;n < 3; n++)
	{
		LL_RTC_SetHours_Alarm(RTC, __LL_RTC_CONVERT_BIN2BCD(TempTime.Hour));
		LL_RTC_SetMinutes_Alarm(RTC, __LL_RTC_CONVERT_BIN2BCD(TempTime.Minute));
		LL_RTC_SetSecond_Alarm(RTC, __LL_RTC_CONVERT_BIN2BCD(TempTime.Second));
		
		
		if((RTC->ALARM >> 16 & 0xff) == __LL_RTC_CONVERT_BIN2BCD(TempTime.Hour) && \
			 (RTC->ALARM >> 8 & 0xff) == __LL_RTC_CONVERT_BIN2BCD(TempTime.Minute) && \
			 (RTC->ALARM & 0xff) == __LL_RTC_CONVERT_BIN2BCD(TempTime.Second))
		{
			Trace_log("Y\r\n");
			break;
		}
	}
}	


void RTC_Init(void)
{        
//    LL_CMU_EnableGroup1BusClock(LL_CMU_GROUP1_BUSCLK_RTCA);//时钟打开
//    LL_RTC_Enable(RTCA);     //RTCA使能
    
  TempTime.Year   = 0x20;
	TempTime.Month  = 0x02;
	TempTime.Day    = 0x21;	
	TempTime.Week   = 0x04;	
	TempTime.Hour   = 0x09;
	TempTime.Minute = 0x00;
	TempTime.Second = 0x00;	
	LL_RTC_Init(RTC,&TempTime);  
}

void SET_RTC_Alarm(void)
{    
	
	RTC_Init();
	Alarm.Hour   = 0x09;
	Alarm.Minute = 0x00;
	Alarm.Second = 0x10;
	LL_RTC_SetHours_Alarm(RTC,Alarm.Hour);
	LL_RTC_SetMinutes_Alarm(RTC,Alarm.Minute);
	LL_RTC_SetSecond_Alarm(RTC,Alarm.Second);
	
	
	LL_RTC_ClearFlag_Alarm(RTC);		//清除闹钟中断标志
	LL_RTC_EnableIT_Alarm(RTC);
	NVIC_DisableIRQ(RTC_IRQn);				//NVIC中断控制器配置
	NVIC_SetPriority(RTC_IRQn,2);
	NVIC_EnableIRQ(RTC_IRQn);	 
}
