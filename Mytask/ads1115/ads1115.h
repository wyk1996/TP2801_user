#ifndef __ads1115_H
#define __ads1115_H



#include "myiic.h"

typedef struct 
{
    float  value[1];
}ad_t;

uint8_t ads1115_write(uint8_t id,uint8_t regaddr,uint8_t byte1,uint8_t byte2);
void ads1115_read(uint8_t regaddr,uint8_t *ad_raw);

void ads1115_get_value(uint8_t channel, uint8_t type);

#endif



