#ifndef _DEVICE_H
#define _DEVICE_H
#include "main.h"

#define APP_UPDATE_FLAG     (1024 * 8)
#define DEVICE_INFO   (9 * 1024)      //FLASH���ݴ洢��ʼ��ַ

//#define sensorerr_num  60


#define recive_len   400   //���ڽ������鳤��
//#define recive_len1  64   //���ڽ������鳤��
//#define output_len   256   //���ڷ������鳤��
#define output_len  400   //���ڷ������鳤��

#define LCD_IT        (1 << 0)
#define SEN_IT        (1 << 1)
#define SLEE_IT       (1 << 2)
#define NET_IT        (1 << 3)
#define OFF_IT        (1 << 4)
#define lora_IT       (1 << 5)
#define loraset_IT    (1 << 6)

#define EVENT_OK      (1)
#define EVENT_RUN     (0)

//NB���������Ͷ���  sever_type
#define NB_tlink        0     
#define NB_onenet       1
#define NB_telecom      2
#define tlink_open      3
#define tlink_close     4
 //Ӳ���汾   tab_model  
#define TP280xVx_0           0x01
#define TP280xVx_0_P         0x02   

//���������� tab_sensor
#define One_wire          0x01    //TOPRIE ����������
#define D032              0x02    //ѹ��   ����
#define D033              0X03    //ѹ��   ����
#define D020              0X04    //����IICѹ��������40MPa
#define DS18B20           0x05    //

//One_wire  TOPRIE ����������  ����Ӧ������
#define TP2305V2          0x01    //��ʪ��
#define TP2302V2          0x02    //PT100
#define TP2360            0x03    //�������  
#define TP2305V3          0x04    //��ʪ��+¶��
#define TP2308            0x05    //��ȩ  
#define TP2303V1          0x06    //PT1000
#define TP2310            0X0A    //������̼  0-5000ppm
#define TP2311            0X0B    //�ȵ�ż
#define TP2312            0x0C    //ˮ�ʵ絼��
#define TP2313						0x0D    //��ʪ�ȴ���ѹ��������
#define TP2314						0x0E    //������̼������
#define TP2315            0x0F    //ˮ��������
#define TP2316            0x10    //pm2.5
#define TP2317            0x11    //TVOC
#define TP2318            0x12    //����
#define TP2319            0x13    //��ȼ����
#define TP2321_1          0x14    //ѹ��������  
#define TP2321            0x15    //Һλ������   
#define TP2322            0x16    //����  

//ͨѶģ��tab_module
#define NULL1             0x01   
#define BC26              0x02
#define BC20              0x03    //NB��GPS��λ
#define EC25              0x04
#define EC25_G            0x05    //4G��GPS��λ�汾
#define TP307V1_1_A       0x06
#define LORAWAN           0x07
#define M101							0x08
#define BC95							0x09

//����Э�� tab_protocol
#define NBIOT             0x01
#define MQTT              0x02
#define TCP               0x03
#define LORA              0x04
#define MODBUS            0x05
#define UDP               0x06
#define LORA_WAN          0x07

#define OFF            0x00
#define ON             0x01
#define CLR            0x02


//��ʾ��λ
#define UNIT_MPa       0x00
#define UNIT_KPa       0x01
#define UNIT_C         0X02    //��
#define UNIT_F         0X03
#define UNIT_0         0X04   // ����ʾ��λ

//������Ӫ��device_operator
#define CHN_Telecom       0x01   //�й�����
#define CHN_Mobile        0x02   //�й��ƶ�
#define CHN_Unicom        0x03   //�й���ͨ
#define ATO_Search        0x04   //�Զ�����

#define alarm_ok             0        //��������������
#define alarm_channel_err    (1<<0)    //��������ȡ����
#define alarm_channel_up     (1<<1)    //���������ݳ�������
#define alarm_channel_down   (1<<2)    //���������ݳ�������
#define alarm_channel_f      (1<<3)    //���������ݳ���������ֵ

//�������ɽ���9ֻ��ǰ3ֻΪ�̶�PT100����������Ӧ���ݴ洢λ0-2����6ֻΪ�ɱ�IIC���ߵ����ߴ���������Ӧ���ݴ洢λΪ3-14

