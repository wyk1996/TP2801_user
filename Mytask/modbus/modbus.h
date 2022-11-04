#ifndef __modbus_H
#define __modbus_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/*  */

/*  */
#define modbus_buf_num    28       //modbus协议寄存器个数   float类型数据，每个数据占两个寄存器
#define modbus_start_add      0x0000       //modbus协议寄存器起始地址
/* USER CODE END Private defines */
uint8_t mbrtu_proc(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf);
uint8_t mbtcp_proc(uint8_t *rx_cmd, uint8_t cmd_size, uint8_t *ack_buf);
void device_data_to_modbusbuf(void);
void modbusbuf_to_device_data(void);

uint8_t MODBUS_read1(void);     //客户定制类的外部modbus传感器 电导率 
uint8_t MODBUS_read2(void);     //客户定制类的外部modbus传感器 PH 

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ modbus_H */
