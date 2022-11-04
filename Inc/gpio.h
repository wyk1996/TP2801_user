#ifndef __GPIO_H__
#define __GPIO_H__

#include "fm33lc0xx_ll_gpio.h"

/*功能IO宏定义*/

//pwr3v 外设供电控制IO   输出
#define pwr3v_gpio       GPIOD
#define pwr3v_gpio_id    LL_GPIO_PIN_11
//开关机控制IO     输出
#define POWER_gpio       GPIOD
#define POWER_gpio_id        LL_GPIO_PIN_4
//外部唤醒和开关机按键   输入
#define key_gpio       GPIOD
#define key_gpio_id    LL_GPIO_PIN_5 
//PCIE模块供电控制引脚   输出
#define pwrpcie_gpio       GPIOD
#define pwrpcie_gpio_id    LL_GPIO_PIN_6
//RS485控制引脚  输出
#define rs485de_gpio       GPIOA
#define rs485de_gpio_id    LL_GPIO_PIN_15
//VDD_EXT模块电源输出检测引脚   输入     lora模块对应模块唤醒引脚
#define vddext_gpio       GPIOC
#define vddext_gpio_id    LL_GPIO_PIN_0
//PWR_KEY       模块复位引脚
#define pwrio_gpio       GPIOC
#define pwrio_gpio_id    LL_GPIO_PIN_1
//EXIT_WAKE     //模块休眠唤醒引脚 NB/4G    lora模块对应模块输出唤醒设备引脚
#define psmio_gpio       GPIOC
#define psmio_gpio_id    LL_GPIO_PIN_6
//almio  继电器输出控制引脚   输出
#define almio_gpio       GPIOC
#define almio_gpio_id    LL_GPIO_PIN_9
//usb_ext USB电源检测IO    输入
#define usbpower_gpio       GPIOC
#define usbpower_gpio_id    LL_GPIO_PIN_11
//led  输出控制引脚   输出
#define LED_gpio       GPIOC
#define LED_gpio_id    LL_GPIO_PIN_12


//ADC_IN1 外部供电电源检测IO    模拟量输入
#define ADC_IN1_gpioc       GPIOC
#define ADC_IN1_gpioc_id    LL_GPIO_PIN_10
//ADC_IN8 电池电压检测IO    模拟量输入
#define ADC_IN8_gpioc       GPIOC
#define ADC_IN8_gpioc_id    LL_GPIO_PIN_7

//PT100传感器采样接口
#define ADC_IN_PT100_port   GPIOD
#define ADC_IN3_PD0_3       LL_GPIO_PIN_0
#define ADC_IN4_PD1_2       LL_GPIO_PIN_1
#define ADC_IN5_PD2_1       LL_GPIO_PIN_2
//3.3v采样口
#define ADC_IN_3_3V_port    GPIOA
#define ADC_IN6_PA_13       LL_GPIO_PIN_13


void GPIO_Init(void);
void GPIO_deinit(void);
void GPIO_Init_pwrpcie(void);
void GPIO_deInit_pwrpcie(void);
void GPIO_Init_pwr3v(void);
void GPIO_deInit_pwr3v(void);
//void exinterup_init_key(void);
void exinterup_init(void);
void exinterup_deinit(void);
void GPIO_ToggleBits(GPIO_Type* GPIOx, uint32_t GPIO_Pin);
void iwdt_reload(void);
//void usb_int_fangdian(void);
#endif
