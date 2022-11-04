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
		w25qxx_read(buff,netAddr,8); //��ѯ��ʽ��ÿ�ζ�8����
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

	//��ȡ��ַ ���û����ܻᵼ���쳣
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
	
	if(hist_check(addr))  /*У����� */
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
  * @brief  ��ʷ���ݱ��溯��
  * @param  д���ַ
  * @retval None
*****************************/

uint8_t hist_save(uint32_t *addr)
{
	uint8_t i;	
	if((*addr % FLASH_SECTOR_SIZE + sizeof(hist_t)) / sizeof(hist_t) == (FLASH_SECTOR_SIZE / sizeof(hist_t)))   /*�ж��Ƿ��������һ��*/
	{
		Trace_log("FLASHд�������仯\r\n");
		if((*addr / FLASH_SECTOR_SIZE + 1)!= FLASH_SECTOR_NUM)
		{
			hist_data.next = (*addr / FLASH_SECTOR_SIZE + 1) * FLASH_SECTOR_SIZE;/*next_addr ָ����һ��������ʼ��ַ*/
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
		Trace_log("�����豸\r\n");
		/*�����׵�ַ*/
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

	hist_data.crc = crc_16((uint8_t *)&hist_data, sizeof(hist_t) - 4);  /*���crc*/	
  	Trace_log("д����Ϣ\r\n");	
	w25qxx_write((uint8_t *)&hist_data, *addr, sizeof(hist_t));	
	w25qxx_read((uint8_t *)&hist_data, *addr, sizeof(hist_t));
	if(hist_data.data_time.year != device_info.data_time.year || hist_data.data_time.month != device_info.data_time.month)
	{
	  Trace_log("д��ʧ��\r\n");
		return 1;
	}
	Trace_log("д��ɹ�\r\n");	
	

  	device_set_flahaddr_addr(*addr);	
	*addr = hist_data.next;  /* ƫ�Ƶ���ǰ��ַ  */
 
	return 0;
}

/**
  * @brief  ������һ�������Ƿ�������ڵ��籣������
  * @param  ��ȡ��ַ
  * @retval 1�����絼�����ݴ��� 0����������
  */

uint8_t hist_check(uint32_t addr)
{
	uint16_t crc_d = 0;
	w25qxx_read((uint8_t *)&hist_data, addr, sizeof(hist_t));
	crc_d = crc_16((uint8_t *)&hist_data, sizeof(hist_t) - 4);

	if(hist_data.crc != crc_d)   /* У��CRC�Ƿ���ȷ */
	{
			Trace_log("SAVE CRC ERR\r\n");
		return 1;
	}
	return 0;
}

/**
  * @brief  ��ȡ��ʷ����
  * @param  ��ȡ��Žṹ��
	* @param  ��ȡ��ַ
  * @retval None
  */
void hist_read(hist_t *read_hist, uint32_t addr)
{
	w25qxx_read((uint8_t *)read_hist, addr ,sizeof(hist_t));
}

/**
  * @brief  ��ȡ���ݣ�����ʼ�����պͽ���������������
  * @param  ��ʼ������-->����������
  * @retval None
  */
uint32_t start_addr = 1;
void hist_read_date(uint8_t s_year, uint8_t s_mon, uint8_t s_day, uint8_t e_year, uint8_t e_mon, uint8_t e_day)
{
	
	uint32_t read_count = 0;
	hist_t min_hist;  			/*��ǰ����������С��*/
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
	uart_transmit_str(&COM0,(uint8_t *)"��¼ʱ��        CH0     CH1    CH2    CH3    CH4    CH5    CH6    CH7    CH8    CH9    CH10    CH11    CH12    CH13    CH14    CH15\r\n");
	if(start_addr == 1)
	{
		return ;
	}
	delay_vtask(5);
	while(1) {
		w25qxx_read((uint8_t *)&min_hist, start_addr, sizeof(hist_t));
		
		if((++read_count >= (4096 / sizeof(hist_t) * 1024)) || (min_hist.next == 0XFFFFFFFF))
		{
			break;  /*����һȦ �����˳�*/
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
	uart_transmit_str(&COM0,(uint8_t *)"��ȡ���\r\n");
}
/**
  * @brief  ��ø���ʱ�����ӽ���������flash�ĵ�ַ
  * @param  ������
  * @retval flash��ַ
  */

uint32_t hist_date_addr_get(uint8_t year, uint8_t mon, uint8_t day)
{
	hist_t min_hist;  			/*��ǰ����������С��*/
	uint32_t min_day      = 0;
	uint32_t read_day     = 0;
	uint32_t select_day   = 0;
	uint32_t current_addr = 0;
	uint32_t temp_addr    = 0;
	
	select_day  = year * 365 + mon * 30 + day;  //��ȡ��ʱ��
	
	uint8_t err_cnt = 0;
	for(uint32_t sec_count = 0;sec_count <  FLASH_SECTOR_NUM; sec_count ++)   /* ö��ÿ���������׵�ַ �ҳ�����ϵ�����*/
	{
		hist_read(&hist_data, sec_count * 4096);
		
		if(hist_data.next > 0X400000)  /* �����ݻ��������쳣���(�����쳣�����������) */
		{
			if(++err_cnt > 3)
			{
				break;
			}
			continue;
		}
		read_day = hist_data.data_time.year * 365 + hist_data.data_time.month * 30 + hist_data.data_time.day;
		if(read_day >= select_day) //���ڵ��ڲ������ڽ����ж�
		{
			read_day += hist_data.data_time.hour; //����Сʱ��һ���б�
			if(min_day == 0) //��һ�θ�ֵ
			{
				min_day = read_day;
				current_addr = sec_count * 4096;
				memcpy(&min_hist, &hist_data, sizeof(hist_t));
			}
			else if(read_day < min_day) //������С��ͬʱ�ַ��϶�ȡ
			{
				min_day = read_day;
				current_addr = sec_count * 4096;
				memcpy(&min_hist, &hist_data, sizeof(hist_t));
			}
		}
	}
	
	if(current_addr == 0)        /*���������ڵ�ʱ�� ��������׵�ַ������Сֵ*/
	{
		if(min_day == 0) //���Ҳ�������
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
		else //����
		{
			break;
		}
	}
	return current_addr;
}

/**
  * @brief  ��ʷ����ص�������ʵ�ֵ͹����Լ�FLASH��ɱ�־
  * @param  None
  * @retval None
  */

