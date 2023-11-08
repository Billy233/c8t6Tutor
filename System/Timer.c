#include "stm32f10x.h"
#include "Timer.h"

void Timer_Init(TIM_TypeDef* timer, uint16_t period, uint16_t preScaler) {
    // 时钟源：内部时钟
    TIM_InternalClockConfig(timer);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = period;
    TIM_TimeBaseInitStructure.TIM_Prescaler = preScaler;
    TIM_TimeBaseInit(timer, &TIM_TimeBaseInitStructure);
}
