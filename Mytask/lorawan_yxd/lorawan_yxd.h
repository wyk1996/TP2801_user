#ifndef  _lorawan_yxd_H_
#define  _lorawan_yxd_H_
#include "main.h"

//band
#define EU433    0
#define CN470    1
#define EU868    2
#define US915    3
#define AU915    5
#define IN865    8


//uint8_t *APPKEY = (uint8_t *)device_info.save.srv_apn;                //Ӧ�ø���Կ        srv_apn[24];   //APN
//uint8_t *NWKSKEY = (uint8_t *)device_info.save.srv_apn_user_name;     //����Ự��Կ      srv_apn_user_name[24];
//uint8_t *APPSKEY = (uint8_t *)device_info.save.srv_apn_user_pass;     //Ӧ�ûỰ��Կ      srv_apn_user_pass[24];

typedef struct 
{
//	uint8_t DEVEUI[20];   //�豸EUI               srv_login[64];     //��¼��
//	uint8_t APPEUI[20];   //Ӧ��EUI               srv_UserName[24];   //MQTT�û���
//	uint8_t DEVADDR[12];       //�豸�̵�ַ       srv_addr[24];   //��������ַ
//	uint8_t NETID[8];        //����ID             srv_Password[24];     //MQTT�û�����
	
	uint8_t CHGROUP;         //�ŵ���
	uint8_t ULDLMODE;        //������ͬ��Ƶ
	uint8_t JOINMODE;        //����ģʽ
	uint8_t CLASS;           //�豸����
	
	uint8_t STATUS;          //�豸״̬
	uint8_t BAND;            //����Ƶ��
	uint8_t APPPORT;         //���д���˿�
	uint8_t DATARATE;        //��������
	
	uint8_t TXPWR;            //���书��
	uint8_t NOP0;           //
	uint8_t ADR;             //��������Ӧ
	uint8_t NOP1;
}
lorawan_at;

extern lorawan_at lorawan;


void lorawan_readstate(void);
void lorawan_setstate(void);
void lorawan_work_statues(void);
uint8_t lorawan_init(void);
void task_lorawan(void);
#endif

