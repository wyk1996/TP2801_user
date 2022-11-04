#ifndef _LCDdisplay_H_
#define _LCDdisplay_H_

#include "main.h"

// TM162x 指令
#define  CMD_SYS_DIS  0x00  // SYS DIS    (0000-0000-X) 关闭系统振荡器
#define  CMD_SYS_EN   0x80  // SYS EN     (0000-0001-X) 打开系统振荡器
#define  CMD_LCD_OFF  0x40  // LCD OFF    (0000-0010-X) 关闭LCD显示屏
#define  CMD_LCD_ON   0xC0  // LCD ON     (0000-0011-X) 打开LCD显示屏
#define  CMD_RC_INT   0x10  // RC INT     (0001-10XX-X) 系统时钟源,片上RC振荡器

//显示页面选择
#define  DIS_MAIN        0x00     //LCD显示主显示实时数据页面
#define  DIS_SET         0x01     //LCD显示设置页面
//实时数据显示部分 DIS_MAIN
#define  DIS_SENSOR1     0x00     //LCD显示传感器1实时数据页
#define  DIS_SENSOR2     0X01     //LCD显示传感器2实时数据页
#define  DIS_SENSOR3     0X02     //LCD显示传感器3实时数据页
//显示数据设置部分 DIS_SET
#define  DIS_SET_KEY     0X00     //参数设置密码输入界面
#define  DIS_S1_D1_UP       0X01     //SENSOR1 D1上限设置界面
#define  DIS_S1_D1_DOWN     0X02     //SENSOR1 D1下限设置界面
#define  DIS_S1_D2_UP       0X03     //SENSOR1 D1上限设置界面
#define  DIS_S1_D2_DOWN     0X04     //SENSOR1 D1下限设置界面
#define  DIS_S2_D1_UP       0X05     //SENSOR1 D1上限设置界面
#define  DIS_S2_D1_DOWN     0X06     //SENSOR1 D1下限设置界面
#define  DIS_S2_D2_UP       0X07     //SENSOR1 D1上限设置界面
#define  DIS_S2_D2_DOWN     0X08     //SENSOR1 D1下限设置界面
#define  DIS_S3_D1_UP       0X09     //SENSOR1 D1上限设置界面
#define  DIS_S3_D1_DOWN     0X0A     //SENSOR1 D1下限设置界面
#define  DIS_S3_D2_UP       0X0B     //SENSOR1 D1上限设置界面
#define  DIS_S3_D2_DOWN     0X0C     //SENSOR1 D1下限设置界面
#define  DIS_ADDR           0x0D     //设备ID设置界面

extern xTaskHandle device_LCDHandle;

void LCD_CLOSE(void);
void LCD_config_create_task(void *params, uint8_t priority);
void wait_time(void);

#endif
