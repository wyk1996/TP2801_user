
#ifndef __JHM1200_IIC_H_
#define __JHM1200_IIC_H_
#include "main.h"

//void SENSOR_IIC2_init(void);
//void SENSOR_IIC2_deinit(void);
//void SENSOR_IIC4_init(void);
//void SENSOR_IIC4_deinit(void);

//uint8_t JHM1200_get_cal(void);
//typedef struct
//{
//	float  sensor2data;
//	float  sensor1data;
////	float  sensor3data;
//	float  sensor2data_f;
//	float  sensor1data_f;
//	uint16_t  Dsensor2;
//	uint16_t  Dsensor1;   
//	unsigned char red45buf[4];   ///��λ����λ 3 2 1 0
//	int32_t  sensor1_u32data;
//	uint32_t  sensor2_u32data;
////	uint16_t ucReceiveData[4];
//}MPA;
//extern MPA MPAbuf;

//uint8_t read45x6DO(uint8_t DF_Command);
//uint8_t read45x6DO_30M(void);
//uint8_t read45x6MR(void);
//uint8_t Intored45buf(void);
//uint8_t Intored45bufIIC4(void);   /////////////////ѹ����ϵ�� IIC4�豸 �ڶ���
//uint8_t Intored45buf_30M(void);
//uint8_t Intored45buf_new(void);
//uint8_t Intored45buf_D036(void);   /////////////////�°汾ҺλD036
//uint8_t Intored45buf_D033(void);
//uint8_t Intored45bufIIC4_D033(void);
//uint8_t Intored45buf_CQ__D020_40MPa(void);

void delay_10pus(uint16_t us);   //8MHZ��   10+us��ʱus
	
//float Q_rsqrt( float number );
float Pt100_RToT(float rRtd);
//float Bytes_ToFloat(float Data, uint8_t *Bytes, uint8_t index) ;
//uint8_t  DQ_tp100(void);
//uint8_t monobus_sensor(void);
//uint8_t monobus_sensor1(void);
//void uint_to_unit(void);//��׼ΪM	
//void danzong_init(void);
//void kaiguan_io_init(void);
//uint8_t kaiguan_read(void);
void port_data_get(void);
uint8_t fangtiao(uint8_t ch);
void alarm_check(uint8_t ch);     //���ݳ����ж�
#endif
