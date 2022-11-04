#include "main.h"
#include "hist.h"

extern device_t device_info;
extern uart_t uart2;


hist_t hist_data;

uint32_t device_get_hist_setaddr(void)
{
	uint8_t  buff[8];
	uint32_t j=512;
	uint16_t netAddr=0;
	while(j--)
	{
	
		  if(netAddr>0x1000)
			{
				break;
			}
		w25qxx_read(buff,netAddr,8); //查询方式。每次读8个。
		if(buff[0]==0xff &  buff[1]==0xff & buff[2]==0xff & buff[3]==0xff & buff[4]==0xff & buff[5]==0xff & buff[6]==0xff & buff[7]==0xff)
		{	
					break;
		}
		netAddr=netAddr+8;
	}		
	
	return  netAddr;
}

uint32_t hist_init(void)
{
	uint32_t addr = 10;

	//获取地址 如果没电可能会导致异常
	addr = device_get_flahaddr_addr();
	
	if(addr >= 0x400000)
	{
		addr = 0;
		device_info.hist_num = 0;
		device_info.save_addr = 0;
		device_info.pend_num = 0;	
		device_pending_num_set();
		device_perv_addr_set();
		device_set_flahaddr_addr(0);
	}
	
	if(hist_check(addr))  /*校验错误 */
	{
		hist_save(&addr);
	}
	else 
	{
		w25qxx_read((uint8_t *)&hist_data, addr, sizeof(hist_t));
		addr = hist_data.next;
	}

	return addr;
}
/*****************************
  * @brief  历史数据保存函数
  * @param  写入地址
  * @retval None
*****************************/

uint8_t hist_save(uint32_t *addr)
{
	uint8_t i;	
	if((*addr % FLASH_SECTOR_SIZE + sizeof(hist_t)) / sizeof(hist_t) == (FLASH_SECTOR_SIZE / sizeof(hist_t)))   /*判断是否扇区最后一个*/
	{
		Trace_log("FLASH写入扇区变化\r\n");
		if((*addr / FLASH_SECTOR_SIZE + 1)!= FLASH_SECTOR_NUM)
		{
			hist_data.next = (*addr / FLASH_SECTOR_SIZE + 1) * FLASH_SECTOR_SIZE;/*next_addr 指向下一个扇区起始地址*/
		}
		else
		{
			hist_data.next = 0;
		}
	}
	else
	{
		hist_data.next = *addr + sizeof(hist_t);
	}
	
	if((*addr % 4096) == 0)
	{
		Trace_log("擦除设备\r\n");
		/*扇区首地址*/
		w25qxx_erase_sector(*addr);
	}
	
	
	hist_data.prev = device_get_flahaddr_addr();
	
	hist_data.data_time.week  = device_info.data_time.week;
	hist_data.data_time.day   = device_info.data_time.day;
	hist_data.data_time.hour  = device_info.data_time.hour;
	hist_data.data_time.min   = device_info.data_time.min;
	hist_data.data_time.month = device_info.data_time.month;
	hist_data.data_time.sec   = device_info.data_time.sec;
	hist_data.data_time.year  = device_info.data_time.year;
	
	for(i=0;i<16;i++)
	{
		hist_data.data[i] = device_info.channel_data[i];
	}

	hist_data.crc = crc_16((uint8_t *)&hist_data, sizeof(hist_t) - 4);  /*求出crc*/	
  	Trace_log("写入信息\r\n");	
	w25qxx_write((uint8_t *)&hist_data, *addr, sizeof(hist_t));	
	w25qxx_read((uint8_t *)&hist_data, *addr, sizeof(hist_t));
	if(hist_data.data_time.year != device_info.data_time.year || hist_data.data_time.month != device_info.data_time.month)
	{
	  Trace_log("写入失败\r\n");
		return 1;
	}
	Trace_log("写入成功\r\n");	
	

  	device_set_flahaddr_addr(*addr);	
	*addr = hist_data.next;  /* 偏移到当前地址  */
 
	return 0;
}

/**
  * @brief  检测最后一包数据是否出错，存在掉电保护功能
  * @param  读取地址
  * @retval 1：掉电导致数据错误 0：数据完整
  */

uint8_t hist_check(uint32_t addr)
{
	uint16_t crc_d = 0;
	w25qxx_read((uint8_t *)&hist_data, addr, sizeof(hist_t));
	crc_d = crc_16((uint8_t *)&hist_data, sizeof(hist_t) - 4);

	if(hist_data.crc != crc_d)   /* 校验CRC是否正确 */
	{
			Trace_log("SAVE CRC ERR\r\n");
		return 1;
	}
	return 0;
}

/**
  * @brief  读取历史数据
  * @param  读取存放结构体
	* @param  读取地址
  * @retval None
  */
void hist_read(hist_t *read_hist, uint32_t addr)
{
	w25qxx_read((uint8_t *)read_hist, addr ,sizeof(hist_t));
}

/**
  * @brief  读取数据，按起始年月日和结束年月日来进行
  * @param  起始年月日-->结束年月日
  * @retval None
  */
