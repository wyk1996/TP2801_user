#ifndef _MYIIC_H
#define _MYIIC_H

#include "main.h"

  


void ADS1115_IIC1_init(void);
void ADS1115_IIC1_deinit(void);
void IIC1_Start(void);
void IIC1_Stop(void);
void IIC1_Send_Byte(uint8_t txd);
uint8_t IIC1_Read_Byte(unsigned char ack);
uint8_t IIC1_Wait_Ack(void);
void IIC1_Ack(void);
void IIC1_NAck(void);
void IIC1_delay(uint8_t us);

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	 

#endif