typedef struct 
{ 
	/* ��������*/
	uint8_t ch_status[16];      //��Ӧ16��ͨ���ı���״̬
	uint8_t flasherror;
}
alarm;

typedef enum 
{
	main_SAVE = 0,
	main_REST,
	main_CLEAR_HIST,	
	main_MODEL,	       
	main_MODULE,
	main_BULID_TIME,
	main_BULID_Version,     //4
	main_end = 7
}main_CORE;

typedef enum 
{	
	new_local_set=0,        //0	
	new_DZ,
	new_battery,
	new_sensor_err_num,		
	new_PORT_NUM, 
	new_reset,
	
	new_CHANNEL_CLB,    //ͨ��1У׼
	new_CHANNEL1_CLB0,   //ͨ��1У׼���ֵ
	new_CHANNEL1_CLB1,   //ͨ��1У׼����ֵ
//	new_CHANNEL2_CLB,    //ͨ��2У׼
	new_CHANNEL2_CLB0,   //ͨ��2У׼���ֵ
	new_CHANNEL2_CLB1,   //ͨ��2У׼����ֵ
//	new_CHANNEL3_CLB,    //ͨ��3У׼
	new_CHANNEL3_CLB0,   //ͨ��3У׼���ֵ
	new_CHANNEL3_CLB1,   //ͨ��3У׼����ֵ
	
	new_PORT0_TYPE,
	new_CHANNEL4_UNIT_P,	     
	new_CHANNEL4_RANGE_H,    //10
	new_CHANNEL4_RANGE_L,
	new_CHANNEL5_UNIT_P,
	new_CHANNEL5_RANGE_H,
	new_CHANNEL5_RANGE_L,
	
	new_PORT1_TYPE,       //15
	new_CHANNEL6_UNIT_P,
	new_CHANNEL6_RANGE_H,
	new_CHANNEL6_RANGE_L,
	new_CHANNEL7_UNIT_P,
	new_CHANNEL7_RANGE_H,    //20
	new_CHANNEL7_RANGE_L,

	new_PORT2_TYPE,
	new_CHANNEL8_UNIT_P,
	new_CHANNEL8_RANGE_H,     
	new_CHANNEL8_RANGE_L,    //25
	new_CHANNEL9_UNIT_P,
	new_CHANNEL9_RANGE_H,
	new_CHANNEL9_RANGE_L,
	
	new_PORT3_TYPE,
	new_CHANNEL10_UNIT_P,      //30
	new_CHANNEL10_RANGE_H,
	new_CHANNEL10_RANGE_L,
	new_CHANNEL11_UNIT_P,
	new_CHANNEL11_RANGE_H,
	new_CHANNEL11_RANGE_L,      //35
	
	new_PORT4_TYPE,
	new_CHANNEL12_UNIT_P,
	new_CHANNEL12_RANGE_H,
	new_CHANNEL12_RANGE_L,       
	new_CHANNEL13_UNIT_P,       //40
	new_CHANNEL13_RANGE_H,
	new_CHANNEL13_RANGE_L,
	
	new_PORT5_TYPE,
	new_CHANNEL14_UNIT_P,
	new_CHANNEL14_RANGE_H,     //45
	new_CHANNEL14_RANGE_L,
	new_CHANNEL15_UNIT_P,
	new_CHANNEL15_RANGE_H,
	new_CHANNEL15_RANGE_L,
	
	new_end   = 50
}
new_CORE;

