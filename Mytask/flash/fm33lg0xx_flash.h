#ifndef F030_FLASH_H_
#define F030_FLASH_H_
#include "main.h"
#define STM32_FLASH_SIZE   128
#define STM32_FLASH_BASE   0x8000000
#define STM32_SECTOR_SIZE  2048


void device_flash_write(uint32_t write_addr, uint32_t *write_buf, uint32_t write_size);
void device_flash_read(uint32_t read_addr, uint32_t *read_buf, uint32_t read_size);

#endif 

