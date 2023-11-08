#ifndef __RTOSINIT_H
#define __RTOSINIT_H

void RTOS_Init(void);
void startTask(void *pvParameters);
void task1(void *pvParameters);
void task2(void *pvParameters);

#endif