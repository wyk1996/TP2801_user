/**
  ******************************************************************************
  * File Name          : SPI.c
  * Description        : This file provides code for the configuration
  *                      of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "spi.h"


void SPI1Init(void)
{
	LL_GPIO_InitTypeDef gpioInitStruct = {0};
    LL_SPI_InitTypeDef spiInitStruct = {0};
    
    spiInitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
    spiInitStruct.BitOrder = LL_SPI_BIT_ORDER_MSB_FIRST;
    spiInitStruct.ClockPhase = LL_SPI_SPI_PHASE_1EDGE;
    spiInitStruct.ClockPolarity = LL_SPI_SPI_POLARITY_LOW;
    spiInitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
    spiInitStruct.Mode = LL_SPI_WORK_MODE_MASTER;
    spiInitStruct.SSN = LL_SPI_SSN_MODE_SOFTWARE;
    spiInitStruct.TransferMode = LL_SPI_MODE_FULL_DUPLEX;
    LL_SPI_Init(SPI1, &spiInitStruct);
    
    LL_SPI_TxBuffClear(SPI1);
    LL_SPI_RxBuffClear(SPI1);
    
    gpioInitStruct.Pin = LL_GPIO_PIN_8 | LL_GPIO_PIN_9 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11;   //LL_GPIO_PIN_8 | LL_GPIO_PIN_9 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11;
        
    gpioInitStruct.Mode = LL_GPIO_MODE_DIGITAL;
    gpioInitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpioInitStruct.Pull = ENABLE;
    gpioInitStruct.RemapPin = DISABLE;
    
    LL_GPIO_Init(GPIOB, &gpioInitStruct);
    #if 0
    gpioInitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    gpioInitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;	
    gpioInitStruct.Pin = LL_GPIO_PIN_8;
    LL_GPIO_Init(GPIOB, &gpioInitStruct);
    #endif
}

void SPI1deInit(void)
{
	LL_GPIO_InitTypeDef gpioInitStruct = {0};
	LL_SPI_DeInit(SPI1);
    gpioInitStruct.Pin = LL_GPIO_PIN_8 | LL_GPIO_PIN_9 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11;
    gpioInitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    gpioInitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    gpioInitStruct.Pull = DISABLE;
    gpioInitStruct.RemapPin = DISABLE;
    LL_GPIO_Init(GPIOB, &gpioInitStruct);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11);
}
/*******************************************************************/
uint32_t SPI1WriteAndRead(uint32_t data)
{   
    LL_SPI_WriteTxBuff(SPI1, data);
    
    #if 0
	uint16_t wait_num = 0;
    #endif
    
    while (!(LL_SPI_IsActiveFlag_TxBufffEmpty(SPI1)));
        #if 0
		{
			if(wait_num++ >= 30)
			{
				SPI1Init();
				break;
			}
			Trace_log("µÈ´ýSPI\r\n");
			vTaskDelay(1000);
		}
		wait_num = 0;
        #endif
    
    while (!(LL_SPI_IsActiveFlag_RxBufffFull(SPI1)));
        #if 0
		{
			if(wait_num++ >= 30)
			{
				SPI1Init();
				break;
			}
			Trace_log("µÈ´ýSPI1\r\n");
			vTaskDelay(1000);
		}
        #endif
    data = LL_SPI_ReadRxBuff(SPI1);    
    return data;
}

void SPI1WriteData(uint8_t data)
{
    LL_SPI_WriteTxBuff(SPI1, data); 
    while (!(LL_SPI_IsActiveFlag_TxBufffEmpty(SPI1)));
    while (!(LL_SPI_IsActiveFlag_RxBufffFull(SPI1)));
}

uint8_t SPI1ReadData(uint8_t data)
{
	uint8_t spi_data;
	LL_SPI_WriteTxBuff(SPI1, data);
	while (!(LL_SPI_IsActiveFlag_RxBufffFull(SPI1)));
	spi_data = LL_SPI_ReadRxBuff(SPI1);
	return spi_data;
}

void SPI1Writebuf(uint8_t *data, uint16_t length)
{
    while (length--)
    {
        LL_SPI_WriteTxBuff(SPI1, *data); data++;
        while (!(LL_SPI_IsActiveFlag_TxBufffEmpty(SPI1)));
    }
    while (!(LL_SPI_IsActiveFlag_RxBufffFull(SPI1)));
}

void SPI1Readbuf(uint8_t *data, uint16_t length)
{
    while (length--)
    {
        LL_SPI_WriteTxBuff(SPI1, 0x55);
        while (!(LL_SPI_IsActiveFlag_RxBufffFull(SPI1)));
        *data = LL_SPI_ReadRxBuff(SPI1); data++;
    }
}


