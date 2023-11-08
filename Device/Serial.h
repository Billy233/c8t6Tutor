#ifndef __SERIAL_H
#define __SERIAL_H
#include "stm32f10x.h"

extern uint8_t pivot;

void Serial_Init(uint32_t baud);
void Serial_SendByte(uint8_t byte);
void Serial_SendArray(uint8_t *array, uint16_t length);
void Serial_SendString(char *string);
void Serial_SendPacket(uint8_t *packet);

uint8_t* getRxPacket(void);
uint8_t getRxFlag(void);
#endif
