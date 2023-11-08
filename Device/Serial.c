#include <stdio.h>
#include "stm32f10x.h"
#include "Serial.h"
#include "general.h"
#define LEN 256

uint8_t pivot = 0;
uint8_t Serial_RxPacket[LEN];
uint8_t RxFlag; // 接受到数据标志

void Serial_Init(uint32_t baud) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);// USART3 对应 PB10 TX PB11 RX

	// 初始化 TX
	GPIO_startUp(GPIOB, GPIO_Pin_10, GPIO_Mode_AF_PP);
	
	// 初始化 RX
	GPIO_startUp(GPIOB, GPIO_Pin_11, GPIO_Mode_IPU);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No; 		// 无校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	// 停止位长度
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStructure);
	
	// 配置中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	NVIC_startUp(USART3_IRQn, 1, 1);
	USART_Cmd(USART3, ENABLE);
}

void Serial_SendByte(uint8_t byte) {

	USART_SendData(USART3, byte);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

// 发送数组
void Serial_SendArray(uint8_t *array, uint16_t length) {
	for (int i = 0; i < length; i++) {
		Serial_SendByte(array[i]);
	}
}

void Serial_SendString(char *string) {
	// string 以 0x00 结尾，存储空间=字符个数 + 1
	for (uint8_t i = 0; string[i] != '\0'; i++) {
		Serial_SendByte(string[i]);
	}
}

// 重定向 printf
int fputc(int ch, FILE *f) {
	Serial_SendByte(ch);
	return ch;
}

// 传 接受数据包指针
uint8_t* getRxPacket(void) {
	return Serial_RxPacket;
}

uint8_t getRxFlag(void) {
	if (RxFlag == 1) {
		RxFlag = 0;
		return 1;
	}
	return 0;
}

// 中断函数
void USART3_IRQHandler(void) {
	static uint8_t RxState = 0;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET) {
		uint8_t RxData = USART_ReceiveData(USART3);
		
		if(RxState == 0 && RxData == 0xAA) {
			RxState = 1;
			pivot = 0;
		} else if (RxState == 1) {
			if(RxData == 0xC6) {
				RxState = 2;
			} else {
				Serial_RxPacket[pivot] = RxData;
				pivot++;
			}
		} else if (RxState == 2 && RxData == 0xFF) {
			RxState = 0;
			RxFlag = 1;
		}
		
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}

// 发送可变包长HEX数据包 AA包头 C6 FF包尾
void Serial_SendPacket(uint8_t *packet) {
	Serial_SendByte(0xAA);
	Serial_SendArray(packet, pivot);
	Serial_SendByte(0xC6);
	Serial_SendByte(0xFF);
}