typedef enum 
{
	NULL_CHANNEL_DATA=0,
	NULL_TIME,
	NULL_CHANNEL0_DATA,
	NULL_CHANNEL1_DATA,
	NULL_CHANNEL2_DATA,
	NULL_CHANNEL3_DATA,
	NULL_CHANNEL4_DATA,    //
	NULL_CHANNEL5_DATA,
	NULL_CHANNEL6_DATA,
	NULL_CHANNEL7_DATA,
	NULL_CHANNEL8_DATA,
	NULL_CHANNEL9_DATA,     //
	NULL_CHANNEL10_DATA,
	NULL_CHANNEL11_DATA,
	NULL_CHANNEL12_DATA,
	NULL_CHANNEL13_DATA,
	NULL_CHANNEL14_DATA,    //
	NULL_CHANNEL15_DATA,
	
	NULL_CHANNEL_SET,
	NULL_SAM_TIME,
	NULL_DIP_NUM,
	NULL_eliminate_signal,
	NULL_device_restart_num,
	NULL_vbat_reset,
	
	NULL_CHANNEL1_SENSOR_NAME,      //ͨ������������
	NULL_CHANNEL1_UP,           //ͨ������
	NULL_CHANNEL1_DOWN,       //ͨ������     //20
	NULL_CHANNEL1_F,         //ͨ��������ֵ
	NULL_CHANNEL1_HC,        //ͨ�������ز�
	NULL_CHANNEL1_C,         //ͨ������ֵ
	
	NULL_CHANNEL2_SENSOR_NAME,      
	NULL_CHANNEL2_UP,           //
	NULL_CHANNEL2_DOWN,       
	NULL_CHANNEL2_F,         
	NULL_CHANNEL2_HC,        
	NULL_CHANNEL2_C,   

	NULL_CHANNEL3_SENSOR_NAME,    //    
	NULL_CHANNEL3_UP,           //
	NULL_CHANNEL3_DOWN,       
	NULL_CHANNEL3_F,         
	NULL_CHANNEL3_HC,        
	NULL_CHANNEL3_C,     //35
	
	NULL_PORT0_SENSOR_NAME,      
	NULL_CHANNEL4_UP,           //
	NULL_CHANNEL4_DOWN,       
	NULL_CHANNEL4_F,         
	NULL_CHANNEL4_HC,        
	NULL_CHANNEL4_C,      
	NULL_CHANNEL5_UP,           //
	NULL_CHANNEL5_DOWN,       
	NULL_CHANNEL5_F,         
	NULL_CHANNEL5_HC,        
	NULL_CHANNEL5_C,
	
	NULL_PORT1_SENSOR_NAME,      
	NULL_CHANNEL6_UP,           //
	NULL_CHANNEL6_DOWN,       
	NULL_CHANNEL6_F,         
	NULL_CHANNEL6_HC,        
	NULL_CHANNEL6_C,	      
	NULL_CHANNEL7_UP,           //
	NULL_CHANNEL7_DOWN,       
	NULL_CHANNEL7_F,         
	NULL_CHANNEL7_HC,        
	NULL_CHANNEL7_C,
	
	NULL_PORT2_SENSOR_NAME,      
	NULL_CHANNEL8_UP,           //
	NULL_CHANNEL8_DOWN,       
	NULL_CHANNEL8_F,         
	NULL_CHANNEL8_HC,        
	NULL_CHANNEL8_C,	     
	NULL_CHANNEL9_UP,           //
	NULL_CHANNEL9_DOWN,       
	NULL_CHANNEL9_F,         
	NULL_CHANNEL9_HC,        
	NULL_CHANNEL9_C,
	
	NULL_PORT3_SENSOR_NAME,      
	NULL_CHANNEL10_UP,           //
	NULL_CHANNEL10_DOWN,       
	NULL_CHANNEL10_F,         
	NULL_CHANNEL10_HC,        
	NULL_CHANNEL10_C,      
	NULL_CHANNEL11_UP,           //
	NULL_CHANNEL11_DOWN,       
	NULL_CHANNEL11_F,         
	NULL_CHANNEL11_HC,        
	NULL_CHANNEL11_C,	
	
	NULL_PORT4_SENSOR_NAME,      
	NULL_CHANNEL12_UP,           //
	NULL_CHANNEL12_DOWN,       
	NULL_CHANNEL12_F,         
	NULL_CHANNEL12_HC,        
	NULL_CHANNEL12_C,      
	NULL_CHANNEL13_UP,           //
	NULL_CHANNEL13_DOWN,       
	NULL_CHANNEL13_F,         
	NULL_CHANNEL13_HC,        
	NULL_CHANNEL13_C,	
	
	NULL_PORT5_SENSOR_NAME,      
	NULL_CHANNEL14_UP,           //
	NULL_CHANNEL14_DOWN,       
	NULL_CHANNEL14_F,         
	NULL_CHANNEL14_HC,        
	NULL_CHANNEL14_C,	      
	NULL_CHANNEL15_UP,           //
	NULL_CHANNEL15_DOWN,       
	NULL_CHANNEL15_F,         
	NULL_CHANNEL15_HC,        
	NULL_CHANNEL15_C,
}
NULL_CORE;
//NB
typedef enum 
{	
	nb_local_read = 0,
	nb_IMEI,
	nb_IMSI,
	nb_ICCID,
	nb_CSQ,
	nb_CESQ,       //5
	
	nb_SRV_SETT,         //
	nb_SRV_OPERATOR,
	nb_SRV_PROTOCOL,
	nb_SRV_TYPE,
	nb_SRV_ADDR,     //10
	nb_SRV_PORT,
	nb_SRV_LOGIN,
	nb_SIM_PIN,
	nb_SIM_PUK,
	nb_APN,           //15
	nb_APN_USER_NAME,
	nb_APN_USER_PASS,
	mqtt_SRV_USERNAME,
	mqtt_SRV_PASSWORD,
	mqtt_SRV_TOPIC,     //20
	mqtt_SRV_SUBSCRIPTION,
		
	nb_SYS_SETT,	
	nb_SRV_TIME,	
  nb_device_dabao,
	nb_zhengdian,     //25
	nb_device_BJ_pinlv_stu,
	nb_device_BJ_pinlv_time,
	nb_gps_stu,
	nb_gps_E,
	nb_gps_N,
	
	nb_end   = 31
}
nb_CORE;

