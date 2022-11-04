#include "taskSystem.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "uart.h"

#if 0
extern  uint8_t TestRxData[100];
extern QueueHandle_t dmaSemaphore;
void UartTask(void *pvParameters)
{
	uint32_t event_bit = 0;
	uint8_t *usb_buf;
	uint16_t us_length;
	usb_buf = pvPortMalloc(64);
	for(;;) {
		us_length = uart_recv_data(&COM0, usb_buf, 64, 20);
		if(us_length >= 7)
		{	
			if(usb_buf[0] == 0XFF)   /* 配置工具指令 */
			{				
				if(usb_buf[2] == 0X4B)
				{
					device_param_wr(usb_buf);
			}
				else if(usb_buf[2] == 0X4D && us_length >= 14)    //set time
				{
					#if 0
					device_rtc_set(usb_buf, &device_info.data_time);	
					RTC_WriteDate(&device_info.data_time);
					#endif
				}
			}
			else if(usb_buf[0] == 0X7E && us_length >= 15)   /* 读取历史数据 */
			{		
				if(usb_buf[2] == 0X02 )
				{
					#if 0
					s_year = usb_buf[10];
					s_mon  = usb_buf[11];
					s_day  = usb_buf[12];
					e_year = usb_buf[13];
					e_mon  = usb_buf[14];
					e_day  = usb_buf[15];						
				
					device_info.event_flash = EVENT_RUN;
					xTaskNotify(device_flashHandle, R_FLASH, eSetBits);
					#endif
				}
			}
			#if 0
			if((usb_buf[3] == 0X2F  || usb_buf[3] == 0X29)    /* 更新程序 */
				&& crc_16(usb_buf, us_length - 3) == (((uint16_t )usb_buf[us_length - 2] << 8) | usb_buf[us_length - 3]))
			{		
				FL_FLASH_PageErase(FLASH, 1024 * 8);
				device_restart();
			}
			#endif
		}
		#if 
		xTaskNotifyWait(0x00, 0xffffffff, &event_bit, 100);
		#endif
	}
}
#endif

