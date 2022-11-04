#ifndef __GPIO_H__
#define __GPIO_H__

#include "fm33lc0xx_ll_gpio.h"

/*����IO�궨��*/

//pwr3v ���蹩�����IO   ���
#define pwr3v_gpio       GPIOD
#define pwr3v_gpio_id    LL_GPIO_PIN_11
//���ػ�����IO     ���
#define POWER_gpio       GPIOD
#define POWER_gpio_id        LL_GPIO_PIN_4
//�ⲿ���ѺͿ��ػ�����   ����
#define key_gpio       GPIOD
#define key_gpio_id    LL_GPIO_PIN_5 
//PCIEģ�鹩���������   ���
#define pwrpcie_gpio       GPIOD
#define pwrpcie_gpio_id    LL_GPIO_PIN_6
//RS485��������  ���
#define rs485de_gpio       GPIOA
#define rs485de_gpio_id    LL_GPIO_PIN_15
//VDD_EXTģ���Դ����������   ����     loraģ���Ӧģ�黽������
#define vddext_gpio       GPIOC
#define vddext_gpio_id    LL_GPIO_PIN_0
//PWR_KEY       ģ�鸴λ����
#define pwrio_gpio       GPIOC
#define pwrio_gpio_id    LL_GPIO_PIN_1
//EXIT_WAKE     //ģ�����߻������� NB/4G    loraģ���Ӧģ����������豸����
#define psmio_gpio       GPIOC
#define psmio_gpio_id    LL_GPIO_PIN_6
//almio  �̵��������������   ���
#define almio_gpio       GPIOC
#define almio_gpio_id    LL_GPIO_PIN_9
//usb_ext USB��Դ���IO    ����
#define usbpower_gpio       GPIOC
#define usbpower_gpio_id    LL_GPIO_PIN_11
//led  �����������   ���
#define LED_gpio       GPIOC
#define LED_gpio_id    LL_GPIO_PIN_12


//ADC_IN1 �ⲿ�����Դ���IO    ģ��������
#define ADC_IN1_gpioc       GPIOC
#define ADC_IN1_gpioc_id    LL_GPIO_PIN_10
//ADC_IN8 ��ص�ѹ���IO    ģ��������
#define ADC_IN8_gpioc       GPIOC
#define ADC_IN8_gpioc_id    LL_GPIO_PIN_7

//PT100�����������ӿ�
#define ADC_IN_PT100_port   GPIOD
#define ADC_IN3_PD0_3       LL_GPIO_PIN_0
#define ADC_IN4_PD1_2       LL_GPIO_PIN_1
#define ADC_IN5_PD2_1       LL_GPIO_PIN_2
//3.3v������
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