//lorawan
typedef enum 
{
	lorawan_SAVE = 0,
	lorawan_REST,
	lorawan_CLEAR_HIST,
	
	lorawan_local_read,
	lorawan_MODEL,
	lorawan_SENSOR,
	lorawan_NETID,
	lorawan_STATUS,
	lorawan_CSQ,
	
	lorawan_S1_value,
	lorawan_S2_value,
	lorawan_mpu_gx,
	lorawan_mpu_gy,
	lorawan_mpu_gz,
	lorawan_mpu_ax,
	lorawan_mpu_ay,
	lorawan_mpu_az,
	lorawan_new_time,
	
	lorawan_SRV_SETT,
	lorawan_SRV_DEVEUI,
	lorawan_SRV_APPEUI,
	lorawan_SRV_DEVADDR,
	lorawan_SRV_APPKEY,
	lorawan_SRV_NWKSKEY,
	lorawan_SRV_APPSKEY,
	lorawan_SRV_CHGROUP,
	lorawan_SRV_ULDLMODE,
	lorawan_SRV_JOINMODE,
	lorawan_SRV_CLASS,
	lorawan_SRV_BAND,
	lorawan_SRV_APPPORT,
	lorawan_SRV_DATARATE,
	lorawan_SRV_TXPWR,
	lorawan_SRV_ADR,
		
	lorawan_SYS_SETT,	
	lorawan_SRV_TIME,
	lorawan_SAM_TIME,
	lorawan_DIP_NUM,
  lorawan_device_dabao,
	lorawan_zhengdian,
	lorawan_device_mpu,
	lorawan_device_shock,
	lorawan_device_BJ_pinlv_stu,
	lorawan_device_BJ_pinlv_time,
	
	lorawan_S1_RANGE,
	lorawan_S2_RANGE,
	lorawan_S1_UNIT,
	lorawan_S2_UNIT,
	lorawan_eliminate_signal,
	lorawan_sensor_clear_f,
	lorawan_S1_CORRECT,
	lorawan_S2_CORRECT,

	lorawan_ALARM_SEET,
  lorawan_shock_alarm,
	lorawan_S1_UP,
	lorawan_S1_DOWN,
	lorawan_S2_UP,
	lorawan_S2_DOWN,

	lorawan_S1_fluctuate,
	lorawan_S2_fluctuate,
	
  lorawan_fluctuate_dowm_S1,
	lorawan_fluctuate_dowm_S2,
	
	lorawan_device_restart_num,
	lorawan_vbat_reset,
		
	lorawan_BULID_TIME,
	lorawan_BULID_Version,
	
	lorawan_LOG   = 255
}
lorawan_CORE;
//LORA
typedef enum 
{
	lora_SAVE = 0,
	lora_REST,
	lora_CLEAR_HIST,
	
	lora_local_read,
	lora_MODEL,
	lora_SENSOR,	
	lora_once,
	lora_rerty,
	lora_updata,
	lora_upack,
	lora_num,
	lora_slot,
	lora_time,
	lora_pwrtime,
	lora_vbat,
	lora_qsignal,
	
	lora_S1_value,
	lora_S2_value,
	lora_mpu_gx,
	lora_mpu_gy,
	lora_mpu_gz,
	lora_mpu_ax,
	lora_mpu_ay,
	lora_mpu_az,
	lora_new_time,
	
	lora_SRV_SETT,
	lora_SRV_NS,
	lora_SRV_NETID,
	lora_SRV_TX_POWER,
	lora_SRV_speed,
	lora_SRV_m_band,

	lora_SYS_SETT,
	
	lora_SRV_TIME,
	lora_SAM_TIME,
	lora_DIP_NUM,
	lora_device_mpu,
	lora_device_shock,
	lora_device_BJ_pinlv_stu,
	lora_device_BJ_pinlv_time,
	
  lora_S1_RANGE,
	lora_S2_RANGE,
	lora_S1_UNIT,
	lora_S2_UNIT,
	lora_eliminate_signal,
	lora_sensor_clear_f,
	lora_S1_CORRECT,
	lora_S2_CORRECT,

	lora_ALARM_SEET,
  lora_shock_alarm,
	lora_S1_UP,
	lora_S1_DOWN,
	lora_S2_UP,
	lora_S2_DOWN,

	lora_S1_fluctuate,
	lora_S2_fluctuate,
	
  lora_fluctuate_dowm_S1,
	lora_fluctuate_dowm_S2,
	
	lora_vbat_reset,
	
	lora_BULID_TIME,
	lora_BULID_Version,
	
	lora_LOG   = 255
}
lora_CORE;

