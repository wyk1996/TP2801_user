#include "main.h"
#include "bstim32.h"
void BSTIM32_Init(void)
{
//    LL_BSTIM32_InitTypeDef   InitStructer;
//    
//    InitStructer.prescaler         = 799;
//    InitStructer.autoReload        = 0xffffffff-1;    
//    InitStructer.autoReloadState   = ENABLE;
//    InitStructer.clockSource       = FL_CMU_BSTIM32_CLK_SOURCE_APBCLK;
//  
//    FL_BSTIM32_Init(BSTIM32,&InitStructer);
//  
//    FL_BSTIM32_Enable(BSTIM32);
}

uint32_t count = 0;

void TimeStart(void)
{
//    BSTIM32->CNT = 0;
}

uint32_t TimeStop(void)
{
//    return BSTIM32->CNT;
}
