/*
 * @Author: your name
 * @Date: 2020-08-10 14:43:31
 * @LastEditTime: 2020-08-10 14:56:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \FM32_NB��ʪ��\BSP\Inc\flash.h
 */
#ifndef _FLASH_H_
#define _FLASH_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* ����ԭ�� ----------------------------------------------------------*/
/* ��  ---------------------------------------------------------------*/
/* ���� --------------------------------------------------------------*/
/* ���Ͷ��� ----------------------------------------------------------*/
/* �������� ----------------------------------------------------------*/

void flash_write(uint32_t write_addr, uint32_t *write_buf, uint32_t write_size);

void flash_read(uint32_t read_addr, uint32_t *read_buf, uint32_t read_size);
uint32_t flash_read_word(uint32_t faddr);
#endif
