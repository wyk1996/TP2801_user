#ifndef  _nb_H_
#define  _nb_H_

#include "main.h"
#include "device.h"

typedef struct
{	 
	uint8_t nb_csq;
	uint8_t nb_cesq;
//	uint16_t nb_cbc;

	uint8_t nb_imsi[18];
	uint8_t nb_imei[18];
	uint8_t nb_iccid[24];
	uint32_t device_restart_num ;   //重启次数记录
	uint32_t Vbat_data;
  uint32_t mgid; 
} nb_ack;	


extern nb_ack nb;
extern uint8_t wireless_buf[recive_len];   //接收数组
extern hist_t  wl_hist_data;
extern rtc_t *gnss_data_time;
extern rtc_t cloud_time;
extern uint8_t transmit_cmd[output_len];  //发送数组1
//extern uint8_t sevebuf[output_len];    //发送数组2
//extern uint8_t BUFSIZ10;         //
//extern uint8_t sevebuf2[output_len1];   //发送数组3

void nb_OFF(void);
void nb_init_hard(void);

uint8_t nb_psmstart(void) ;         //PSM启动 
uint8_t nb_open(void);
uint8_t nb_init(void);

void get_nb_message(void);
uint8_t nb_sevebuf(void);
uint8_t nb_link_sever(void);

uint16_t str_to_hex16(uint8_t *buf, uint8_t * format, uint16_t us_length);
void hex16_to_str(uint8_t  *s_inchar, uint16_t s_len, uint8_t  *a_outtxt);

char BcdToChar(uint8_t _bcd);
uint8_t task_nb(void);
uint8_t task_srnb(void);
uint8_t task_tcp_udp(void);
uint8_t task_udpDZDG(void);
//void wireless_callback(uint32_t event);

void clear_allalarm(void);     //清除所有报警标志位
#endif

