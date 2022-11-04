#ifndef _isl1208_H_
#define _isl1208_H_

#include "device.h"
#include "main.h"
#define SDA3_PIN   LL_GPIO_PIN_5
#define SCL3_PIN   LL_GPIO_PIN_4

#define IIC3_SCL_H()   GPIOB->BSRR|=SCL3_PIN //输出
#define IIC3_SCL_L()   GPIOB->BRR|=SCL3_PIN //输出

#define IIC3_SDA_H()   GPIOB->BSRR|=SDA3_PIN  //输出 
#define IIC3_SDA_L()   GPIOB->BRR|=SDA3_PIN   
#define isl1208 0
#define ds1337 1
#if isl1208 
#define ISLWR  0xde        //写辨识字节
#define ISLRD  0Xdf       //读辨识字节


#define ISL1208_ADDR			0x6F
//地址
#define ISL1208_RTC_SECOND 		0x00   //时间
#define ISL1208_RTC_MINUTE		0x01
#define ISL1208_RTC_HOUR		0x02
#define ISL1208_RTC_DAY			0x03
#define ISL1208_RTC_MONTH		0x04
#define ISL1208_RTC_YEAR		0x05
#define ISL1208_RTC_WEEK		0x06

#define ISL1208_SR_REG			0x07     //控制&状态
#define ISL1208_INT_REG			0x08
#define ISL1208_ATR_REG			0x0A
#define ISL1208_DTR_REG			0x0B

#define ISL1208_ALM_SECOND 		0x0C   //报警
#define ISL1208_ALM_MINUTE		0x0D
#define ISL1208_ALM_HOUR		0x0E
#define ISL1208_ALM_DAY			0x0F
#define ISL1208_ALM_MONTH		0x10
#define ISL1208_ALM_WEEK		0x11

#define ISL1208_USER_DAT0		0x12   //用户
#define ISL1208_USER_DAT1		0x13
void WriteREG(uint8_t adds,uint8_t datas);
uint8_t ReadREG(uint8_t adds);
void ISL128Init(void);

//void SetTime(pMY_TIME _mytime);
//void GetTime(pMY_TIME _mytime);
//void Auto_Time_Set(void);

void read_time(void );
void Auto_Time_Set(uint8_t *buf);
void write_time(void);
void SetAlarm(void);
static uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day);
#endif
///****************************/
#if ds1337
#define DS1337_ADDR       0XD0

/*Timekeeper Registers*/

#define SECONDS       0X00
#define MINUTES       0X01
#define HOUR          0X02
#define DAY           0X03  /*1 - 7*/
#define DATE          0X04  /*1 - 31*/
#define MONTH         0X05
#define YEAR 					0X06
//#define A1SECONDS     0X07
//#define A1MINUTES     0X08
//#define A1HOURS       0X09
//#define A1DAY         0X0A
#define A2MINUTES     0X0B
#define A2HOURS       0X0C
#define A2DAY         0X0D
#define CONTROL       0X0E
#define STATUS        0X0F

///* Timerkeeper Params */
//#define HOUR12        0X40
//#define HOUR24        0X00
//#define AM            0X00
//#define PM            0X20

///*Control Params*/
//#define A1IE          0X01
//#define A2IE          0X02
//#define INTEN         0X04
//#define RS1           0X08
//#define RS2           0X10

///*Control Params*/
//#define A1F           0X01
//#define A2F           0X02
//#define OSF           0X80

///* Alarmsa Params */
//#define DYDT          0X40

//#define A1M1          0X80
//#define A1M2          0X80
//#define A1M3          0X80
//#define A1M4          0X80
//#define A2M2          0X80
//#define A2M3          0X80
//#define A2M4          0X80
#endif
///****************************/

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
extern MY_TIME sMyTime;
void IIC3_init(void);
void IIC3_Start(void);
void IIC3_Stop(void);
void IIC3_Send_Byte(uint8_t txd);
uint8_t IIC3_Read_Byte(unsigned char ack);
uint8_t IIC3_Wait_Ack(void);
void IIC3_Ack(void);
void IIC3_NAck(void);

static int IntToBCD(int i);     //十进制转BCD
static int BCDToInt(int bcd);   //BCD转十进制
uint8_t ds1337_write(uint8_t addr, uint8_t reg_addr, uint8_t *pdata, uint16_t size);
void ds1337_read(uint8_t addr, uint8_t regaddr,uint8_t *reg_value, uint16_t size);
uint8_t ds1337_settime(rtc_t *buf);
uint8_t ds1337_gettime(MY_TIME *DateTime);
uint8_t setalarm(void);
void UserCallback( void );
#endif
