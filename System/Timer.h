#ifndef __TIMER_H
#define __TIMER_H

extern uint32_t cnt;
void Timer_Init(TIM_TypeDef* timer, uint16_t period, uint16_t preScaler);

#endif