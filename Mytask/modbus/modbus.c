/*
 * @Author: Snail
 * @Date: 2019-09-09 22:03:04
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-10-19 09:47:03
 * @Description: 
 */
/*
 * mbrtu_slave.c
 *
 *  Created on: 2019��2��19��
 *      Author: Snail
 */
#include "main.h"
#include "modbus.h"


//���豸��֧�ֵ�modbus������ʹ�� 03 06 10
// 03 06 10 һ�����ʹ�� �����Ĵ�������
// 01 05 15 һ�����ʹ�� �����̵���������Ȧ
// ���������룬�����ⳡ��һ�㲻��
#define modbus_rtu_01   DISABLE      //bit����  ����Ȧ״̬
#define modbus_rtu_02   DISABLE      //bit����  ��������ɢ��
#define modbus_rtu_03   ENABLE       //16bit����  ������/������ּĴ�������
#define modbus_rtu_04   DISABLE      //16bit����  ������Ĵ�������
#define modbus_rtu_05   DISABLE      //bit����    д������Ȧ
#define modbus_rtu_06   ENABLE       //16bit����  д�����Ĵ�������
#define modbus_rtu_0F   DISABLE      //bite����   д�����Ȧ
#define modbus_rtu_10   ENABLE       //16bit����  д����Ĵ�������
#define modbus_rtu_14   DISABLE      //�ļ���¼����  ���ļ���¼    һ�㲻��
#define modbus_rtu_15   DISABLE      //�ļ���¼����  д�ļ���¼    һ�㲻��
#define modbus_rtu_16   DISABLE      //16bit����  ����д�Ĵ���     һ�㲻��
#define modbus_rtu_17   DISABLE      //16bit����  ��д����Ĵ���   һ�㲻��
#define modbus_rtu_2B   DISABLE      //��ȡ�豸��Ϣ������          һ�㲻��
#define modbus_rtu_err  ENABLE      //��������Ӧ��    һ��ʹ��
/* USER CODE BEGIN Private defines */


/*DI����ֵ�Ĵ���ֵ, AIģ�����Ĵ���ֵ*/

uint8_t count_value_reg[modbus_buf_num*2];   //���ݴ洢buf   ���мĴ�������16λ���䣬����Ϊfloat����
/* 

*/

/* MB������� */
#define RSP_ERR_CMD        0X01   /* ��֧�ֵĹ����� */
#define RSP_ERR_REG_ADDR   0X02	  /* �Ĵ�����ַ���� */
#define RSP_ERR_VALUE      0X03   /* ����ֵ����� */
#define RSP_ERR_WRITE      0X04	  /* д��ʧ�� */

/**************modbus tcpӦ���������***********************************/
static uint8_t mbs_tcp_send_ack_err(uint8_t *rx_cmd, uint8_t *ack_buf, uint8_t err_code)
{
	ack_buf[0] = rx_cmd[0];
	ack_buf[1] = rx_cmd[1];
	ack_buf[2] = rx_cmd[2];
	ack_buf[3] = rx_cmd[3];
	ack_buf[4] = 0x00;
	ack_buf[5] = 0x03;
	ack_buf[6] = rx_cmd[6];
	ack_buf[7] = rx_cmd[7] | 0x80;
	ack_buf[8] = err_code;

	return 9;
}
/**************modbus rtuӦ���������***********************************/
static uint8_t mbs_rtu_send_ack_err(uint8_t *rx_cmd, uint8_t *ack_buf, uint8_t err_code)
{
	uint16_t crc;

	ack_buf[0] = rx_cmd[0];
	ack_buf[1] = rx_cmd[1] | 0x80;
	ack_buf[2] = err_code;

	crc = crc_16(ack_buf, 3);

	ack_buf[3] = crc >> 8;
	ack_buf[4] = crc;

	return 5;
}
///**************modbus д��float���ݵ�����***********************************/
//void mbs_write_reg_float(float *value, uint8_t fun_code, uint16_t reg_addr)
//{
//	uint8_t *p_buf;
//	
//	if(fun_code == 0x04)
//	{
//		p_buf= &analog_value_reg[reg_addr * 2];
//	}
//	else if(fun_code == 0x03)
//	{
//		p_buf = &count_value_reg[reg_addr * 2];
//	}
//	uint8_t *temp;
//	temp = (uint8_t *)value;

