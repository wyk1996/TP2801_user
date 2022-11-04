#ifndef _SD3078_H_
#define _SD3078_H_

#include "main.h"
#include "device.h"

typedef struct My_Time
{
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t week;
	uint8_t buff[10];
} MY_TIME,*time_p;


enum Freq{F_0Hz, F32KHz, F4096Hz, F1024Hz, F64Hz, F32Hz, F16Hz, F8Hz, \
					F4Hz, F2Hz, F1Hz, F1_2Hz, F1_4Hz, F1_8Hz, F1_16Hz, F_1s};
enum clk_Souce{S_4096Hz, S_1024Hz, S_1s, S_1min};
/*此结构体定义了倒计时中断可供配置的频率源、IM和初值主要参数*/
typedef	struct
{
	enum clk_Souce d_clk;
	uint8_t   IM;	//IM=1:周期性中断
	uint32_t  init_val;
} CountDown_Def;

///*
// * 通过宏定义选择使用的RTC型号
// * SD30XX_25XX -> 使用SD30或SD25系列
// * SD20XX_24XX -> 使用SD20或SD24系列
// */
//#define		SD30XX_25XX
//#undef		SD20XX_24XX

/********************************************************/
#define		H               1
#define		L               0
#define		Chg_enable			0x82
#define		Chg_disable			0

/******************** Device Address ********************/
#define		RTC_Address     0x64 

/******************** Alarm register ********************/
#define		Alarm_SC				0x07
#define		Alarm_MN				0x08
#define		Alarm_HR				0x09
#define		Alarm_WK				0x0A
#define		Alarm_DY				0x0B
#define		Alarm_MO				0x0C
#define		Alarm_YR				0x0D
#define		Alarm_EN				0x0E

/******************** Control Register *******************/
#define		CTR1            0x0F
#define		CTR2            0x10
#define		CTR3            0x11

/***************** Timer Counter Register ****************/
#define		Timer_Counter1	0x13
#define		Timer_Counter2	0x14
#define		Timer_Counter3	0x15

/******************** Battery Register ********************/
#define		Chg_MG          0x18		//充电管理寄存器地址
#define		Bat_H8          0x1A		//电量最高位寄存器地址
#define		Bat_L8          0x1B		//电量低八位寄存器地址


/*********************Users RAM    2CH----71H  ******************************/
#define   user_ram1       0x2c     /***4 byte ***/   //电池电量存储
#define   user_ram2       0x30    //断点续传地址
#define   user_ram3       0x34    //断点续传个数
#define   user_ram4       0x38    //看门狗复位标志
#define   user_ram5       0x3c   //
#define   user_ram6       0x40   //
#define   user_ram7       0x44   //重启次数存储
#define   user_ram8       0x48   //历史记录存储地址

/*********************** ID Register **********************/
#define		ID_Address			0x72		//ID号起始地址

/********************** 报警中断宏定义 *********************/
#define		sec_ALM					0x01
#define		min_ALM					0x02
#define		hor_ALM					0x04
#define		wek_ALM					0x08
#define		day_ALM					0x10
#define		mon_ALM					0x20
#define		yar_ALM					0x40

/********************** 中断使能宏定义 **********************/
#define		INTDE						0x04		//倒计时中断
#define		INTAE						0x02		//报警中断
#define		INTFE						0x01		//频率中断

/********************** 中断演示宏定义 **********************/
//#define 	FREQUENCY				0				//频率中断
//#define 	ALARM						1				//报警中断
//#define 	COUNTDOWN				2				//倒计时中断
//#define 	DISABLEINT					3				//禁止中断

/*************** 中断输出类型选择，请自行选择 ****************/
#define 	INT_TYPE			COUNTDOWN //定义中断输出类型

/***********读写时间函数*************/

extern MY_TIME sMyTime;
uint8_t RTC_WriteDate(rtc_t	*psRTC);
uint8_t RTC_ReadDate(rtc_t	*psRTC);

/*******I2C多字节连续读写函数********/
uint8_t I2CWriteSerial(uint8_t DeviceAddress,uint8_t Address,uint8_t length,uint8_t *ps);
uint8_t I2CReadSerial(uint8_t DeviceAddress,uint8_t Address,uint8_t length,uint8_t *ps);
/*********I2C端口初始化函数*********/
void SD3078_IIC3_init(void );
void SD3078_IIC3_deinit(void);

int IntToBCD(int i); //十进制转BCD
int BCDToInt(int bcd); //BCD转十进制
/*********RTC中断配置函数*********/
void Set_CountDown(CountDown_Def *CountDown_Init);
void Set_Alarm(uint8_t Enable_config, rtc_t *psRTC);
void SetFrq(enum Freq F_Out);
void ClrINT(uint8_t int_EN);
void sd3078_alarm(void);
void sd3078_dealarm(void); 
void sd3078_alarm_Callback(void);
uint8_t sd3078_ram_set(uint8_t Address, uint32_t setbuf);
uint32_t sd3078_ram_get(uint8_t Address);
//void rtc_extioinit(void);
#endif /* __RTC_H */

