#include "lptim32.h"

//LPTIM32�жϷ������
void LPTIM_IRQHandler(void)
{
//    //��ʱ�����ʱ��תLED
//    if(FL_LPTIM32_IsEnabledIT_Update(LPTIM32) && FL_LPTIM32_IsActiveFlag_Update(LPTIM32))
//    {
////        LED0_TOG();
//        FL_LPTIM32_ClearFlag_Update(LPTIM32);
//    }
}

void LPTIM32_Init(void)
{
//    FL_LPTIM32_InitTypeDef timInit;

//    /*---------------- ��ʱ��ʱ���׼���� ----------------*/
//    FL_LPTIM32_StructInit(&timInit);
//    
//    timInit.clockSource          = FL_CMU_LPTIM32_CLK_SOURCE_APBCLK;
//    timInit.mode                 = FL_LPTIM32_OPERATION_MODE_NORMAL;
//    timInit.prescalerClockSource = FL_LPTIM32_CLK_SOURCE_INTERNAL;
//    timInit.prescaler            = FL_LPTIM32_PSC_DIV8;
//    timInit.autoReload           = 1000 - 1;
//    timInit.onePulseMode         = FL_LPTIM32_ONE_PULSE_MODE_CONTINUOUS;
//    timInit.triggerEdge          = FL_LPTIM32_ETR_TRIGGER_EDGE_RISING;
//    timInit.countEdge            = FL_LPTIM32_ETR_COUNT_EDGE_RISING;
//    FL_LPTIM32_Init(LPTIM32, &timInit);
//    
//    /*---------------------------------------------*/

//    /*---------------- �ж����� ----------------*/
//    /* �����־ */
//    FL_LPTIM32_ClearFlag_Update(LPTIM32);
//    
//    /* �ж�ʹ�� */
//    FL_LPTIM32_EnableIT_Update(LPTIM32);   

//    /* ʹ�ܲ�����NVIC */
//    NVIC_DisableIRQ(LPTIMx_IRQn);
//    NVIC_SetPriority(LPTIMx_IRQn,2);//�����ж����ȼ�
//    NVIC_EnableIRQ(LPTIMx_IRQn);
//    /*---------------------------------------------*/
//    
//    /* ʹ��LPTIM32 */
//    FL_LPTIM32_Enable(LPTIM32);
}

