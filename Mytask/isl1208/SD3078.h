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
/*�˽ṹ�嶨���˵���ʱ�жϿɹ����õ�Ƶ��Դ��IM�ͳ�ֵ��Ҫ����*/
typedef	struct
{
	enum clk_Souce d_clk;
	uint8_t   IM;	//IM=1:�������ж�
	uint32_t  init_val;
} CountDown_Def;

///*
// * ͨ���궨��ѡ��ʹ�õ�RTC�ͺ�
// * SD30XX_25XX -> ʹ��SD30��SD25ϵ��
// * SD20XX_24XX -> ʹ��SD20��SD24ϵ��
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
#define		Chg_MG          0x18		//������Ĵ�����ַ
#define		Bat_H8          0x1A		//�������λ�Ĵ�����ַ
#define		Bat_L8          0x1B		//�����Ͱ�λ�Ĵ�����ַ


/*********************Users RAM    2CH----71H  ******************************/
#define   user_ram1       0x2c     /***4 byte ***/   //��ص����洢
#define   user_ram2       0x30    //�ϵ�������ַ
#define   user_ram3       0x34    //�ϵ���������
#define   user_ram4       0x38    //���Ź���λ��־
#define   user_ram5       0x3c   //
#define   user_ram6       0x40   //
#define   user_ram7       0x44   //���������洢
#define   user_ram8       0x48   //��ʷ��¼�洢��ַ

/*********************** ID Register **********************/
#define		ID_Address			0x72		//ID����ʼ��ַ

/********************** �����жϺ궨�� *********************/
#define		sec_ALM					0x01
#define		min_ALM					0x02
#define		hor_ALM					0x04
#define		wek_ALM					0x08
#define		day_ALM					0x10
#define		mon_ALM					0x20
#define		yar_ALM					0x40

/********************** �ж�ʹ�ܺ궨�� **********************/
#define		INTDE						0x04		//����ʱ�ж�
#define		INTAE						0x02		//�����ж�
#define		INTFE						0x01		//Ƶ���ж�

/********************** �ж���ʾ�궨�� **********************/
//#define 	FREQUENCY				0				//Ƶ���ж�
//#define 	ALARM						1				//�����ж�
//#define 	COUNTDOWN				2				//����ʱ�ж�
//#define 	DISABLEINT					3				//��ֹ�ж�

/*************** �ж��������ѡ��������ѡ�� ****************/
#define 	INT_TYPE			COUNTDOWN //�����ж��������

/***********��дʱ�亯��*************/

extern MY_TIME sMyTime;
uint8_t RTC_WriteDate(rtc_t	*psRTC);
uint8_t RTC_ReadDate(rtc_t	*psRTC);

/*******I2C���ֽ�������д����********/
uint8_t I2CWriteSerial(uint8_t DeviceAddress,uint8_t Address,uint8_t length,uint8_t *ps);
uint8_t I2CReadSerial(uint8_t DeviceAddress,uint8_t Address,uint8_t length,uint8_t *ps);
/*********I2C�˿ڳ�ʼ������*********/
void SD3078_IIC3_init(void );
void SD3078_IIC3_deinit(void);

int IntToBCD(int i); //ʮ����תBCD
int BCDToInt(int bcd); //BCDתʮ����
/*********RTC�ж����ú���*********/
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

