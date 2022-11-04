#include "main.h"
#include "fm33lc0xx_flash.h"


void device_flash_read(uint32_t read_addr, uint32_t *read_buf, uint32_t read_size)
{
	uint32_t read_pos = 0;
	while(read_pos < read_size)
	{
		read_buf[read_pos] = *(volatile uint32_t *)read_addr;
		read_pos++;
		read_addr += 4;
	}
}

void device_flash_write(uint32_t write_addr, uint32_t *write_buf, uint32_t write_size)
{
 //	uint32_t addrx = 0;
	uint32_t endaddr = 0;

//	if(write_addr < STM32_FLASH_BASE || write_addr % 4)
//		return ;

	endaddr = write_addr + write_size * 4;

	//taskENTER_CRITICAL();

	LL_FLASH_PageErase(FLASH, write_addr);
//	LL_FLASH_SectorErase(FLASH, write_addr);

	while(write_addr < endaddr)
	{
		if(LL_FLASH_Program_Word(FLASH, write_addr, *write_buf) != PASS)
		{
			break;
		}
		write_addr += 4;
		write_buf ++;
	}
	//taskEXIT_CRITICAL();
}

