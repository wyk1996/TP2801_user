#ifndef __ec20_4g_H
#define __ec20_4g_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/*  */

/*  */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */
#define init_sever   (1 << 1 )   
#define line_sever   (1 << 2 )   
#define MQTT_DATA    (1 << 3 )
#define MQTT_SUB     (1 << 4 )
#define MQTT_get     (1 << 5 )


uint8_t EC20_MQTT_task(void);
uint8_t EC20_MQTT_task_s(void);    //不休眠秒级模式下

uint8_t EC20_TCP_task(void);
uint8_t hex_to_BCD(uint8_t data);
	

/* USER CODE BEGIN Prototypes */
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ ec20_4g_H */
