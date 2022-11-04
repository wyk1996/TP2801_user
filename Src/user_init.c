#include "user_init.h"

void ClockInit(void)
{
    uint32_t trim;
    switch (RCHF_CLOCK)
    {
        case LL_RCC_RCHF_FREQUENCY_8MHZ:
            LL_RCC_SetRCHFFrequency(RCHF_CLOCK);
            trim = NVR_CHECK(RCHF8M_NVR_TRIM, RCHF8M_DEF_TRIM) & 0x7f;
            LL_RCC_SetRCHFTrimValue(trim);
            break;
        
        case LL_RCC_RCHF_FREQUENCY_16MHZ:
            LL_RCC_SetRCHFFrequency(RCHF_CLOCK);
            trim = NVR_CHECK(RCHF16M_NVR_TRIM, RCHF16M_DEF_TRIM) & 0x7f;
            LL_RCC_SetRCHFTrimValue(trim);
            break;
        
        case LL_RCC_RCHF_FREQUENCY_24MHZ:
            LL_RCC_SetRCHFFrequency(RCHF_CLOCK);
            trim = NVR_CHECK(RCHF24M_NVR_TRIM, RCHF24M_DEF_TRIM) & 0x7f;
            LL_RCC_SetRCHFTrimValue(trim);
            break;
        
        default:
            LL_RCC_SetRCHFFrequency(RCHF_CLOCK);
            trim = NVR_CHECK(RCHF8M_NVR_TRIM, RCHF8M_DEF_TRIM) & 0x7f;
            LL_RCC_SetRCHFTrimValue(trim);
            break;
    }
    
    LL_RCC_SetSystemClockSource(LL_RCC_SYSTEM_CLKSOURCE_RCHF);
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
}

static void SystickInit(void)
{
    #if 0
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    #endif
}


void FoutInit(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = DISABLE;
    LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    LL_GPIO_SetFout0(GPIO_COMMON, LL_GPIO_FOUT0_SELECT_AHBCLK_DIV64);
}

void UserInit(void)
{
	ClockInit();
    SystickInit();
    #if DEBUG_ENTER
    //调试使用，可直接移除
    FoutInit();
    #endif
}

void DelayUs(uint32_t count)
{
    count = (uint64_t)LL_RCC_GetSystemClockFreq() * count / 1000000;
    count = count > 16777216 ? 16777216 : count;
    
    SysTick->LOAD = count - 1;
    SysTick->VAL = 0;
    while (!((SysTick->CTRL >> 16) & 0x1));
}

void DelayMs(uint32_t count)
{
    while (count--)
    {
        DelayUs(1000);
    }
}
