#include "DMA.h"
#include "stm32f10x.h"

void dmaInit(uint32_t first, uint32_t second, uint16_t size)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // 使能DMA时钟

    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = first;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据长度为8位
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Enable;    // 外设地址自增
    DMA_InitStructure.DMA_MemoryBaseAddr     = second;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte; // 内存数据长度为8位
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;    // 使能内存地址自增
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;   // 数据传输方向，Periph -> Memory first -> second
    DMA_InitStructure.DMA_BufferSize         = size;                    // 传输计数器
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;         // 正常模式,不自动重装
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;     // 中等优先级
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Enable;          // 禁止内存到内存传输
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel1, ENABLE);
}