typedef struct 
{ 
	/* �� �� ʱ �� �� �� ��*/
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t day;
	uint8_t week;
	uint8_t month;
	uint8_t year;
}
rtc_t;

//������Ϣ
typedef struct 
{
	uint8_t first_data[2];   //flash��֤��־λ
	uint16_t save_len;       //flash�洢����ռ�ÿռ�
	
	uint8_t srv_addr[24];   //��������ַ
	uint8_t srv_port[8];    //�˿ں�
	uint8_t srv_login[64];     //��¼��
	uint8_t srv_UserName[24];   //MQTT�û���
	uint8_t srv_Password[24];     //MQTT�û�����
	uint8_t srv_Topic[24];         //MQTTͷ�ļ�
	uint8_t srv_Subacription[24];    //MQTT������
	
	uint8_t srv_apn[24];   //APN
	uint8_t srv_apn_user_name[24];
	uint8_t srv_apn_user_pass[24];
	
	uint8_t srv_sim_pin[24];    //SIM��PIN��
	uint8_t srv_sim_puk[24];    //SIM��PUK��

//����һ����16��ͨ��(CH0-CH15)��CH0ͨ��Ϊ��ص�������;,��CH1-3ͨ����PT100���ݣ���Ӧ3·PT100�����ӿ�;����12��ͨ��ΪIIC���ߵ�����ͨ��������ÿ2��ͨ����Ӧһ��port�ӿڣ�
//�������ӿ�9��(port0-port8),����0-5ΪIIC���ߵ����߽ӿڣ�6-8ΪPT100�ӿ�
//
	float channel_up[16];      //ͨ��ֵ����
	float channel_down[16];    //ͨ��ֵ����
	float channel_f[16];       //ͨ��������ֵ
	float channel_hc[16];      //ͨ�������ظ���ֵ
	float channel_c[16];       //ͨ������ֵ
	
	int32_t channel_range_l[16];  //��������������   ��Ժ�12������
  int32_t channel_range_h[16];  //��������������   ��Ժ�12������
	uint8_t channel_unit_p[16];  //Ԥ�赥λ
	uint8_t channel_unit[16];    //��ʾ��λ
	
	uint8_t  port_sensor_type[6];      //�豸�ҽӴ���������
	uint8_t  port_one_wire_type[6];        //����Ӧ�����ߴ���������
	
	uint32_t battery_cap;      //�������	
		
	uint16_t srv_time;    //�����ϴ�ʱ����	
	uint16_t BJ_pinlv_time;   //���������ϱ�Ƶ��
	
	uint8_t device_operator;     //������Ӫ������   
	uint8_t  zhengdian;     //�����ر������ϱ����ܱ�־λ	
	uint8_t BJ_pinlv_stu;     //���������ϱ�Ƶ�ʸı�״̬λ
	uint8_t port_num;     //IIC�ӿڴ���������
	
	uint16_t  lora_id;      //lora��ַ
	uint8_t sever_type;    //NB_����������
	uint8_t nop1;
//	uint8_t sensor_clear_flag; 	//�������Զ�У0����  
	
	uint8_t eliminate_signal;	   //С�ź��г�����	    
	uint8_t device_dabao;  //�Ƿ�������ϴ���־	
	uint8_t tab_model;  // �豸�ͺ�
	uint8_t SENSOR_SAM_TIME;     //����������ʱ����  
	
	uint8_t tab_module;      //�豸ģ��
	uint8_t tab_protocol;      //�豸ͨѶЭ��	
	uint8_t modbus_addr;     //modbus��ַ
	uint8_t dip_num;          //С����λ��   Ĭ��4λ  
			
	uint32_t baud;           //modbus������
	uint32_t parity;         //modbusУ������
	uint32_t stop_bits;      //modbusֹͣλ
	uint32_t data_bits;      //modbus����λ
			
	uint8_t GPS_stu;
	uint8_t nop0;
	uint16_t nop4;      //δʹ��
	
	
	float pt100_100[3];
	float pt100_402[3];
	uint32_t nop11;      //δʹ��
	uint32_t nop12;      //δʹ��
	uint32_t nop13;      //δʹ��
	uint32_t nop14;      //δʹ��
	
  uint8_t DZ_NUM;      //���Ƴ�����
	uint8_t sensorerr_num;      //���������߻��ߴ����ض�����		
	uint16_t device_crc;  	
}
save_param_t;
/////�ܴ�С 0x11c
//������ֵ

