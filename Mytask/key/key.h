#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"
#define KEY_COUNT    4

typedef struct
{
	uint8_t (*IsKeyPressFunt)(void);
	uint8_t  press_count;             /* 按下时间累计 */
	uint16_t long_count;             /* 长按计数 */
	uint16_t long_time;             
	uint8_t state;
}key_t;

typedef enum
{
	KID_MENU = 0,
	KID_PWR
}KEY_ID_E;

typedef enum
{
	KEY_NONE = 0,
	
//	MENU_PRESS,
//	MENU_UP,
//	MENU_LONG,
//	
	PWR_PRESS,
	PWR_UP,
	PWR_LONG,
}KEY_FUNC;


#define  KEY_FIFO_SIZE 5
typedef struct
{
	uint8_t buf[KEY_FIFO_SIZE];
	uint8_t read;
	uint8_t write; 
}key_fifo_t;

#define KEY_FILTER_TIME  2
#define KEY_LONG_TIME    200

void key_init(void);
void key_scan(void);
uint8_t key_get(void);
void key_put(uint8_t key_code);
void key_clear(void);

#endif 
