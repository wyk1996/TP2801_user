#ifndef _ht1623_H
#define _ht1623_H

#include "main.h"
//#include "bihua.h"

//uint8_t Dis_abc[64]; 
//uint8_t  Num_128[32];

#define BIAS   0x24 // 0x29

#define SYSDIS   0x00       //�ر�������LCD bias
#define SYSEN   0x01        //��������
#define LCDOFF  0x02        //�ر� LCD bias
#define LCDON   0x03     		//����
#define TIMERDIS  0x04        //��ʹtime bass ���
#define WDTDIS  0X05
#define IRQDIS  0X80
#define LCD_F16   0XA4
#define LCD_F32   0XA5
#define LCD_F64   0XA6
#define LCD_F128   0XA7       //Ĭ��

#define LCD_run_T    (1 << 0)    //�����¶�
#define LCD_run_F    (1 << 1)    //����ѹ��
#define ERR_1      (1 << 2)    //���������� 
#define ERR_2      (1 << 3)    //SIM������
#define ERR_3      (1 << 4)    //SIM����ע��ʧ��
#define ERR_4      (1 << 5)    //����������ʧ��
#define ERR_5      (1 << 6)    //�����ϱ�ʧ��
#define ERR_6      (1 << 7)    //�洢ʧ��

//void ht1623_io(void );

void SendBit_1621(uint8_t data,uint8_t cnt) ;
void SendDataBit_1621(uint8_t data,uint8_t cnt) ;
void Write_1621(uint8_t addr,uint8_t data) ;
void WriteAll_1621(uint8_t addr,uint8_t *p,uint8_t cnt) ;
void SendCmd(uint8_t command) ;

void HT1621_Init(void );
void LCDD_deint(void);
void WriteAll_HT1623(uint8_t *p,uint8_t cnt) ;
void writer_all_man(void);
void init_clear_Num(uint8_t *p,uint8_t cnt);

uint8_t Retrieve(uint8_t dat,uint8_t *p,uint8_t cnt);//����оͻ�1��

void Dis_All(char Dis_unit,float Dis_data ,uint8_t *time ,uint8_t Dis_signal ,uint16_t vbet1 ,uint8_t Dis_dip);
void oled_callback(void);
void Dis_err(uint8_t *timer ,uint8_t Dis_signal,uint16_t Dis_vbat,uint16_t Dis_error_flag);
uint8_t display_num_value(float num_value ,uint8_t *buf_num,uint8_t dip);
	

#endif
