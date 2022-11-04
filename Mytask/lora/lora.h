#ifndef  _lora_H_
#define  _lora_H_
#include "device.h"
typedef struct
{
	uint8_t lora_once;
	uint8_t lora_retry;
	uint8_t lora_updata;
	uint8_t lora_upack;
	uint8_t lora_updata_errnum;
	uint8_t LoRa_Txpower_set;
	
	uint8_t LoRa_sudu_set;
//	uint8_t LoRa_sudu;
	
	uint16_t lora_slot;
	uint16_t lora_num;	
	uint16_t LoRa_NETid_set;	
//	uint16_t LoRa_NETid;
	
//	uint32_t LoRa_Band;
//	uint32_t LoRa_Txpower;	
	uint32_t lora_time;
	uint32_t lora_pwrtime;
	uint32_t LoRa_Band_set;
	
}LORA_databuf;

extern LORA_databuf lora_buf;
uint8_t lora_data(void);

uint16_t lora_read(void);


void lora_set_NSID(uint16_t set_Nsid);
void lora_set_BAND(uint32_t set_Band ,uint8_t set_sudu);
void lora_set_Txpower(uint8_t Tx_Power);
uint8_t lora_get_base(void);
int16_t lora_get_signal(void);
void lora_ext(void);
void lora_rest(void);
uint8_t cs_Check( uint8_t *buf,uint8_t lensizeof);
//void lora_rest_gpioinit(void);


#endif