//������Ϣ
typedef struct {	
	rtc_t data_time;
	save_param_t save;           /* ���浽eeprom���� */	 
		
//	uint8_t event_sensor;      //��������ȡ�ɹ���־
//	uint8_t sensor_err_num;    //��������ȡ�������
	
	uint8_t event_flash;   //flash���������־����ֹ��������
	uint8_t event_up;    //�����ϱ������־����ֹ��������
	uint8_t event_display;  //������ʾ�����־����ֹ��������
	
	uint8_t event_sensorread;   //��������ȡ�����־����ֹ��������
	uint8_t event_again;   //�豸�������߳����ϵ��־  
	uint8_t event_flash_again;    //�豸�������߳����ϵ�flash�洢��־λ	
	
	float channel_data[16];

	float channel_data_f[16];    //������һ�ִ��������ݣ����ڷ����ж�
	
	uint8_t first_work;    //ϵͳ��һ�����б�־λ
	uint8_t alarm_statue;    //�̵������״̬	
	uint8_t Params_sever;  //�Ϸ�ICCID ��һ������ ��  ����������ɷ��ͱ�־
	
	uint8_t alarming_flag;    //����������־λ  �ϱ��������
	alarm  alarm_flag_ID;           //���������¼�ID
	alarm  alarm_flag_STATUS;           //���������¼�STATUS
	uint8_t port_err_num[9];    //9��ͨ����������ȡʧ�ܴ�����־λ
	
	uint8_t debug_en;     //���ڴ�ӡ��־
	uint8_t device_en;   //usb �޸ı�־    0��ʾUSB�޸Ĺ�   1��ʾUSB���Ʊ���

	uint8_t sensor_sam_timer;   //�����ϴ���־λ

	uint16_t vabt_timer;    //��ص������¼�ʱ��	
	uint8_t gps_time;           // nbģ��gps��λʱ����   24Сʱһ��
	uint16_t wait_min;           /* �ȴ��ϱ�Min */
	uint8_t alarm_barck_flag;    //����������ʱ�������
	
	uint8_t gps_satues;       //gps��λ״ָ̬ʾ��־
	uint8_t modbus_read_en;     //modbus��ȡ����ʹ��
	uint8_t modbus_write_en;    //modbusд�����ʹ��
	
	
	uint8_t MCU_statues;          //MCU����״̬    0  ����״̬   1 �͹�������״̬   2 �ػ�״̬
	
	uint8_t djq_statue;         //�̵����������״̬��־
	uint8_t time_calib;         //ʱ��У׼��־
	
	uint8_t pt100_clb_sta;     //PT100У׼����
	
	float PT100_K[3];      //pt100ͨ��У׼Kֵ
	float PT100_D[3];      //PT100ͨ��У׼Dֵ
	

	uint32_t  hist_num;
	uint32_t  save_addr;
	uint32_t  pend_num;           /* �ȴ��������� */
	uint32_t  pend_addr;           /* �ȴ��������� */
//  uint32_t  bak_num;
//  uint32_t  bak_addr;
	
	uint32_t sensor_stime;    //��������ֹʱ�������
	uint8_t lorawan_net_status;    //lorawan ����״̬  0 ����/����ʧ��   1�����ɹ�
	uint8_t lorawan_set_data_en;    //lorawan �в����޸�
	
	uint8_t key;             //����������־
	uint8_t waibu_pwr_usb;       //�ⲿ��ԴUSB�����ǩ
	uint8_t waibu_pwr_24;       //�ⲿ��ԴUSB�����ǩ
//	uint16_t report_time;    //�����ϴ�ʱ�仺��

//	sensor_value_t sensor;       /* ���������� */	

  
//	float event_sensor1_data;    //����sen1��һ�ε�����
	float GPS_E,GPS_N;
//	float vbat_bfb;     //��ص����ٷֱ�
	uint32_t vbat_value;   //��ص�ѹ
	
	/*****��������*******************/
	
	uint8_t mqtt_statue;        //4G�������뼶�ϱ���ʽ��MQTT����״̬
	
} device_t;

