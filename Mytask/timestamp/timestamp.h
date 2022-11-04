#ifndef __TIMESTAMP_H_
#define __TIMESTAMP_H_
#include "main.h"

void time_stamp_2_utc(long ts);

uint32_t time_2_time_stamp(rtc_t *date_time);

void stamp_2_time(long ts, rtc_t *time);

#endif
