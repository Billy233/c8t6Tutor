#include "stm32f10x.h"
#include "filter.h"

//初始化，把filter清零
void AverFilter_Init(AverFilter *filter) {
    for (int i = 0; i < WINDOW_SIZE; i++) {
        filter->data[i] = 0.0;
    }
    filter->pivot = 0;
    filter->count = 0;
}

//添加一个数据到数据缓冲区
void AverFilter_AddData(AverFilter *filter, float data) {
    filter->data[filter->pivot] = data;
    filter->pivot = (filter->pivot + 1) % WINDOW_SIZE; //循环数据缓冲区eg.当pivot==4时，下一次pivot==0
    if (filter->count < WINDOW_SIZE) {
        filter->count++;
    }
}

//求和并求平均值
float AverFilter_GetResult(AverFilter *filter) {
    float sum = 0.0;
    for (int i = 0; i < filter->count; i++) {
        sum += filter->data[i];
    }
    return sum / filter->count;
}
