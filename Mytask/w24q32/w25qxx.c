/*
 * @Author: your name
 * @Date: 2020-08-07 11:38:13
 * @LastEditTime: 2020-08-24 10:23:19
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \FM32_NB��ʪ��\BSP\Src\w25q32.c
 */
/* Includes ------------------------------------------------------------------*/
#include "w25qxx.h"
#include "main.h"
/* ����ԭ�� ----------------------------------------------------------*/
/* ��  ---------------------------------------------------------------*/
/* ���� --------------------------------------------------------------*/

#define flash_nss_pin         LL_GPIO_PIN_8
#define flash_sck_pin         LL_GPIO_PIN_9
#define flash_miso_pin        LL_GPIO_PIN_10
#define flash_mosi_pin        LL_GPIO_PIN_11
#define flash_spi_port        GPIOB

/* ���Ͷ��� ----------------------------------------------------------*/
/* �������� ----------------------------------------------------------*/
static void w25qxx_wait_busy(void);
static uint8_t w25qxx_rd_sr(void);
//static void w25qxx_wr_sr(uint8_t sr);
static void w25qxx_wr_enable(void);
//static void w25qxx_wr_disable(void);
static void w25qxx_write_page(uint8_t *wr_buf, uint32_t wr_addr, uint16_t wr_size);
static void w25qxx_write_nocheck(uint8_t *wr_buf, uint32_t wr_addr, uint16_t wr_size);
/* �������� ----------------------------------------------------------*/

//static void w25qxx_cs(int en)
//{
//	en ? LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_8) : LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_8);
//}

static void flash_unlock(void)
{
//	xSemaphoreGive(flash_semap);
}
/**
 * @description:  FLASH?
 * @return None
 */
static void flash_lock(void)
{
//	xSemaphoreTake(flash_semap, portMAX_DELAY);
}
/**
 * @description:  �ȴ�FLASHоƬ��
 * @param {type} 
 * @return {type} 
 */
static void w25qxx_wait_busy(void)
{
	uint16_t wait_num = 0;

	LL_SPI_ResetSSNPin(SPI1);	
//  w25qxx_cs(1);	
	while((w25qxx_rd_sr() & 0X01) == 0X01)
	{
		if(wait_num++ >= 3000)
		{
			break;
		}
//		Trace_log("�ȴ�flash\r\n");
		vTaskDelay(10);
	}
	LL_SPI_SetSSNPin(SPI1);	
//	w25qxx_cs(0);
}

/**
 * @description:  ��ȡW25QXX״̬�Ĵ���
 * @param {type} 
 * @return {type} 
 */

static uint8_t w25qxx_rd_sr(void)
{
	uint8_t sr = 0;
	SPI1WriteAndRead(W25X_ReadStatusReg);
	sr = SPI1WriteAndRead(0Xff);
	return sr;
}

/**
 * @description:  ʹ��д��
 * @param {type} 
 * @return {type} 
 */
static void w25qxx_wr_enable(void)
{
	LL_SPI_ResetSSNPin(SPI1);
//	w25qxx_cs(1);
	SPI1WriteAndRead(W25X_WriteEnable);	
	LL_SPI_SetSSNPin(SPI1);
//	w25qxx_cs(0);
}

/**
 * @description:  �ر�д�����
 * @param {type} 
 * @return {type} 
 */
//static void w25qxx_wr_disable(void)
//{
//	spi_rd_wr(W25X_WriteDisable);
//}

/**
 * @description:  д��FLASHҳ
 * @param  wr_buf:Ҫд�������ָ��
 * @param  wr_addr д��ĵ�ַ
 * @param  wr_szie д��Ĵ�С
 * @return None
 */
static void w25qxx_write_page(uint8_t *wr_buf, uint32_t wr_addr, uint16_t wr_size)
{
	w25qxx_wait_busy();
	w25qxx_wr_enable();
	w25qxx_wait_busy();
//	portENTER_CRITICAL();
	LL_SPI_ResetSSNPin(SPI1);
//	w25qxx_cs(1);
	
	SPI1WriteAndRead(W25X_PageProgram);
	SPI1WriteAndRead((uint8_t)((wr_addr) >> 16));
	SPI1WriteAndRead((uint8_t)((wr_addr) >> 8));
	SPI1WriteAndRead((uint8_t)wr_addr);
	
	while(wr_size--)
	{
		SPI1WriteAndRead( *wr_buf++);
	}
	LL_SPI_SetSSNPin(SPI1);
//	w25qxx_cs(0);
//	portEXIT_CRITICAL();
	w25qxx_wait_busy();
}
/**
 * @description:  д��FLASH ��󳤶Ȳ�����65535 ��ҪԤ�ȼ��flash
 * @param  wr_buf:Ҫд�������ָ��
 * @param  wr_addr д��ĵ�ַ
 * @param  wr_szie д��Ĵ�С
 * @return None
 */
static void w25qxx_write_nocheck(uint8_t *wr_buf, uint32_t wr_addr, uint16_t wr_size)
{
	uint16_t pagermain;
	pagermain = 256 - wr_addr % 256;  /*���ʣ��ռ�*/
	if(wr_size <= pagermain)   
	{
		pagermain = wr_size;  /*Ҫд���С��ʣ��ռ�ֱ��д�룬���������д��ʣ��ռ�*/
	}
	while(1)
	{
		w25qxx_write_page(wr_buf, wr_addr, pagermain);
		if(wr_size == pagermain)
		{
			break; /*д���ӵ�*/
		}
		else 
		{
			wr_buf += pagermain;
			wr_addr += pagermain;
			wr_size -= pagermain;
			
			if(wr_size > 256)
			{
				pagermain = 256;
			}else 
			{
				pagermain = wr_size;
			}
		}
	};
}

