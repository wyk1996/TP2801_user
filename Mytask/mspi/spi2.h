#ifndef __spi2_H
#define __spi2_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* 定义SPI总线的 GPIO端口 */
//  PB12   ------> SPI2_NSS
//  PB13   ------> SPI2_SCK
//  PB14   ------> SPI2_MISO
//  PB15   ------> SPI2_MOSI 
#define RCC_CS 	  LL_IOP_GRP1_PERIPH_GPIOB
#define PORT_CS	  GPIOB
#define PIN_CS		LL_GPIO_PIN_12    //10

#define RCC_SCK 	LL_IOP_GRP1_PERIPH_GPIOB
#define PORT_SCK	GPIOB
#define PIN_SCK		LL_GPIO_PIN_13			//1

#define RCC_MISO 	LL_IOP_GRP1_PERIPH_GPIOB
#define PORT_MISO	GPIOB
#define PIN_MISO	LL_GPIO_PIN_14     //2

#define RCC_MOSI 	LL_IOP_GRP1_PERIPH_GPIOB
#define PORT_MOSI	GPIOB
#define PIN_MOSI	LL_GPIO_PIN_15       //0

	#define CS_0()		PORT_CS->BRR = PIN_CS
	#define CS_1()		PORT_CS->BSRR = PIN_CS

	#define SCK_0()		PORT_SCK->BRR = PIN_SCK
	#define SCK_1()		PORT_SCK->BSRR = PIN_SCK

	#define MOSI_0()	PORT_MOSI->BRR = PIN_MOSI
	#define MOSI_1()	PORT_MOSI->BSRR = PIN_MOSI

	#define MISO_IS_HIGH()	(LL_GPIO_IsInputPinSet(PORT_MISO, PIN_MISO) == 1)/////
	
//	uint8_t g_spi_busy = 0;		/* SPI 总线共享标志 */
	
void bsp_InitSPIBus(void);

void bsp_spiWrite0(uint8_t _ucByte,uint8_t cnt);

uint8_t bsp_spiRead0(void);

void bsp_spiWrite2(uint8_t _ucByte,uint8_t cnt) ;

void bsp_spiWrite1(uint8_t _ucByte,uint8_t cune);
uint8_t bsp_spiRead1(void);


void bsp_spiDelay(void);
void bsp_SPI_Init(uint16_t _cr1);

void bsp_SpiBusEnter(void);
void bsp_SpiBusExit(void);
uint8_t bsp_SpiBusBusy(void);
void bsp_SetSpiSck(uint8_t _data);


#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */
