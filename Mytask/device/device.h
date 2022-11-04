#ifndef _DEVICE_H
#define _DEVICE_H
#include "main.h"

#define APP_UPDATE_FLAG     (1024 * 8)
#define DEVICE_INFO   (9 * 1024)      //FLASH数据存储起始地址

//#define sensorerr_num  60


#define recive_len   400   //串口接收数组长度
//#define recive_len1  64   //串口接收数组长度
//#define output_len   256   //串口发送数组长度
#define output_len  400   //串口发送数组长度

#define LCD_IT        (1 << 0)
#define SEN_IT        (1 << 1)
#define SLEE_IT       (1 << 2)
#define NET_IT        (1 << 3)
#define OFF_IT        (1 << 4)
#define lora_IT       (1 << 5)
#define loraset_IT    (1 << 6)

#define EVENT_OK      (1)
#define EVENT_RUN     (0)

//NB服务器类型定义  sever_type
#define NB_tlink        0     
#define NB_onenet       1
#define NB_telecom      2
#define tlink_open      3
#define tlink_close     4
 //硬件版本   tab_model  
#define TP280xVx_0           0x01
#define TP280xVx_0_P         0x02   

//传感器类型 tab_sensor
#define One_wire          0x01    //TOPRIE 单总线类型
#define D032              0x02    //压力   沃天
#define D033              0X03    //压力   沃天
#define D020              0X04    //测勤IIC压力传感器40MPa
#define DS18B20           0x05    //

//One_wire  TOPRIE 单总线类型  自适应传感器
#define TP2305V2          0x01    //温湿度
#define TP2302V2          0x02    //PT100
#define TP2360            0x03    //红外测温  
#define TP2305V3          0x04    //温湿度+露点
#define TP2308            0x05    //甲醛  
#define TP2303V1          0x06    //PT1000
#define TP2310            0X0A    //二氧化碳  0-5000ppm
#define TP2311            0X0B    //热电偶
#define TP2312            0x0C    //水质电导率
#define TP2313						0x0D    //温湿度大气压力传感器
#define TP2314						0x0E    //二氧化碳大量程
#define TP2315            0x0F    //水浸传感器
#define TP2316            0x10    //pm2.5
#define TP2317            0x11    //TVOC
#define TP2318            0x12    //噪音
#define TP2319            0x13    //可燃气体
#define TP2321_1          0x14    //压力单总线  
#define TP2321            0x15    //液位单总线   
#define TP2322            0x16    //氧气  

//通讯模块tab_module
#define NULL1             0x01   
#define BC26              0x02
#define BC20              0x03    //NB带GPS定位
#define EC25              0x04
#define EC25_G            0x05    //4G带GPS定位版本
#define TP307V1_1_A       0x06
#define LORAWAN           0x07
#define M101							0x08
#define BC95							0x09

//连接协议 tab_protocol
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


//显示单位
#define UNIT_MPa       0x00
#define UNIT_KPa       0x01
#define UNIT_C         0X02    //℃
#define UNIT_F         0X03
#define UNIT_0         0X04   // 不显示单位

//网络运营商device_operator
#define CHN_Telecom       0x01   //中国电信
#define CHN_Mobile        0x02   //中国移动
#define CHN_Unicom        0x03   //中国联通
#define ATO_Search        0x04   //自动搜网

#define alarm_ok             0        //传感器数据正常
#define alarm_channel_err    (1<<0)    //传感器读取错误
#define alarm_channel_up     (1<<1)    //传感器数据超过上限
#define alarm_channel_down   (1<<2)    //传感器数据超过下限
#define alarm_channel_f      (1<<3)    //传感器数据超过波动阈值

//传感器可接入9只，前3只为固定PT100传感器，对应数据存储位0-2，后6只为可变IIC或者单总线传感器，对应数据存储位为3-14

typedef struct 
{ 
	/* 报警向量*/
	uint8_t ch_status[16];      //对应16个通道的报警状态
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
	
	new_CHANNEL_CLB,    //通道1校准
	new_CHANNEL1_CLB0,   //通道1校准零点值
	new_CHANNEL1_CLB1,   //通道1校准满点值
//	new_CHANNEL2_CLB,    //通道2校准
	new_CHANNEL2_CLB0,   //通道2校准零点值
	new_CHANNEL2_CLB1,   //通道2校准满点值
//	new_CHANNEL3_CLB,    //通道3校准
	new_CHANNEL3_CLB0,   //通道3校准零点值
	new_CHANNEL3_CLB1,   //通道3校准满点值
	
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
	
	NULL_CHANNEL1_SENSOR_NAME,      //通道传感器名称
	NULL_CHANNEL1_UP,           //通道上限
	NULL_CHANNEL1_DOWN,       //通道下限     //20
	NULL_CHANNEL1_F,         //通道波动阈值
	NULL_CHANNEL1_HC,        //通道报警回差
	NULL_CHANNEL1_C,         //通道修正值
	
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
	/* 秒 分 时 周 日 月 年*/
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t day;
	uint8_t week;
	uint8_t month;
	uint8_t year;
}
rtc_t;

//配置信息
typedef struct 
{
	uint8_t first_data[2];   //flash验证标志位
	uint16_t save_len;       //flash存储数据占用空间
	
	uint8_t srv_addr[24];   //服务器地址
	uint8_t srv_port[8];    //端口号
	uint8_t srv_login[64];     //登录包
	uint8_t srv_UserName[24];   //MQTT用户名
	uint8_t srv_Password[24];     //MQTT用户密码
	uint8_t srv_Topic[24];         //MQTT头文件
	uint8_t srv_Subacription[24];    //MQTT订阅名
	
	uint8_t srv_apn[24];   //APN
	uint8_t srv_apn_user_name[24];
	uint8_t srv_apn_user_pass[24];
	
	uint8_t srv_sim_pin[24];    //SIM卡PIN码
	uint8_t srv_sim_puk[24];    //SIM卡PUK码

//数据一共是16个通道(CH0-CH15)，CH0通道为电池电量数据;,第CH1-3通道是PT100数据，对应3路PT100采样接口;后面12个通道为IIC或者单总线通道，并且每2个通道对应一个port接口；
//传感器接口9个(port0-port8),其中0-5为IIC或者单总线接口，6-8为PT100接口
//
	float channel_up[16];      //通道值上限
	float channel_down[16];    //通道值下限
	float channel_f[16];       //通道波动阈值
	float channel_hc[16];      //通道报警回复差值
	float channel_c[16];       //通道修正值
	
	int32_t channel_range_l[16];  //传感器量程下限   针对后12个数据
  int32_t channel_range_h[16];  //传感器量程上限   针对后12个数据
	uint8_t channel_unit_p[16];  //预设单位
	uint8_t channel_unit[16];    //显示单位
	
	uint8_t  port_sensor_type[6];      //设备挂接传感器类型
	uint8_t  port_one_wire_type[6];        //自适应单总线传感器类型
	
	uint32_t battery_cap;      //电池容量	
		
	uint16_t srv_time;    //数据上传时间间隔	
	uint16_t BJ_pinlv_time;   //报警触发上报频率
	
	uint8_t device_operator;     //网络运营商名称   
	uint8_t  zhengdian;     //开启关闭整点上报功能标志位	
	uint8_t BJ_pinlv_stu;     //报警触发上报频率改变状态位
	uint8_t port_num;     //IIC接口传感器个数
	
	uint16_t  lora_id;      //lora地址
	uint8_t sever_type;    //NB_服务器类型
	uint8_t nop1;
//	uint8_t sensor_clear_flag; 	//传感器自动校0开关  
	
	uint8_t eliminate_signal;	   //小信号切除开关	    
	uint8_t device_dabao;  //是否开启打包上传标志	
	uint8_t tab_model;  // 设备型号
	uint8_t SENSOR_SAM_TIME;     //传感器采样时间间隔  
	
	uint8_t tab_module;      //设备模块
	uint8_t tab_protocol;      //设备通讯协议	
	uint8_t modbus_addr;     //modbus地址
	uint8_t dip_num;          //小数点位数   默认4位  
			
	uint32_t baud;           //modbus波特率
	uint32_t parity;         //modbus校验类型
	uint32_t stop_bits;      //modbus停止位
	uint32_t data_bits;      //modbus数据位
			
	uint8_t GPS_stu;
	uint8_t nop0;
	uint16_t nop4;      //未使用
	
	
	float pt100_100[3];
	float pt100_402[3];
	uint32_t nop11;      //未使用
	uint32_t nop12;      //未使用
	uint32_t nop13;      //未使用
	uint32_t nop14;      //未使用
	
  uint8_t DZ_NUM;      //定制程序编号
	uint8_t sensorerr_num;      //传感器掉线或者错误重读次数		
	uint16_t device_crc;  	
}
save_param_t;
/////总大小 0x11c
//传感器值

//其他信息
typedef struct {	
	rtc_t data_time;
	save_param_t save;           /* 保存到eeprom数据 */	 
		
//	uint8_t event_sensor;      //传感器读取成功标志
//	uint8_t sensor_err_num;    //传感器读取错误次数
	
	uint8_t event_flash;   //flash操作激活标志，防止进入休眠
	uint8_t event_up;    //数据上报激活标志，防止进入休眠
	uint8_t event_display;  //数据显示激活标志，防止进入休眠
	
	uint8_t event_sensorread;   //传感器读取激活标志，防止进入休眠
	uint8_t event_again;   //设备重启或者初次上电标志  
	uint8_t event_flash_again;    //设备重启或者初次上电flash存储标志位	
	
	float channel_data[16];

	float channel_data_f[16];    //保存上一轮传感器数据，用于防跳判断
	
	uint8_t first_work;    //系统第一次运行标志位
	uint8_t alarm_statue;    //继电器输出状态	
	uint8_t Params_sever;  //上发ICCID 第一次数据 且  用于配置完成发送标志
	
	uint8_t alarming_flag;    //报警触发标志位  上报完后清零
	alarm  alarm_flag_ID;           //报警向量事件ID
	alarm  alarm_flag_STATUS;           //报警向量事件STATUS
	uint8_t port_err_num[9];    //9个通道传感器读取失败次数标志位
	
	uint8_t debug_en;     //串口打印标志
	uint8_t device_en;   //usb 修改标志    0表示USB修改过   1表示USB控制保存

	uint8_t sensor_sam_timer;   //数据上传标志位

	uint16_t vabt_timer;    //电池电量更新计时器	
	uint8_t gps_time;           // nb模块gps定位时间间隔   24小时一次
	uint16_t wait_min;           /* 等待上报Min */
	uint8_t alarm_barck_flag;    //传感器采样时间计数器
	
	uint8_t gps_satues;       //gps定位状态指示标志
	uint8_t modbus_read_en;     //modbus读取操作使能
	uint8_t modbus_write_en;    //modbus写入操作使能
	
	
	uint8_t MCU_statues;          //MCU运行状态    0  开机状态   1 低功耗休眠状态   2 关机状态
	
	uint8_t djq_statue;         //继电器输出控制状态标志
	uint8_t time_calib;         //时间校准标志
	
	uint8_t pt100_clb_sta;     //PT100校准命令
	
	float PT100_K[3];      //pt100通道校准K值
	float PT100_D[3];      //PT100通道校准D值
	

	uint32_t  hist_num;
	uint32_t  save_addr;
	uint32_t  pend_num;           /* 等待发送数量 */
	uint32_t  pend_addr;           /* 等待发送数量 */
//  uint32_t  bak_num;
//  uint32_t  bak_addr;
	
	uint32_t sensor_stime;    //传感器静止时间计数器
	uint8_t lorawan_net_status;    //lorawan 网络状态  0 开机/联网失败   1联网成功
	uint8_t lorawan_set_data_en;    //lorawan 有参数修改
	
	uint8_t key;             //按键触发标志
	uint8_t waibu_pwr_usb;       //外部电源USB接入标签
	uint8_t waibu_pwr_24;       //外部电源USB接入标签
//	uint16_t report_time;    //数据上传时间缓存

//	sensor_value_t sensor;       /* 传感器数据 */	

  
//	float event_sensor1_data;    //保存sen1上一次的数据
	float GPS_E,GPS_N;
//	float vbat_bfb;     //电池电量百分比
	uint32_t vbat_value;   //电池电压
	
	/*****定制类编号*******************/
	
	uint8_t mqtt_statue;        //4G不休眠秒级上报方式，MQTT链接状态
	
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
void DZ_init(void);       //定制类标签初始化
void channel_data_init(void);    //通道数据初始化
void device_hex_to_string(uint8_t *hex,uint8_t *str,uint8_t hex_len,uint8_t str_len);
void device_string_to_hex(uint8_t *str,uint8_t *hex,uint8_t str_len,uint8_t hex_len);
void device_vbat_reset(void);   //电池电量复位
#endif
