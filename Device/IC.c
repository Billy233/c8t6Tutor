#include <stdio.h>
#include "stm32f10x.h"
#include "general.h"
#include "Timer.h"
#include "Delay.h"
#include "LED.h"
#include "OLED.h"

uint32_t startTime = 0, endTime = 0;
uint8_t captured = 0;

void IC_trigger(void) {
    // 将Trig引脚设置为低电平
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
    Delay_us(2); // 短暂延时

    // 生成一个高电平脉冲
    GPIO_SetBits(GPIOB, GPIO_Pin_5);
    Delay_us(20);

    // 再次将Trig引脚设置为低电平
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

void IC_Init(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_startUp(GPIOA, GPIO_Pin_0, GPIO_Mode_IN_FLOATING); // 配置echo引脚 浮空输入

    Timer_Init(TIM2, 65536 - 1, 7200 - 1); // Timer_Init(TIMx, period, preScaler)

    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_PWMIConfig(TIM2, &TIM_ICInitStructure);

    // 通道1捕捉上升沿，通道2捕捉下降沿
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
    NVIC_startUp(TIM2_IRQn, 1, 1);
    TIM_Cmd(TIM2, ENABLE);
}

// 超声波中断函数
void TIM2_IRQHandler(void) {
    if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET) {
        if(captured == 0) {
            startTime = TIM_GetCapture1(TIM2);
            captured = 1;
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    } else if (TIM_GetITStatus(TIM2, TIM_IT_CC2) == SET) {
        if (captured == 1) {
            endTime = TIM_GetCapture2(TIM2);

            if (endTime >= startTime) {
                // 加上判断防止出现 start 和 end 之间计数器溢出的情况
                uint32_t delta = endTime - startTime;
                float distance = (float)delta * 3.43 / 2;  // 72MHz / 7200 = 10^4Hz delta单位是 10^-4 s
                char s[10];
                sprintf(s, "%.3f", distance);              // float2string 
                OLED_ShowString(1, 1, s, 16);
                OLED_Update();
            }
            captured = 0;
        }
            
    }
}
