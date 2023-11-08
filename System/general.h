#ifndef __GENERAL_H
#define __GENERAL_H

void GPIO_startUp(GPIO_TypeDef *GPIOx, uint16_t pin, GPIOMode_TypeDef mode);
void NVIC_startUp(uint8_t IRQChannel, uint8_t preemptionPriority, uint8_t subPriority);

#endif