extern device_t device_info;

uint16_t crc_16(uint8_t *p_buf, uint16_t length);
void device_clear_hist(void);
void device_param_write(save_param_t *p_pram);
void device_param_read(save_param_t *p_pram);
void device_param_get(uint8_t param_id);
void device_param_set(uint8_t *wr_buf);
void device_rtc_set(uint8_t *wr_buf,rtc_t *rtc_time);

void device_perv_addr_set(void);
uint32_t device_perv_addr_get(void);
void device_pending_num_set(void );
uint32_t device_pending_num_get(void);
uint32_t device_pending_addr_get(void);
void device_pending_addr_set(uint32_t num);
void device_nbvbat_set(uint32_t vbat);
uint32_t device_nbvbat_get(void);

//void device_iwdg_flag_set(uint16_t flag);
//uint32_t device_iwdg_flag__get(void);


void device_set_flahaddr_addr(uint32_t flag_addr);
uint32_t device_get_flahaddr_addr(void);

void device_restart_num_set(uint32_t restart_num);
uint32_t device_restart_num_get(void);

void VBAT_get(void);
void DZ_init(void);       //�������ǩ��ʼ��
void channel_data_init(void);    //ͨ�����ݳ�ʼ��
void device_hex_to_string(uint8_t *hex,uint8_t *str,uint8_t hex_len,uint8_t str_len);
void device_string_to_hex(uint8_t *str,uint8_t *hex,uint8_t str_len,uint8_t hex_len);
void device_vbat_reset(void);   //��ص�����λ
#endif
