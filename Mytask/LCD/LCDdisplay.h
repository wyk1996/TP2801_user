#ifndef _LCDdisplay_H_
#define _LCDdisplay_H_

#include "main.h"

// TM162x ָ��
#define  CMD_SYS_DIS  0x00  // SYS DIS    (0000-0000-X) �ر�ϵͳ����
#define  CMD_SYS_EN   0x80  // SYS EN     (0000-0001-X) ��ϵͳ����
#define  CMD_LCD_OFF  0x40  // LCD OFF    (0000-0010-X) �ر�LCD��ʾ��
#define  CMD_LCD_ON   0xC0  // LCD ON     (0000-0011-X) ��LCD��ʾ��
#define  CMD_RC_INT   0x10  // RC INT     (0001-10XX-X) ϵͳʱ��Դ,Ƭ��RC����

//��ʾҳ��ѡ��
#define  DIS_MAIN        0x00     //LCD��ʾ����ʾʵʱ����ҳ��
#define  DIS_SET         0x01     //LCD��ʾ����ҳ��
//ʵʱ������ʾ���� DIS_MAIN
#define  DIS_SENSOR1     0x00     //LCD��ʾ������1ʵʱ����ҳ
#define  DIS_SENSOR2     0X01     //LCD��ʾ������2ʵʱ����ҳ
#define  DIS_SENSOR3     0X02     //LCD��ʾ������3ʵʱ����ҳ
//��ʾ�������ò��� DIS_SET
#define  DIS_SET_KEY     0X00     //�������������������
#define  DIS_S1_D1_UP       0X01     //SENSOR1 D1�������ý���
#define  DIS_S1_D1_DOWN     0X02     //SENSOR1 D1�������ý���
#define  DIS_S1_D2_UP       0X03     //SENSOR1 D1�������ý���
#define  DIS_S1_D2_DOWN     0X04     //SENSOR1 D1�������ý���
#define  DIS_S2_D1_UP       0X05     //SENSOR1 D1�������ý���
#define  DIS_S2_D1_DOWN     0X06     //SENSOR1 D1�������ý���
#define  DIS_S2_D2_UP       0X07     //SENSOR1 D1�������ý���
#define  DIS_S2_D2_DOWN     0X08     //SENSOR1 D1�������ý���
#define  DIS_S3_D1_UP       0X09     //SENSOR1 D1�������ý���
#define  DIS_S3_D1_DOWN     0X0A     //SENSOR1 D1�������ý���
#define  DIS_S3_D2_UP       0X0B     //SENSOR1 D1�������ý���
#define  DIS_S3_D2_DOWN     0X0C     //SENSOR1 D1�������ý���
#define  DIS_ADDR           0x0D     //�豸ID���ý���

extern xTaskHandle device_LCDHandle;

void LCD_CLOSE(void);
void LCD_config_create_task(void *params, uint8_t priority);
void wait_time(void);

#endif