//	p_buf[3] = *temp;temp++;
//	p_buf[2] = *temp;temp++;
//	p_buf[1] = *temp;temp++;
//	p_buf[0] = *temp;
//}
///**************modbus д��int���ݵ�����***********************************/
//void mbs_write_reg_int(uint32_t *value, uint8_t fun_code, uint16_t reg_addr)
//{
//	uint8_t *p_buf;
//	if(fun_code == 0x03)
//	{
//		p_buf= &count_value_reg[reg_addr * 2];
//	}
//	else if(fun_code == 0x04)
//	{
//		p_buf= &analog_value_reg[reg_addr * 2];
//	}
//	uint8_t *temp;
//	temp = (uint8_t *)value;
//	
//	p_buf[3] = *temp;temp++;
//	p_buf[2] = *temp;temp++;
//	p_buf[1] = *temp;temp++;
//	p_buf[0] = *temp;
//}
/**********************************************************************************/
static uint16_t BEBuf2Uint16(uint8_t *buf)
{
	uint16_t ret_data;
	ret_data = (buf[0]<<8)+buf[1];
	return ret_data;
}
/**************modbus tcp 01�����봦��***********************************/
static uint8_t mbs_tcp_01(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf)
{
	uint16_t reg, read_cnt, cnt ,ack_size;

	if(modbus_rtu_01 == DISABLE)    //��֧�ָĹ�����
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_CMD);
	}
	
	reg = BEBuf2Uint16(&rx_cmd[8]);       //�Ĵ�����ʼ��ַ
	read_cnt = BEBuf2Uint16(&rx_cmd[10]);     //�Ĵ�������	
	if(reg < modbus_start_add || reg > (modbus_start_add+modbus_buf_num))     //��ʼ��ַ������ַ��Χ
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_REG_ADDR);
	}
	else if((reg + read_cnt-modbus_start_add) > modbus_buf_num)   //��ȡ�����򳬳���Χ
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_VALUE);
	}
	 
	ack_size = (read_cnt + 7) / 8;

	ack_buf[0] = rx_cmd[0];
	ack_buf[1] = rx_cmd[1];
	ack_buf[2] = rx_cmd[2];
	ack_buf[3] = rx_cmd[3];
	ack_buf[4] = 0;
	ack_buf[5] = ack_size + 3;
	ack_buf[6] = rx_cmd[6];
	ack_buf[7] = rx_cmd[7];
	ack_buf[8] = ack_size;

	/* �Ӹ�λ��ȡ����λ */
	reg = read_cnt + reg - 1;
	for(cnt = 0; cnt < read_cnt; cnt++)
	{
		ack_buf[9] <<= 1;
//		ack_buf[9] |= do_read(reg--);
	}

	return ack_buf[8] + 9;
}
/**************modbus tcp 02�����봦��***********************************/
/* ������������ */
static uint8_t mbs_tcp_02(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf)
{
	uint16_t reg, read_cnt, ack_size, cnt;

	if(modbus_rtu_02 == DISABLE)    //��֧�ָĹ�����
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_CMD);
	}

	reg = BEBuf2Uint16(&rx_cmd[8]);
	read_cnt = BEBuf2Uint16(&rx_cmd[10]);

	if((reg + read_cnt) > 8)
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_VALUE);
	}

	ack_size = (read_cnt + 7) / 8;

	ack_buf[0] = rx_cmd[0];
	ack_buf[1] = rx_cmd[1];
	ack_buf[2] = rx_cmd[2];
	ack_buf[3] = rx_cmd[3];
	ack_buf[4] = 0;
	ack_buf[5] = ack_size + 3;
	ack_buf[6] = rx_cmd[6];
	ack_buf[7] = rx_cmd[7];
	ack_buf[8] = ack_size;

	/* �Ӹ�λ��ȡ����λ */
	reg = read_cnt + reg - 1;
	for(cnt = 0; cnt < read_cnt; cnt++)
	{
		ack_buf[9] <<= 1;
//		ack_buf[9] |= di_read(reg--);
	}

	return ack_buf[8] + 9;
}
/**************modbus tcp 03�����봦��***********************************/
/* ��ȡ03�Ĵ��� ��д�Ĵ���*/
static uint8_t mbs_tcp_03(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf)
{
	uint16_t reg, value_num, cnt;

	if(modbus_rtu_03 == DISABLE)    //��֧�ָù�����
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_CMD);
	}
	
	reg = BEBuf2Uint16(&rx_cmd[8]);      //�Ĵ�����ʼ��ַ
	value_num = BEBuf2Uint16(&rx_cmd[10]);    //�Ĵ�������

	if(reg < modbus_start_add || reg > (modbus_start_add+modbus_buf_num))     //��ʼ��ַ������ַ��Χ
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_REG_ADDR);
	}
	else if((reg + value_num-modbus_start_add) > modbus_buf_num)   //��ȡ�����򳬳���Χ
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_VALUE);
	}
  value_num *= 2;
	/* ȡֵ */
	for(cnt = 0; cnt < value_num; cnt++)      //ÿ���Ĵ���Ϊ16bit
	{
		ack_buf[9 + cnt] = count_value_reg[cnt + reg * 2];
	}
	
	ack_buf[0] = rx_cmd[0];
	ack_buf[1] = rx_cmd[1];
	ack_buf[2] = rx_cmd[2];
	ack_buf[3] = rx_cmd[3];
	ack_buf[4] = (value_num + 3) >> 8;
	ack_buf[5] = (value_num + 3);
	ack_buf[6] = rx_cmd[6];
	ack_buf[7] = 3;
	ack_buf[8] = value_num;

	return value_num + 9;
}
/**************modbus tcp 04�����봦��***********************************/
/* ��ȡ04�Ĵ�����ֻ������ģ���� */
static uint8_t mbs_tcp_04(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf)
{
	uint16_t reg, value_num, cnt;

	if(modbus_rtu_04 == DISABLE)    //��֧�ָù�����
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_CMD);
	}
	
	reg = BEBuf2Uint16(&rx_cmd[8]);
	value_num = BEBuf2Uint16(&rx_cmd[10]);

	if(reg < modbus_start_add || reg > (modbus_start_add+modbus_buf_num))     //��ʼ��ַ������ַ��Χ
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_REG_ADDR);
	}
	else if((reg + value_num-modbus_start_add) > modbus_buf_num)   //��ȡ�����򳬳���Χ
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_VALUE);
	}
	value_num *= 2;

	/* ȡֵ */
	for(cnt = 0; cnt < value_num; cnt++)
	{
//		ack_buf[9 + cnt] = analog_value_reg[cnt + reg * 2];
	}

	ack_buf[0] = rx_cmd[0];
	ack_buf[1] = rx_cmd[1];
	ack_buf[2] = rx_cmd[2];
	ack_buf[3] = rx_cmd[3];
	ack_buf[4] = (value_num + 3) >> 8;
	ack_buf[5] = (value_num + 3);
	ack_buf[6] = rx_cmd[6];
	ack_buf[7] = 4;
	ack_buf[8] = value_num;

	return value_num + 9;
}
/**************modbus tcp 05�����봦��***********************************/
static uint8_t mbs_tcp_05(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf)
{
//	uint16_t reg;

	if(modbus_rtu_05 == DISABLE)    //��֧�ָù�����
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_CMD);
	}