uint32_t start_addr = 1;
void hist_read_date(uint8_t s_year, uint8_t s_mon, uint8_t s_day, uint8_t e_year, uint8_t e_mon, uint8_t e_day)
{
	
	uint32_t read_count = 0;
	hist_t min_hist;  			/*当前数组里面最小的*/
	uint32_t curren_day;
	
	uint32_t from_day = s_year * 365 + s_mon * 30 + s_day;
	uint32_t to_day = e_year * 365 + e_mon * 30 + e_day;
	
	uint8_t test = 0;
	
	char ptr_buf[254];
	
	while(s_year <= e_year)
	{
		start_addr = hist_date_addr_get(s_year, s_mon, s_day);
		if(start_addr != 1)
		{
			break;
		}
		else 
		{
			s_year += 1;s_mon = 1;s_day = 1;
		}
	}
	uart_transmit_str(&COM0,(uint8_t *)"记录时间        CH0     CH1    CH2    CH3    CH4    CH5    CH6    CH7    CH8    CH9    CH10    CH11    CH12    CH13    CH14    CH15\r\n");
	if(start_addr == 1)
	{
		return ;
	}
	delay_vtask(5);
	while(1) {
		w25qxx_read((uint8_t *)&min_hist, start_addr, sizeof(hist_t));
		
		if((++read_count >= (4096 / sizeof(hist_t) * 1024)) || (min_hist.next == 0XFFFFFFFF))
		{
			break;  /*读了一圈 马上退出*/
		}
		curren_day = min_hist.data_time.year * 365 + min_hist.data_time.month * 30 + min_hist.data_time.day;
		if(curren_day >= from_day && curren_day <= to_day)
		{						
			sprintf(ptr_buf, "%d-%02d-%02d %02d:%02d:%02d    %.1f   %.2f   %.2f   %.2f   %.4f   %.4f   %.4f   %.4f   %.4f   %.4f   %.4f   %.4f   %.4f   %.4f   %.4f   %.4f\n",
				min_hist.data_time.year,
				min_hist.data_time.month,
				min_hist.data_time.day,
				min_hist.data_time.hour,
				min_hist.data_time.min,
				min_hist.data_time.sec,
				min_hist.data[0],min_hist.data[1],min_hist.data[2],min_hist.data[3],
				min_hist.data[4],min_hist.data[5],min_hist.data[6],min_hist.data[7],
				min_hist.data[8],min_hist.data[9],min_hist.data[10],min_hist.data[11],
				min_hist.data[12],min_hist.data[13],min_hist.data[14],min_hist.data[15]);

			uart_transmit_str(&COM0,(uint8_t *)ptr_buf);

			delay_vtask(5);
			start_addr = min_hist.next;
		}
		else 
		{
			if(test++ > 20)
			{
				break;
			}
		  start_addr = min_hist.next;			
		}
				
	}
	delay_vtask(1);
	uart_transmit_str(&COM0,(uint8_t *)"读取完毕\r\n");
}
/**
  * @brief  获得给出时间的最接近数据所在flash的地址
  * @param  年月日
  * @retval flash地址
  */

uint32_t hist_date_addr_get(uint8_t year, uint8_t mon, uint8_t day)
{
	hist_t min_hist;  			/*当前数组里面最小的*/
	uint32_t min_day      = 0;
	uint32_t read_day     = 0;
	uint32_t select_day   = 0;
	uint32_t current_addr = 0;
	uint32_t temp_addr    = 0;
	
	select_day  = year * 365 + mon * 30 + day;  //读取的时间
	
	uint8_t err_cnt = 0;
	for(uint32_t sec_count = 0;sec_count <  FLASH_SECTOR_NUM; sec_count ++)   /* 枚举每个扇区的首地址 找出最符合的扇区*/
	{
		hist_read(&hist_data, sec_count * 4096);
		
		if(hist_data.next > 0X400000)  /* 无数据或者数据异常清空(数据异常基本不会出现) */
		{
			if(++err_cnt > 3)
			{
				break;
			}
			continue;
		}
		read_day = hist_data.data_time.year * 365 + hist_data.data_time.month * 30 + hist_data.data_time.day;
		if(read_day >= select_day) //大于等于查找日期进入判断
		{
			read_day += hist_data.data_time.hour; //加上小时进一步判别
			if(min_day == 0) //第一次赋值
			{
				min_day = read_day;
				current_addr = sec_count * 4096;
				memcpy(&min_hist, &hist_data, sizeof(hist_t));
			}
			else if(read_day < min_day) //遇到更小的同时又符合读取
			{
				min_day = read_day;
				current_addr = sec_count * 4096;
				memcpy(&min_hist, &hist_data, sizeof(hist_t));
			}
		}
	}
	
	if(current_addr == 0)        /*当符合日期的时候 则把扇区首地址当成最小值*/
	{
		if(min_day == 0) //查找不到数据
		{
			current_addr = device_get_flahaddr_addr();
			hist_read(&min_hist, current_addr);
			read_day = min_hist.data_time.year * 365 + min_hist.data_time.month * 30 + min_hist.data_time.day;
			if(read_day < select_day)
			{
				return 1;
			}
		}
	}
	read_day = min_hist.data_time.year * 365 + min_hist.data_time.month * 30 + min_hist.data_time.day;
	
		while(1)
	{
		if((current_addr == 0x00 && min_hist.prev == 0x00) || current_addr == 0xffffffff)
		{
			break;
		}
		temp_addr = min_hist.prev;
		hist_read(&min_hist, temp_addr);
		vTaskDelay(2);
		read_day = min_hist.data_time.year * 365 + min_hist.data_time.month * 30 + min_hist.data_time.day;
		if(read_day >= select_day)
		{
			current_addr = temp_addr; 
		}
		else //跳出
		{
			break;
		}
	}
	return current_addr;
}

/**
  * @brief  历史保存回调函数，实现低功耗以及FLASH完成标志
  * @param  None
  * @retval None
  */

