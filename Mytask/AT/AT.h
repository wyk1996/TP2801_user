#ifndef __AT_H
#define __AT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

char TEST_AT(uint8_t ATT);   //���AT����
char SET_ATW(void);     //AT&W
char SET_ATE(char ate);   //���û���
char SET_CFUN(char cfun);   //������������
char TEST_ATI(void);     //��ȡģ���ź�
char SET_IPR(char *ipr);   //���ò�����

char TEST_CGMR(void);      //��ѯ�汾����Ϣ

void GET_IMEI(char *imei,char *lei);     //��ȡIMEI
void GET_IMSI(char *imsi);     //��ȡIMSI
void GET_ICCID(char *iccid,char *lei);    //��ȡiccid

char TEST_CPIN(void);     //���SIM����ȡ״̬
char TEST_CGATT(uint8_t wait_count);    //������總��״̬
char TEST_CREG(uint8_t wait_count);     //�������ע��״̬1
char TEST_CEREG(uint8_t wait_count);     //�������ע��״̬2
uint8_t GET_CSQ(void);     //����ź�ǿ��
uint8_t GET_CESQ(void);    //����ź�����   BC26
uint16_t GET_CBC(void);    //���ģ�鹩���ѹ
char SET_QPOWD(void);    //ģ��ػ�
char SET_QRST(void);   //ģ������   BC26��
char SET_NRB(void);   //ģ��ָ������   BC95��
char SET_QBAND(char qband);    //������Ӫ����Ϣ  NBIOTģ��
char SET_QRELLOCK(void);        //AT������ٽ���   NBIOTģ��

void PCIE_PW_OPEN(void);    //��PICEģ���Դ
void PCIE_PW_CLOSE(void);    //�ر�PICEģ���Դ
//void TAB_RST(void);     //ģ�鰴������
	
uint8_t strstrlen(uint8_t *str1, uint8_t *str2, uint16_t len);  //�ַ����Ա� 
uint8_t check_recv2(uart_t *uart, uint8_t *recv_buf ,const char *str,const char *str2, uint16_t rd_size ,uint16_t wait_tick);
uint8_t check_recv3(uart_t *uart, uint8_t *recv_buf ,const char *str0,const char *str1,const char *str2, uint16_t rd_size ,uint16_t wait_tick);


#ifdef __cplusplus
}
#endif
#endif /*__ AT_H */
