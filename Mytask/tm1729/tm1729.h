#ifndef _tm1729_H
#define _tm1729_H

#include "main.h"
//#include "bihua.h"

//uint8_t Dis_abc[64]; 
//uint8_t  Num_128[32];

#define TM1729_ADDR         0x3E
#define TM1729_DDRAM_ADDR   0x00

//显示控制指令
#define TM1729_DISCTL       0xA0
//工作模式
#define TM1729_NOMAL_MODE   0x00
#define TM1729_SAVE_1_MODE  0x08
#define TM1729_SAVE_2_MODE  0x10
#define TM1729_SAVE_3_MODE  0x18
//液晶驱动波形
#define TM1729_LINE_FLIP    0x00     //Line翻转
#define TM1729_FRAME_FLIP   0x04     //Frame翻转
//power save mode SR设定
#define TM1729_POWER_SAVE_1 0x00    
#define TM1729_POWER_SAVE_2 0x01
#define TM1729_NORMAL       0x02
#define TM1729_HIGH_POWER   0x03

//模式设置
#define TM1729_MODESET      0xC0
#define TM1729_0FF_MODE     0x00
#define TM1729_1_3_BIAS     0x08
#define TM1729_1_2_BIAS     0x0C

//芯片设置
#define TM1729_ICSET        0xE8
#define TM1729_ICSET_RESET  0x02

//闪烁控制
#define TM1729_BLKCTL       0xF0
#define TM1729_BLKCTL_OFF   0x00
#define TM1729_BLKCTL_0_5   0x01      //0.5Hz
#define TM1729_BLKCTL_1_0   0x02      //1Hz
#define TM1729_BLKCTL_2_0   0x03      //2Hz

//全屏控制
#define TM1729_APCTL        0xFC
#define TM1729_APCTL_ON     0x02      //全屏点亮
#define TM1729_APCTL_OFF    0x01      //全屏关闭

#define TM1729_ADSET        0x80      //显示地址设置

#define LCD_run_T    (1 << 0)    //正常温度
#define LCD_run_F    (1 << 1)    //正常压力
#define ERR_1      (1 << 2)    //传感器错误 
#define ERR_2      (1 << 3)    //SIM卡错误
#define ERR_3      (1 << 4)    //SIM入网注册失败
#define ERR_4      (1 << 5)    //服务器连接失败
#define ERR_5      (1 << 6)    //数据上报失败
#define ERR_6      (1 << 7)    //存储失败


//void ht1623_io(void );


void TM1792_IIC6_INIT(void);
void TM1792_IIC6_DEINIT(void);
uint8_t TM1729_Init(void );
void LCD_CLOSE(void);
void LCD_OPEN_ALL(void);
void LCD_OFF_ALL(void);
void TM1729_display_init(void);
void LCD_TEST(void);

void Dis_All(char Dis_unit,float Dis_data ,uint8_t *time ,uint8_t Dis_signal ,float vbet1 ,uint8_t Dis_dip);
void Dis_err(uint8_t *timer ,uint8_t Dis_signal,float Dis_vbat,uint16_t Dis_error_flag);

	

#endif