//	reg = BEBuf2Uint16(&rx_cmd[8]);
//	if(reg > 4)
//	{
//		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_VALUE); 
//	}
//	/* �����̵��� */
//	do_write(reg, rx_cmd[10]);
//	memcpy(ack_buf, rx_cmd, cmd_size);

	return cmd_size;
}
/**************modbus tcp 06�����봦��***********************************/
static uint8_t mbs_tcp_06(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf)
{
	uint16_t reg, cnt;
	uint8_t i;
	if(modbus_rtu_06 == DISABLE)    //��֧�ָù�����
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_CMD);
	}
	reg = BEBuf2Uint16(&rx_cmd[8]);      //�Ĵ�����ַ
	if(reg < modbus_start_add || reg > (modbus_start_add+modbus_buf_num))     //��ʼ��ַ������ַ��Χ
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_REG_ADDR);
	}
	count_value_reg[reg*2] = rx_cmd[10];
	count_value_reg[reg*2+1] = rx_cmd[11];
	
	cnt = cmd_size;
	for(i=0;i<cnt;i++)
	{
		ack_buf[i] = rx_cmd[i];
	}
	return cnt;
}
/**************modbus tcp 0f�����봦��***********************************/
static uint8_t mbs_tcp_0f(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf)
{
	uint16_t reg_addr;
	uint16_t reg_num;
//	uint8_t  do_temp;
	
	if(modbus_rtu_0F == DISABLE)    //��֧�ָù�����
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_CMD);
	}
	
	reg_addr  = BEBuf2Uint16(&rx_cmd[8]);
	reg_num   = BEBuf2Uint16(&rx_cmd[10]);
	
	if(reg_addr + reg_num > 8)
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_VALUE);
	}
	for(uint8_t i = 0;i < reg_num; i++)
	{
//		do_temp = rx_cmd[7 + 6 + i / 8];
//		do_temp = (do_temp >> (i % 8)) & 0x01;
//		do_write(i + reg_addr, do_temp);
	}
	
	memcpy(ack_buf, rx_cmd, 12);
	
	ack_buf[5] = 6;
	
	return 12;
}
/**************modbus tcp 10�����봦��***********************************/
static uint8_t mbs_tcp_10(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf)
{
	
	uint16_t reg_addr;
	uint16_t reg_num;
	
	if(modbus_rtu_10 == DISABLE)    //��֧�ָù�����
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_CMD);
	}
	
	reg_addr  = BEBuf2Uint16(&rx_cmd[8]);
	reg_num   = BEBuf2Uint16(&rx_cmd[10]);
	if(reg_addr < modbus_start_add || reg_addr > (modbus_start_add+modbus_buf_num))     //��ʼ��ַ������ַ��Χ
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_REG_ADDR);
	}
	else if((reg_addr + reg_num-modbus_start_add) > modbus_buf_num)   //��ȡ�����򳬳���Χ
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_VALUE);
	}	
	reg_addr *= 2;
	for(uint8_t i = 0; i < rx_cmd[6 + 6]; i++)
	{
		count_value_reg[reg_addr+ i] = rx_cmd[7 + 6 + i];
	}
	
