#ifndef __SPI_H__
#define __SPI_H__

#include "main.h"

void SPI1Init(void);
void SPI1deInit(void);
uint32_t SPI1WriteAndRead(uint32_t data);
void SPI1WriteData(uint8_t data);
uint8_t Spi1ReadData(uint8_t data);
void SPI1Writebuf(uint8_t *data, uint16_t length);
void SPI1Readbuf(uint8_t *data, uint16_t length);

#endif
