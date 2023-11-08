#include "stm32f10x.h"
#include "key.h"
#include "general.h"
#include "Delay.h"
#include "IC.h"

void key_Init(void) {

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_startUp(GPIOB, GPIO_Pin_15, GPIO_Mode_IPD);		 // PB15接按键 OUT
	GPIO_startUp(GPIOB, GPIO_Pin_5, GPIO_Mode_Out_PP);       // PB5 接超声波 trig

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);
	//配置 EXTI 外部中断
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;

	EXTI_Init(&EXTI_InitStructure);
	NVIC_startUp(EXTI15_10_IRQn, 1, 1);
}

void EXTI15_10_IRQHandler(void) {
	if(EXTI_GetITStatus(EXTI_Line15) == SET) {
        Delay_ms(20); // 消抖
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)) {
            IC_trigger();
        }
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
}
