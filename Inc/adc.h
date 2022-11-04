/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : adc.h
  * @brief          : Header for adc.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 FMSH.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by FM under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C" {
#endif


#define ADC_VREF    (*((uint16_t *)(0x1FFFFB08)))   // 30℃ vref1.22采样值
#define ADC_VREF1    (*((uint16_t *)(0x1FFFFB0c)))   // 30℃ vref1.22采样值
	
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"


#if defined(USE_FULL_ASSERT)
#include "fm33_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void AdcInit_P(void);
void AdcInit_sensor(void);   //ADC采样传感器数据
//uint32_t adc_get_voltage_sample(uint32_t channal);
void get_adc_vbat_value(void);
uint8_t GET_PT100_data(void);
void PT100_K_D_get(void);
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */

/************************ (C) COPYRIGHT FMSH *****END OF FILE****/
