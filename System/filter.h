#ifndef __FILTER_H
#define __FILTER_H
#define WINDOW_SIZE 5 // 求平均值一组数据的个数

// AverFilter结构体 data为数据缓冲区 pivot为数据缓冲区的指针 count为数据缓冲区中数据的个数
typedef struct {
    float data[WINDOW_SIZE];
    uint8_t pivot;
    uint8_t count;
} AverFilter;

void AverFilter_Init(AverFilter *filter);
void AverFilter_AddData(AverFilter *filter, float data);
float AverFilter_GetResult(AverFilter *filter);

#endif