//	float limit = 0;
//	for(uint8_t ch = 0; ch < 2; ch++)
//	{
//		limit = BEBuf2Float(&count_value_reg[(8 + ch * 2) * 2]);
//		if(system_get_dac_type(ch) == TYPE_I)
//		{
//			if(limit > 20)
//			{
//				limit = 20.0f;
//			}
//			else if(limit < 0)
//			{
//				limit = 0;
//			}
//		}
//		else
//		{
//			if(limit > 10)
//			{
//				limit = 10;
//			}
//			else if(limit < 0)
//			{
//				limit = 0;
//			}
//		}
//		mbs_write_reg_float(&limit, 0x03, 8 + ch * 2);
//	}
//	
//	di_set_cnt(CH1, BEBuf2Uint32(&count_value_reg[0]));
//	di_set_cnt(CH2, BEBuf2Uint32(&count_value_reg[4]));
//	di_set_freq(CH1, BEBuf2Uint32(&count_value_reg[8]));
//	di_set_freq(CH2, BEBuf2Uint32(&count_value_reg[12]));
	
	memcpy(ack_buf, rx_cmd, 12);
	
	ack_buf[5] = 6;
	
	return 12;
}
/**************modbus rtu 01�����봦��***********************************/
/*01������Ϊ���̵������*/
static uint8_t mbs_rtu_01(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf)
{
	uint16_t reg, read_cnt, cnt, ack_size, crc;

	if(modbus_rtu_01 == DISABLE)    //��֧�ָĹ�����
	{
		return mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_CMD);
	}
	
	ack_buf[0] = rx_cmd[0];
	ack_buf[1] = rx_cmd[1];

	reg = BEBuf2Uint16(&rx_cmd[2]);
	read_cnt = BEBuf2Uint16(&rx_cmd[4]);

	if(reg < modbus_start_add || reg > (modbus_start_add+modbus_buf_num))     //��ʼ��ַ������ַ��Χ
	{
		return mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_REG_ADDR);
	}
	else if((reg + read_cnt-modbus_start_add) > modbus_buf_num)   //��ȡ�����򳬳���Χ
	{
		return mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_VALUE);
	}

	ack_size = (read_cnt + 7) / 8;
	ack_buf[2] = ack_size;


	/* �Ӹ�λ��ȡ����λ */
	reg = read_cnt + reg - 1;
	for(cnt = 0; cnt < read_cnt; cnt++)
	{
//		ack_buf[3] <<= 1;
//		ack_buf[3] |= do_read(reg--);
	}
	
	crc = crc_16(ack_buf, ack_size + 3);
	ack_buf[ack_size + 3] = crc >> 8;
	ack_buf[ack_size + 4] = crc;

	return ack_buf[2] + 5;
}
/**************modbus rtu 02�����봦��***********************************/
/* ������������ */
static uint8_t mbs_rtu_02(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf)
{
	uint16_t reg, read_cnt, cnt, ack_size, crc;

	if(modbus_rtu_02 == DISABLE)    //��֧�ָĹ�����
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_CMD);
	}
	ack_buf[0] = rx_cmd[0];
	ack_buf[1] = rx_cmd[1];

	reg = BEBuf2Uint16(&rx_cmd[2]);
	read_cnt = BEBuf2Uint16(&rx_cmd[4]);

	if((reg + read_cnt) > 8)
	{
		return mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_VALUE);
	}

	ack_size = (read_cnt + 7) / 8;
	ack_buf[2] = ack_size;


	/* �Ӹ�λ��ȡ����λ */
	reg = read_cnt + reg - 1;
	for(cnt = 0; cnt < read_cnt; cnt++)
	{
//		ack_buf[3] <<= 1;
//		ack_buf[3] |= di_read(reg--);
	}
	
	crc = crc_16(ack_buf, ack_size + 3);
	ack_buf[ack_size + 3] = crc >> 8;
	ack_buf[ack_size + 4] = crc;

	return ack_buf[2] + 5;
}
/**************modbus rtu 03�����봦��***********************************/
/* ��ȡ03�Ĵ���,����ֵ��Ƶ�ʺ�dac */
static uint8_t mbs_rtu_03(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf)
{
	uint16_t reg, value_num, cnt, crc;
	
	if(modbus_rtu_03 == DISABLE)    //��֧�ָù�����
	{
		return mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_CMD);
	}
	ack_buf[0] = rx_cmd[0];
	ack_buf[1] = rx_cmd[1];
	
	reg = BEBuf2Uint16(&rx_cmd[2]);
	value_num = BEBuf2Uint16(&rx_cmd[4]);

	if(reg < modbus_start_add || reg > (modbus_start_add+modbus_buf_num))     //��ʼ��ַ������ַ��Χ
	{
		return mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_REG_ADDR);
	}
	else if((reg + value_num-modbus_start_add) > modbus_buf_num)   //��ȡ�����򳬳���Χ
	{
		return mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_VALUE);
	}

	value_num *= 2;
	ack_buf[2] = value_num;
	for(cnt = 0; cnt < value_num; cnt++)
	{
		ack_buf[3 + cnt] = count_value_reg[cnt + reg * 2];
	}

	crc = crc_16(ack_buf, value_num + 3);
	ack_buf[value_num + 3] = crc >> 8;
	ack_buf[value_num + 4] = crc;

	return value_num + 5;
}
/**************modbus rtu 04�����봦��***********************************/
/* ��ȡģ���� */
static uint8_t mbs_rtu_04(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf)
{
	uint16_t reg, value_num, cnt, crc;

	if(modbus_rtu_04 == DISABLE)    //��֧�ָù�����
	{
		return mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_CMD);
	}
	ack_buf[0] = rx_cmd[0];
	ack_buf[1] = rx_cmd[1];

	reg = BEBuf2Uint16(&rx_cmd[2]);
	value_num = BEBuf2Uint16(&rx_cmd[4]);

	if(reg < modbus_start_add || reg > (modbus_start_add+modbus_buf_num))     //��ʼ��ַ������ַ��Χ
	{
		return mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_REG_ADDR);
	}
	else if((reg + value_num-modbus_start_add) > modbus_buf_num)   //��ȡ�����򳬳���Χ
	{
		return mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_VALUE);
	}

	value_num *= 2;

	for(cnt = 0; cnt < value_num; cnt++)
	{
//		ack_buf[3 + cnt] = analog_value_reg[cnt + reg * 2];
	}
	ack_buf[2] = value_num;

	crc = crc_16(ack_buf, value_num + 3);
	ack_buf[value_num + 3] = crc >> 8;
	ack_buf[value_num + 4] = crc;

	return value_num + 5;
}
/**************modbus rtu 05�����봦��***********************************/
static uint8_t mbs_rtu_05(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf)
{
	uint16_t reg;

	if(modbus_rtu_05 == DISABLE)    //��֧�ָù�����
	{
		return mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_CMD);
	}
	reg = BEBuf2Uint16(&rx_cmd[2]);

	if(reg > 4)
	{
		return mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_VALUE); 
	}

	/* �����̵��� */
