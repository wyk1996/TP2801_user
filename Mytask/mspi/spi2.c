#include "Includes.h"

///* ����SPI���ߵ� GPIO�˿� */
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


uint8_t g_spi_busy = 0;		/* SPI ���߹����־ */

void bsp_InitSPIBus(void)
{
	LL_GPIO_InitTypeDef  GPIO_InitStructure;

	/* ��GPIOʱ�� */
	//LL_IOP_GRP1_EnableClock(RCC_SCK | RCC_MOSI | RCC_MISO);	
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

	/* ����SPI����SCK��MISO �� MOSIΪ��������ģʽ */
	GPIO_InitStructure.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Mode = LL_GPIO_MODE_OUTPUT;	/* �������ģʽ */
	GPIO_InitStructure.Pin = PIN_SCK;
	LL_GPIO_Init(PORT_SCK, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = PIN_MOSI;
	LL_GPIO_Init(PORT_MOSI, &GPIO_InitStructure);	
	
	GPIO_InitStructure.Pin = PIN_CS;
	LL_GPIO_Init(PORT_CS, &GPIO_InitStructure);	
	
	GPIO_InitStructure.Mode = LL_GPIO_MODE_INPUT;		/* MISO ����Ϊ�������� */
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
*	�� �� ��: bsp_spiWrite0
*	����˵��: ��SPI���߷���һ���ֽڡ�SCK�����زɼ�����, SCK����ʱΪ�͵�ƽ��
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_spiRead0
*	����˵��: ��SPI���߽���8��bit���ݡ� SCK�����زɼ�����, SCK����ʱΪ�͵�ƽ��
*	��    ��: ��
*	�� �� ֵ: ����������
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
*	�� �� ��: bsp_spiWrite1
*	����˵��: ��SPI���߷���һ���ֽڡ�  SCK�����زɼ�����, SCK����ʱΪ�ߵ�ƽ
*	��    ��:  ��
*	�� �� ֵ: ��
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
		SCK_1();				/* SCK�����زɼ�����, SCK����ʱΪ�ߵ�ƽ */
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
		SCK_1();				/* SCK�����زɼ�����, SCK����ʱΪ�ߵ�ƽ */
		bsp_spiDelay();
	}

}
/*
*********************************************************************************************************
*	�� �� ��: bsp_spiRead1
*	����˵��: ��SPI���߽���8��bit���ݡ�  SCK�����زɼ�����, SCK����ʱΪ�ߵ�ƽ
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_SpiBusEnter
*	����˵��: ռ��SPI����
*	��    ��: ��
*	�� �� ֵ: 0 ��ʾ��æ  1��ʾæ
*********************************************************************************************************
*/
void bsp_SpiBusEnter(void)
{
	g_spi_busy = 1;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SpiBusExit
*	����˵��: �ͷ�ռ�õ�SPI����
*	��    ��: ��
*	�� �� ֵ: 0 ��ʾ��æ  1��ʾæ
*********************************************************************************************************
*/
void bsp_SpiBusExit(void)
{
	g_spi_busy = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SpiBusBusy
*	����˵��: �ж�SPI����æ�������Ǽ������SPIоƬ��Ƭѡ�ź��Ƿ�Ϊ1
*	��    ��: ��
*	�� �� ֵ: 0 ��ʾ��æ  1��ʾæ
*********************************************************************************************************
*/
uint8_t bsp_SpiBusBusy(void)
{
	return g_spi_busy;
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_SetSpiSck
*	����˵��: �������ģʽ������SCK GPIO��״̬���ں���CS=0֮ǰ�����ã����ڲ�ͬ�����SPI�豸���л���
*	��    ��: ��
*	�� �� ֵ: ��
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