/**
 * @description:  ����оƬ
 * @param {type} 
 * @return {type} 
 */
void w25qxx_erase_chip(void)
{
	flash_lock();
	w25qxx_wait_busy();	
	
	w25qxx_wr_enable();
	
	w25qxx_wait_busy();	
		
	LL_SPI_ResetSSNPin(SPI1);
//	w25qxx_cs(1);
	SPI1WriteAndRead(W25X_ChipErase);
	LL_SPI_SetSSNPin(SPI1);
//	w25qxx_cs(0);
	
	w25qxx_wait_busy();
	flash_unlock();
}

/**
 * @description:  ����оƬ
 * @param {type} 
 * @return {type} 
 */
void w25qxx_erase_sector(uint32_t dst_addr)
{
	flash_lock();
	w25qxx_wait_busy();	
	
	w25qxx_wr_enable();
	
	w25qxx_wait_busy();
	
//	portENTER_CRITICAL();
	/*���ҳ*/
	LL_SPI_ResetSSNPin(SPI1);
//	w25qxx_cs(1);
	SPI1WriteAndRead( W25X_SectorErase);
	SPI1WriteAndRead( (uint8_t)((dst_addr) >> 16));
	SPI1WriteAndRead( (uint8_t)((dst_addr) >> 8));
	SPI1WriteAndRead( (uint8_t)dst_addr);	
	LL_SPI_SetSSNPin(SPI1);
//	w25qxx_cs(0);
//	portEXIT_CRITICAL();
	w25qxx_wait_busy();
	flash_unlock();
}

/**
 * @description: FLASHоƬ����
 * @param  None
 * @return None
 */
void w25qxx_power_down(void)
{
	flash_lock();
	LL_SPI_ResetSSNPin(SPI1);
	SPI1WriteAndRead( W25X_PowerDown);
	LL_SPI_SetSSNPin(SPI1);
	flash_unlock();
}

/**
 * @description: FLASHоƬ����
 * @param  None
 * @return None
 */
void w25qxx_power_wakeup(void)
{
	flash_lock();
	LL_SPI_ResetSSNPin(SPI1);
	SPI1WriteAndRead(W25X_ReleasePowerDown);
	LL_SPI_SetSSNPin(SPI1);
	flash_unlock();
}
/**
 * @description:  FLASH��ȡ
 * @param  read_buf: ��ȡ���������ָ���ַ
 * @param  read_addr ��ȡ��FLASH��ַ
 * @param  read_szie Ҫ��ȡ�Ĵ�С
 * @return None
 */
void w25qxx_read(uint8_t *read_buf, uint32_t read_addr, uint16_t read_size)
{
	flash_lock();
	w25qxx_wait_busy();
	LL_SPI_ResetSSNPin(SPI1);
	SPI1WriteAndRead(W25X_ReadData);
	SPI1WriteAndRead((uint8_t)((read_addr) >> 16));
	SPI1WriteAndRead((uint8_t)((read_addr) >> 8));
	SPI1WriteAndRead((uint8_t)read_addr);
	while(read_size--) {
		*read_buf++ = SPI1WriteAndRead(0XFF);
	}
	LL_SPI_SetSSNPin(SPI1);
	flash_unlock();
}

/**
 * @description: w25qxxд��
 * @param  wr_buf:Ҫд�������ָ��
 * @param  wr_addr д��ĵ�ַ
 * @param  wr_szie д��Ĵ�С
 * @return None
 */
void w25qxx_write(uint8_t *wr_buf, uint32_t wr_addr, uint16_t wr_size)
{
	flash_lock();
	w25qxx_write_nocheck(wr_buf, wr_addr, wr_size); /*ֱ��д�� ��Ϊ����FF*/
	flash_unlock();
}
/**
 * @description:  w25qxx��������״̬
 * @param  None
 * @return None
 */
void w25qxx_enter_sleep_mode(void)
{
	LL_GPIO_InitTypeDef io_config = {0}; 
	io_config.Pin        = flash_nss_pin | flash_miso_pin | flash_mosi_pin | flash_sck_pin;
	io_config.Mode       = LL_GPIO_MODE_INPUT;
	io_config.Pull       = DISABLE;
	io_config.RemapPin   = DISABLE;
	
	LL_GPIO_Init(flash_spi_port, &io_config);
	LL_GPIO_DisablePinInput(flash_spi_port, flash_nss_pin | flash_miso_pin | flash_mosi_pin | flash_sck_pin);
	
}

/**
 * @description:  w25qxx�˳�����״̬
 * @param  None
 * @return None
 */
void w25qxx_exti_sleep_mode(void)
{
	LL_GPIO_InitTypeDef io_config = {0}; 
	io_config.Pin = flash_nss_pin | flash_miso_pin | flash_mosi_pin | flash_sck_pin;
	io_config.Mode = LL_GPIO_MODE_DIGITAL;
	io_config.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	io_config.Pull = ENABLE;
	io_config.RemapPin = DISABLE;
	LL_GPIO_Init(GPIOB, &io_config);

	LL_GPIO_EnablePinInput(flash_spi_port, flash_miso_pin | flash_mosi_pin | flash_nss_pin | flash_sck_pin);
}

/**
 * @description: ��ʼ��W25qXX
 * @param  None
 * @return None
 */
void w25qxx_init(void)
{
	uint8_t err_cnt = 0;
  	SPI1Init();	
	LL_SPI_ResetSSNPin(SPI1);

	while((w25qxx_rd_sr() & 0X01) == 0X01)
	{
		if(++err_cnt > 10)
		{
			break;
		}
		vTaskDelay(10);
	}
  	LL_SPI_SetSSNPin(SPI1);
	w25qxx_power_wakeup();
	
}
