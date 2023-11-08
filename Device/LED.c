#include "stm32f10x.h"
#include "general.h"

#define SPEED GPIO_Speed_50MHz

void LED_Init(GPIO_TypeDef *GPIOx, uint16_t pin) {

	if (GPIOx == GPIOA) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	} else if (GPIOx == GPIOB) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	} else if (GPIOx == GPIOC) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	}
	GPIO_startUp(GPIOx, pin, GPIO_Mode_Out_PP);
}

void LED_ON(GPIO_TypeDef* GPIOx, uint16_t pin) {
	GPIO_ResetBits(GPIOx, pin);
}

void LED_OFF(GPIO_TypeDef* GPIOx, uint16_t pin) {
	GPIO_SetBits(GPIOx, pin);
}

void LED_Switch(GPIO_TypeDef* GPIOx, uint16_t pin) {
	if (GPIO_ReadOutputDataBit(GPIOx, pin) == 1) {
		GPIO_ResetBits(GPIOx, pin);
	} else {
		GPIO_SetBits(GPIOx, pin);
	}
}
