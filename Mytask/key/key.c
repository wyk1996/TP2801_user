#include "key.h"

key_t  key_param[KEY_COUNT];

key_fifo_t key_cache;


static void key_init_var(void);
static void key_init_hard(void);

static uint8_t isKey1Press(void){if(GPIOA->IDR & LL_GPIO_PIN_1)return 0;else return 1;}
static uint8_t isKey2Press(void){if(GPIOA->IDR & LL_GPIO_PIN_0)return 1;else return 0;}

/**
  * @brief  按键初始化方法
  * @param  None
  * @retval None
  */
void key_init(void)
{
	key_init_hard();
	//key_init_var();
}

/**
  * @brief  按键硬件初始化
  * @param  None
  * @retval None
  */
static void key_init_hard(void)
{
	
	LL_GPIO_InitTypeDef GPIO_InitStrcut;
	
	 /**USART2 GPIO Configuration  
  PA0   ------> MENU_KEY
  PA1   ------> PWR_KEY 
  */
	
	GPIO_InitStrcut.Pin =  LL_GPIO_PIN_0;
	GPIO_InitStrcut.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStrcut.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOA, &GPIO_InitStrcut);
	
}

/**
  * @brief  按键类参数初始化
  * @param  None
  * @retval None
  */
void key_init_var(void)
{
	uint8_t i;
	key_cache.read  = 0;
	key_cache.write = 0;
	
	for(i = 0;i < KEY_COUNT; i++)
	{
		key_param[i].state = 0;
		key_param[i].long_count = 0;
		key_param[i].press_count = 0;
		key_param[i].long_time = KEY_LONG_TIME;
	}
	key_param[KID_MENU].IsKeyPressFunt = isKey1Press;
	key_param[KID_PWR].IsKeyPressFunt = isKey2Press;
	
}

/**
  * @brief  按键状态检测
  * @param  要检测的按键ID
  * @retval None
  */
void key_check(uint8_t key_id)
{
	key_t *key;
	key = &key_param[key_id];
	
	if(key->IsKeyPressFunt())
	{
		if(key->press_count < KEY_FILTER_TIME)
		{
			key->press_count = KEY_FILTER_TIME;
		}
		else if(key->press_count < KEY_FILTER_TIME * 2)
		{
			key->press_count++;
		}
		else 
		{
			if(key->state == 0)
			{
				key->state = 1;
				key_put(3 * key_id + 1);
			}
			if(key->long_time > 0)
			{
				if((key->long_count) < key->long_time)
				{
					if(++key->long_count == key->long_time)
					{
						key_put(3 * key_id + 3);
					}
				}
			}
		}
	}
	else 
	{
		if(key->press_count > KEY_FILTER_TIME)
		{
			key->press_count = KEY_FILTER_TIME;
		}
		else if( key->press_count != 0)
		{
			key->press_count--;
		}
		else 
		{
			if(key->state == 1)
			{
				/*曾经按下过*/
				key->state = 0;
				key_put(3 * key_id + 2);
			}
		}
		key->long_count = 0;
	}
}

/**
  * @brief  按键扫描函数
  * @param  None
  * @retval None
  */
void key_scan(void)
{
	uint8_t scan_count = 0;
	for(;scan_count < KEY_COUNT; scan_count++)
	{
		key_check(scan_count);
	}
}

/**
  * @brief  按键获取函数
  * @param  None
  * @retval None
  */
uint8_t key_get(void)
{
	uint8_t ret;
	if(key_cache.read == key_cache.write)
	{
		return KEY_NONE;
	}
	else 
	{
		ret = key_cache.buf[key_cache.read];
		if(++key_cache.read >= KEY_FIFO_SIZE)
		{
			key_cache.read = 0;
		}
	}
	return ret;
}

/**
  * @brief  按键状态输出FIFO函数
  * @param  None
  * @retval None
  */
void key_put(uint8_t key_code)
{
	key_cache.buf[key_cache.write] = key_code;
	if(++key_cache.write >= KEY_FIFO_SIZE)
	{
		key_cache.write = 0;
	}
}

/**
  * @brief  按键FIFO清除函数
  * @param  None
  * @retval None
  */
void key_clear(void)
{
	key_cache.write = key_cache.read = 0;
}