//	do_write(reg, rx_cmd[4]);

	memcpy(ack_buf, rx_cmd, cmd_size);

	return cmd_size;
	
}
/**************modbus rtu 06�����봦��***********************************/
static uint8_t mbs_rtu_06(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf)
{
	uint16_t reg, cnt;
	uint8_t i;
	
	if(modbus_rtu_06 == DISABLE)    //��֧�ָù�����
	{
		return mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_CMD);
	}
	reg = BEBuf2Uint16(&rx_cmd[2]);      //�Ĵ�����ַ
	if(reg < modbus_start_add || reg > (modbus_start_add+modbus_buf_num))     //��ʼ��ַ������ַ��Χ
	{
		return mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_REG_ADDR);
	}
	count_value_reg[reg*2] = rx_cmd[4];
	count_value_reg[reg*2+1] = rx_cmd[5];
	
	cnt = cmd_size;
	for(i=0;i<cnt;i++)
	{
		ack_buf[i] = rx_cmd[i];
	}
	return cnt;
	
}
/**************modbus rtu 0f�����봦��***********************************/
static uint8_t mbs_rtu_0f(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf)
{
	uint16_t reg_addr;
	uint16_t reg_num;
	uint8_t  do_temp;
	
	if(modbus_rtu_0F == DISABLE)    //��֧�ָù�����
	{
		return mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_CMD);
	}
	reg_addr  = BEBuf2Uint16(&rx_cmd[2]);
	reg_num   = BEBuf2Uint16(&rx_cmd[4]);
	
	if(reg_addr + reg_num > 8)
	{
		return mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_VALUE);
	}
	for(uint8_t i = 0;i < reg_num; i++)
	{
		do_temp = rx_cmd[7 + i / 8];
		do_temp = (do_temp >> (i % 8)) & 0x01;
//		do_write(reg_addr + i, do_temp);
	}
	memcpy(ack_buf, rx_cmd, 6);
	
	uint16_t crc = crc_16(ack_buf, 6);

	ack_buf[6] = crc >> 8;
	ack_buf[7] = crc;
	
	return 8;
}
/**************modbus rtu 10�����봦��***********************************/
static uint8_t mbs_rtu_10(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf)
{
	uint16_t reg_addr;
	uint16_t reg_num;
	
	if(modbus_rtu_10 == DISABLE)    //��֧�ָù�����
	{
		return mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_CMD);
	}
	reg_addr  = BEBuf2Uint16(&rx_cmd[2]);
	reg_num   = BEBuf2Uint16(&rx_cmd[4]);
	if(reg_addr < modbus_start_add || reg_addr > (modbus_start_add+modbus_buf_num))     //��ʼ��ַ������ַ��Χ
	{
		return mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_REG_ADDR);
	}
	else if((reg_addr + reg_num-modbus_start_add) > modbus_buf_num)   //��ȡ�����򳬳���Χ
	{
		return mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_VALUE);
	}	
	reg_addr *= 2;
	for(uint8_t i = 0; i < rx_cmd[6]; i++)
	{
		count_value_reg[reg_addr+ i] = rx_cmd[7 + i];
	}
	
	memcpy(ack_buf, rx_cmd, 6);
	uint16_t crc = crc_16(ack_buf, 6);

	ack_buf[6] = crc >> 8;
	ack_buf[7] = crc;
	
	return 8;
}
/**************modbus rtu ��������ദ��***********************************/
uint8_t mbrtu_proc(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf)
{
	uint16_t ack_size = 0;
	if(crc_16(rx_cmd, cmd_size) != 0)
	{
		return 0;
	}
	if(device_info.save.modbus_addr == rx_cmd[0])
	{
		switch(rx_cmd[1])
		{
			case 0x01:
				ack_size = mbs_rtu_01(rx_cmd, cmd_size, ack_buf);break;

			case 0x02:
				ack_size = mbs_rtu_02(rx_cmd, cmd_size, ack_buf);break;

			case 0x03:
				ack_size = mbs_rtu_03(rx_cmd, cmd_size, ack_buf);break;

			case 0x04:
				ack_size = mbs_rtu_04(rx_cmd, cmd_size, ack_buf);break;
			
			case 0x05:
				ack_size = mbs_rtu_05(rx_cmd, cmd_size, ack_buf);break;
			
			case 0x06:
				ack_size = mbs_rtu_06(rx_cmd, cmd_size, ack_buf);break;
			
			case 0x0f:
				ack_size = mbs_rtu_0f(rx_cmd, cmd_size, ack_buf);break;
			
			case 0x10:
				ack_size = mbs_rtu_10(rx_cmd, cmd_size, ack_buf);break;

			default:
				ack_size = mbs_rtu_send_ack_err(rx_cmd, ack_buf, RSP_ERR_CMD);break;
		}
	}
	return ack_size;
}
/**************modbus tcp ��������ദ��***********************************/
uint8_t mbtcp_proc(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf)
{
	uint16_t ack_size = 0;

	switch(rx_cmd[7])
	{
		case 0x01:
			ack_size = mbs_tcp_01(rx_cmd, cmd_size, ack_buf);break;

		case 0x02:
			ack_size = mbs_tcp_02(rx_cmd, cmd_size, ack_buf);break;

		case 0x03:
			ack_size = mbs_tcp_03(rx_cmd, cmd_size, ack_buf);break;

		case 0x04:
			ack_size = mbs_tcp_04(rx_cmd, cmd_size, ack_buf);break;
		
		case 0x05:
			ack_size = mbs_tcp_05(rx_cmd, cmd_size, ack_buf);break;
		
		case 0x06:
			ack_size = mbs_tcp_06(rx_cmd, cmd_size, ack_buf);break;
		
		case 0x0f:
			ack_size =  mbs_tcp_0f(rx_cmd, cmd_size, ack_buf);break;
		
		case 0x10:
			ack_size = mbs_tcp_10(rx_cmd, cmd_size, ack_buf);break;

		default:
			ack_size = mbs_tcp_send_ack_err(rx_cmd, ack_buf, RSP_ERR_CMD);break;
	}
	return ack_size;
}
/**************�豸��������modbus����***********************************/
/* 
0000         0002     		 0004        0006          0008       000A 
sensor1data  sensor2data   sensor1_up  sensor1_down  sensor1_f  sensor1_hc  
000C        000E          0010       0012        0014          0015
sensor2_up  sensor2_down  sensor2_f  sensor2_hc  sensor1_unit  sensor2_unit
0016         0017  0018       0019       001A    001B              001C
modbus_addr  baud  data_bits  stop_bits  parity  eliminate_signal  sensor_clear_flag
*/
void device_data_to_modbusbuf(void)
{
//	uint8_t i=0;
	union de_data   //����������
	{
		uint8_t data[4];
		float value;
	};
//	union de_data d_data;
/*
  d_data.value = MPAbuf.sensor1data;
  count_value_reg[i++] = d_data.data[3];
	count_value_reg[i++] = d_data.data[2];
	count_value_reg[i++] = d_data.data[1];
	count_value_reg[i++] = d_data.data[0];
	
	d_data.value = MPAbuf.sensor2data;
  count_value_reg[i++] = d_data.data[3];
	count_value_reg[i++] = d_data.data[2];
	count_value_reg[i++] = d_data.data[1];
	count_value_reg[i++] = d_data.data[0];
	
	d_data.value = device_info.save.sensor1_up;
  count_value_reg[i++] = d_data.data[3];
	count_value_reg[i++] = d_data.data[2];
	count_value_reg[i++] = d_data.data[1];
	count_value_reg[i++] = d_data.data[0];
	
	d_data.value = device_info.save.sensor1_down;
  count_value_reg[i++] = d_data.data[3];
	count_value_reg[i++] = d_data.data[2];
	count_value_reg[i++] = d_data.data[1];
	count_value_reg[i++] = d_data.data[0];
	
	d_data.value = device_info.save.sensor1_f;
  count_value_reg[i++] = d_data.data[3];
	count_value_reg[i++] = d_data.data[2];
	count_value_reg[i++] = d_data.data[1];
	count_value_reg[i++] = d_data.data[0];
	
	d_data.value = device_info.save.sensor1_hc;
  count_value_reg[i++] = d_data.data[3];
	count_value_reg[i++] = d_data.data[2];
	count_value_reg[i++] = d_data.data[1];
	count_value_reg[i++] = d_data.data[0];
	
	d_data.value = device_info.save.sensor2_up;
  count_value_reg[i++] = d_data.data[3];
	count_value_reg[i++] = d_data.data[2];
	count_value_reg[i++] = d_data.data[1];
	count_value_reg[i++] = d_data.data[0];
	
	d_data.value = device_info.save.sensor2_down;
  count_value_reg[i++] = d_data.data[3];
	count_value_reg[i++] = d_data.data[2];
	count_value_reg[i++] = d_data.data[1];
	count_value_reg[i++] = d_data.data[0];
	
	d_data.value = device_info.save.sensor2_f;
  count_value_reg[i++] = d_data.data[3];
	count_value_reg[i++] = d_data.data[2];
	count_value_reg[i++] = d_data.data[1];
	count_value_reg[i++] = d_data.data[0];
	
	d_data.value = device_info.save.sensor2_hc;
  count_value_reg[i++] = d_data.data[3];
	count_value_reg[i++] = d_data.data[2];
	count_value_reg[i++] = d_data.data[1];
	count_value_reg[i++] = d_data.data[0];
	
	count_value_reg[i++] = device_info.save.sensor1_unit;
	count_value_reg[i++] = device_info.save.sensor2_unit;
	count_value_reg[i++] = device_info.save.modbus_addr;
	
	count_value_reg[i++] = device_info.save.baud>>8;
	count_value_reg[i++] = device_info.save.baud;
	
	count_value_reg[i++] = 0x00;
	switch(device_info.save.data_bits)
	{
		case LL_UART_DATAWIDTH_8B: count_value_reg[i++] = 0x08;break;
		case LL_UART_DATAWIDTH_9B: count_value_reg[i++] = 0x09;break;
		default : count_value_reg[i++] = 0x08;break;
	}
	
	count_value_reg[i++] = 0x00;
	switch(device_info.save.stop_bits)
	{
		case LL_UART_STOPBITS_1: count_value_reg[i++] = 0x01;break;
		case LL_UART_STOPBITS_2: count_value_reg[i++] = 0x02;break;
		default : count_value_reg[i++] = 0x01;break;
	}
	
	count_value_reg[i++] = 0x00;
	switch(device_info.save.parity)
	{
		case LL_UART_PARITY_NONE: count_value_reg[i++] = 0x00;break;
		case LL_UART_PARITY_ODD:  count_value_reg[i++] = 0x01;break;
		case LL_UART_PARITY_EVEN: count_value_reg[i++] = 0x02;break;
		default : count_value_reg[i++] = 0x00;break;
	}
	
	count_value_reg[i++] = 0x00;
	count_value_reg[i++] = device_info.save.eliminate_signal;
	
	count_value_reg[i++] = 0x00;
	count_value_reg[i++] = device_info.save.sensor_clear_flag;*/
}
/**************modbus�������ݴ����豸***********************************/
void modbusbuf_to_device_data(void)
{
//	uint8_t i;
	union de_data   //����������
	{
		uint8_t data[4];
		float value;
	};
//	union de_data d_data;
//	i = 8;	
/*	
  d_data.data[3] = count_value_reg[i++];
	d_data.data[2] = count_value_reg[i++];
	d_data.data[1] = count_value_reg[i++];
	d_data.data[0] = count_value_reg[i++];
	device_info.save.sensor1_up = d_data.value;
	
	d_data.data[3] = count_value_reg[i++];
	d_data.data[2] = count_value_reg[i++];
	d_data.data[1] = count_value_reg[i++];
	d_data.data[0] = count_value_reg[i++];
	device_info.save.sensor1_down =  d_data.value;
	
	d_data.data[3] = count_value_reg[i++];
	d_data.data[2] = count_value_reg[i++];
	d_data.data[1] = count_value_reg[i++];
	d_data.data[0] = count_value_reg[i++];
	device_info.save.sensor1_f = d_data.value;
	
	d_data.data[3] = count_value_reg[i++];
	d_data.data[2] = count_value_reg[i++];
	d_data.data[1] = count_value_reg[i++];
	d_data.data[0] = count_value_reg[i++];
	device_info.save.sensor1_hc = d_data.value;
	
	d_data.data[3] = count_value_reg[i++];
	d_data.data[2] = count_value_reg[i++];
	d_data.data[1] = count_value_reg[i++];
	d_data.data[0] = count_value_reg[i++];
	device_info.save.sensor2_up = d_data.value;
	
	d_data.data[3] = count_value_reg[i++];
	d_data.data[2] = count_value_reg[i++];
	d_data.data[1] = count_value_reg[i++];
	d_data.data[0] = count_value_reg[i++];
	device_info.save.sensor2_down = d_data.value;
	
	d_data.data[3] = count_value_reg[i++];
	d_data.data[2] = count_value_reg[i++];
	d_data.data[1] = count_value_reg[i++];
	d_data.data[0] = count_value_reg[i++];
	device_info.save.sensor2_f = d_data.value;
	
	d_data.data[3] = count_value_reg[i++];
	d_data.data[2] = count_value_reg[i++];
	d_data.data[1] = count_value_reg[i++];
	d_data.data[0] = count_value_reg[i++];
	device_info.save.sensor2_hc = d_data.value;
	
	device_info.save.sensor1_unit = count_value_reg[i++];
	device_info.save.sensor2_unit = count_value_reg[i++];
	device_info.save.modbus_addr = count_value_reg[i++];
	
	device_info.save.baud = count_value_reg[i++];
	device_info.save.baud<<=8;
	device_info.save.baud |= count_value_reg[i++];
	
	i++;
	device_info.save.data_bits = count_value_reg[i++];
	
	i++;
	device_info.save.stop_bits = count_value_reg[i++];
	
	i++;
	device_info.save.parity = count_value_reg[i++];
		
	i++;
	device_info.save.eliminate_signal = count_value_reg[i++];
	
	i++;
	device_info.save.sensor_clear_flag = count_value_reg[i++];*/
}
/***************************************************/

/***************************************************/





