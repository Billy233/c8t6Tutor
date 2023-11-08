#include "stm32f10x.h"
#include "general.h"
#include "Timer.h"

// 舵机周期 20ms 50Hz 
// CK_PSC / (ARR + 1) / (PSC + 1) = 50      ARR + 1 = 20000   PSC + 1 = 72
// f = CK_PSC / (PSC + 1) = 1MHz            T = 1 / 1M = 10^3 ms
// 经测试CCR顺时针有效值: 414 ~ 1460   对应 0.414ms ~ 1.46ms     占空比: 2.07% ~ 7.3%
           // 逆时针有效值：1528 ~ 2573    1.528ms ~ 2.573ms    占空比：7.64% ~ 12.865%  
// 离 1.5ms 越远转速越快?
void PWM_Init(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_startUp(GPIOA, GPIO_Pin_0, GPIO_Mode_AF_PP);

    Timer_Init(TIM2, 20000 - 1, 72 - 1);

    // TIM OC 输出捕获初始化
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 500;              // CCR
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    TIM_Cmd(TIM2, ENABLE);
}

void PWM_setCompare1(uint16_t cmp) {

    TIM_SetCompare1(TIM2, cmp);
}

// 设置 PSC -> PWM 频率
void PWM_setPrescaler(uint16_t prescaler) {
    TIM_PrescalerConfig(TIM2, prescaler, TIM_PSCReloadMode_Update);
}