#ifndef __USER_INIT_H__
#define __USER_INIT_H__

#include "main.h"

#define RCHF_CLOCK  LL_RCC_RCHF_FREQUENCY_8MHZ

#define NVR_CHECK(_N_VALUE_, _T_VALUE_)                         \
                            ((((_N_VALUE_ >> 16) & 0xffff) ==   \
                            ((~_N_VALUE_) & 0xffff)) ? _N_VALUE_ : _T_VALUE_)

#define RCHF8M_DEF_TRIM     (0x30)      // RC8M 经验校准值
#define RCHF16M_DEF_TRIM    (0x2A)      // RC16M 经验校准值
#define RCHF24M_DEF_TRIM    (0x27)      // RC24M 经验校准值

#define RCHF8M_NVR_TRIM     (*(uint32_t *)0x1FFFFB40)	// RC8M 常温校准值
#define RCHF16M_NVR_TRIM 	(*(uint32_t *)0x1FFFFB3C)	// RC16M 常温校准值
#define RCHF24M_NVR_TRIM 	(*(uint32_t *)0x1FFFFB38)	// RC24M 常温校准值


void UserInit(void);

void FoutInit(void);

void DelayUs(uint32_t count);
void DelayMs(uint32_t count);

#endif
