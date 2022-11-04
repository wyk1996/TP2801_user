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


//uint8_t *APPKEY = (uint8_t *)device_info.save.srv_apn;                //应用根秘钥        srv_apn[24];   //APN
//uint8_t *NWKSKEY = (uint8_t *)device_info.save.srv_apn_user_name;     //网络会话秘钥      srv_apn_user_name[24];
//uint8_t *APPSKEY = (uint8_t *)device_info.save.srv_apn_user_pass;     //应用会话秘钥      srv_apn_user_pass[24];

typedef struct 
{
//	uint8_t DEVEUI[20];   //设备EUI               srv_login[64];     //登录包
//	uint8_t APPEUI[20];   //应用EUI               srv_UserName[24];   //MQTT用户名
//	uint8_t DEVADDR[12];       //设备短地址       srv_addr[24];   //服务器地址
//	uint8_t NETID[8];        //网络ID             srv_Password[24];     //MQTT用户密码
	
	uint8_t CHGROUP;         //信道组
	uint8_t ULDLMODE;        //上下行同异频
	uint8_t JOINMODE;        //激活模式
	uint8_t CLASS;           //设备类型
	
	uint8_t STATUS;          //设备状态
	uint8_t BAND;            //地区频段
	uint8_t APPPORT;         //上行传输端口
	uint8_t DATARATE;        //数据速率
	
	uint8_t TXPWR;            //发射功率
	uint8_t NOP0;           //
	uint8_t ADR;             //速率自适应
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

