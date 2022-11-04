/*
 * @Author: your name
 * @Date: 2020-08-07 11:38:06
 * @LastEditTime: 2020-08-24 10:23:39
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edi
 * @FilePath: \FM32_NB温湿度\BSP\Inc\w25q32.h
 */
#ifndef _W25QXX_H_
#define _W25QXX_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* 函数原型 ----------------------------------------------------------*/
/* 宏  ---------------------------------------------------------------*/
/*寄存器表*/
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 
/* 变量 --------------------------------------------------------------*/
/* 类型定义 ----------------------------------------------------------*/
/* 函数声明 ----------------------------------------------------------*/

void w25qxx_erase_sector(uint32_t dst_addr);
void w25qxx_power_down(void);
void w25qxx_power_wakeup(void);
void w25qxx_read(uint8_t *read_buf, uint32_t read_addr, uint16_t read_size);
void w25qxx_write(uint8_t *wr_buf, uint32_t wr_addr, uint16_t wr_size);
void w25qxx_init(void);
void w25qxx_enter_sleep_mode(void);
void w25qxx_exti_sleep_mode(void);
void w25qxx_erase_chip(void);
#endif
