#include "Includes.h"

///* 定义SPI总线的 GPIO端口 */
//#define RCC_CS 	  LL_IOP_GRP1_PERIPH_GPIOB
//#define PORT_CS	  GPIOB
//#define PIN_CS		LL_GPIO_PIN_0

//#define RCC_SCK 	LL_IOP_GRP1_PERIPH_GPIOB
//#define PORT_SCK	GPIOB
//#define PIN_SCK		LL_GPIO_PIN_1

//#define RCC_MISO 	LL_IOP_GRP1_PERIPH_GPIOB
//#define PORT_MISO	GPIOB
//#define PIN_MISO	LL_GPIO_PIN_2

//#define RCC_MOSI 	LL_IOP_GRP1_PERIPH_GPIOB
//#define PORT_MOSI	GPIOB
//#define PIN_MOSI	LL_GPIO_PIN_10

//	#define CS_0()		PORT_CS->BRR = PIN_CS
//	#define CS_1()		PORT_CS->BSRR = PIN_CS

//	#define SCK_0()		PORT_SCK->BRR = PIN_SCK
//	#define SCK_1()		PORT_SCK->BSRR = PIN_SCK

//	#define MOSI_0()	PORT_MOSI->BRR = PIN_MOSI
//	#define MOSI_1()	PORT_MOSI->BSRR = PIN_MOSI

//	#define MISO_IS_HIGH()	(LL_GPIO_IsInputPinSet(PORT_MISO, PIN_MISO) == 1)/////


uint8_t g_spi_busy = 0;		/* SPI 总线共享标志 */

void bsp_InitSPIBus(void)
{
	LL_GPIO_InitTypeDef  GPIO_InitStructure;

	/* 打开GPIO时钟 */
	//LL_IOP_GRP1_EnableClock(RCC_SCK | RCC_MOSI | RCC_MISO);	
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

	/* 配置SPI引脚SCK、MISO 和 MOSI为复用推挽模式 */
	GPIO_InitStructure.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Mode = LL_GPIO_MODE_OUTPUT;	/* 推挽输出模式 */
	GPIO_InitStructure.Pin = PIN_SCK;
	LL_GPIO_Init(PORT_SCK, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = PIN_MOSI;
	LL_GPIO_Init(PORT_MOSI, &GPIO_InitStructure);	
	
	GPIO_InitStructure.Pin = PIN_CS;
	LL_GPIO_Init(PORT_CS, &GPIO_InitStructure);	
	
	GPIO_InitStructure.Mode = LL_GPIO_MODE_INPUT;		/* MISO 设置为输入上拉 */
	GPIO_InitStructure.Pin = PIN_MISO;
	LL_GPIO_Init(PORT_MISO, &GPIO_InitStructure);

CS_1();
SCK_1();
MOSI_1();
}
	
void bsp_spiDelay(void)
{
	uint32_t i;

	for (i = 0; i < 2; i++);
//	dwt_delay_us(1);
}
/*
*********************************************************************************************************
*	函 数 名: bsp_spiWrite0
*	功能说明: 向SPI总线发送一个字节。SCK上升沿采集数据, SCK空闲时为低电平。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_spiWrite0(uint8_t _ucByte,uint8_t cnt)
{
	uint8_t i;
	for(i = 0; i < cnt ; i++)
	{
		if (_ucByte & 0x80)
		{
			MOSI_1();
		}
		else
		{
			MOSI_0();
		}
		bsp_spiDelay();
		SCK_1();
		_ucByte <<= 1;
		bsp_spiDelay();
		SCK_0();
	}
	bsp_spiDelay();
}
/*
*********************************************************************************************************
*	函 数 名: bsp_spiRead0
*	功能说明: 从SPI总线接收8个bit数据。 SCK上升沿采集数据, SCK空闲时为低电平。
*	形    参: 无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t bsp_spiRead0(void)
{
	uint8_t i;
	uint8_t read = 0;
	for (i = 0; i < 8; i++)
	{
		read = read<<1;
		if (MISO_IS_HIGH())
		{
			read++;
		}
		SCK_1();
		bsp_spiDelay();
		SCK_0();
		bsp_spiDelay();
	}
	return read;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_spiWrite1
*	功能说明: 向SPI总线发送一个字节。  SCK上升沿采集数据, SCK空闲时为高电平
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_spiWrite1(uint8_t _ucByte ,uint8_t cnt)
{
	uint8_t i;
	for(i = 0; i < cnt ; i++)
	{
		if (_ucByte & 0x80)
		{
			MOSI_1();
		}
		else
		{
			MOSI_0();
		}
		SCK_0();
		_ucByte <<= 1;
		bsp_spiDelay();
		SCK_1();				/* SCK上升沿采集数据, SCK空闲时为高电平 */
		bsp_spiDelay();
	}
}
void bsp_spiWrite2(uint8_t _ucByte,uint8_t cnt )
{
	uint8_t i;

	for(i = 0; i < cnt ; i++)
	{
		if (_ucByte & 0x01)
		{
			MOSI_1();
		}
		else
		{
			MOSI_0();
		}
		SCK_0();
		_ucByte >>= 1;
		bsp_spiDelay();
		SCK_1();				/* SCK上升沿采集数据, SCK空闲时为高电平 */
		bsp_spiDelay();
	}

}
/*
*********************************************************************************************************
*	函 数 名: bsp_spiRead1
*	功能说明: 从SPI总线接收8个bit数据。  SCK上升沿采集数据, SCK空闲时为高电平
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t bsp_spiRead1(void)
{

	uint8_t i;
	uint8_t read = 0;

	for (i = 0; i < 8; i++)
	{
		SCK_0();
		bsp_spiDelay();
		read = read << 1;
		if (MISO_IS_HIGH())
		{
			read++;
		}
		SCK_1();
		bsp_spiDelay();
	}
	return read;
}


/*
*********************************************************************************************************
*	函 数 名: bsp_SpiBusEnter
*	功能说明: 占用SPI总线
*	形    参: 无
*	返 回 值: 0 表示不忙  1表示忙
*********************************************************************************************************
*/
void bsp_SpiBusEnter(void)
{
	g_spi_busy = 1;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SpiBusExit
*	功能说明: 释放占用的SPI总线
*	形    参: 无
*	返 回 值: 0 表示不忙  1表示忙
*********************************************************************************************************
*/
void bsp_SpiBusExit(void)
{
	g_spi_busy = 0;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SpiBusBusy
*	功能说明: 判断SPI总线忙。方法是检测其他SPI芯片的片选信号是否为1
*	形    参: 无
*	返 回 值: 0 表示不忙  1表示忙
*********************************************************************************************************
*/
uint8_t bsp_SpiBusBusy(void)
{
	return g_spi_busy;
}
/*
*********************************************************************************************************
*	函 数 名: bsp_SetSpiSck
*	功能说明: 用于软件模式。设置SCK GPIO的状态。在函数CS=0之前被调用，用于不同相序的SPI设备间切换。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetSpiSck(uint8_t _data)
{
	if (_data == 0)
	{
		SCK_0();
	}
	else
	{
		SCK_1();
	}
}



