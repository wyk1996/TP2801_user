#ifndef _HIST_H_
#define _HIST_H_

#include "main.h"

#define FLASH_SECTOR_SIZE   4096    //扇区大小
#define FLASH_SECTOR_NUM    1024   ///W25Q16扇区数是512

#define  W_FLASH   1 << 0
#define  R_FLASH   1 << 1
#define  C_FLASH   1 << 2
#define  WL_FLASH  1 << 3
//#define  W_FLASH   2
//#define  R_FLASH   3
//#define  C_FLASH   4
//#define  WL_FLASH  5



//#define SAVE_LAST_SAVE_ADDR  0x0000  //保存 4个字节
//#define SAVE_FIRST_SAVE_ADDR 0x0004  //保存 4个字节
//#define  SAVE_SEG_NUM_ADDR   0x0008 //



typedef struct
{
	uint32_t prev;
	
	float data[16];
	
	rtc_t data_time;
	
	uint32_t next;
	
	uint32_t crc;
	
}hist_t;    /* 历史数据结构体 仿双向指针 */

extern hist_t hist_data;

//void device_set_hist_num_addr(uint32_t saddr, uint32_t addr,uint32_t segnum);
//uint32_t device_get_hist_num_addr(void);
uint32_t device_get_hist_setaddr(void);
uint32_t hist_init(void);

uint32_t hist_get_current_addr(void);

uint8_t hist_save(uint32_t *addr);

uint8_t hist_check(uint32_t addr);

void hist_read(hist_t *read_hist, uint32_t addr);

void hist_read_date(uint8_t s_year, uint8_t s_mon, uint8_t s_day, uint8_t e_year, uint8_t e_mon, uint8_t e_day);

uint32_t hist_date_addr_get(uint8_t year, uint8_t mon, uint8_t day);


#endif /*_HIST_H_*/
