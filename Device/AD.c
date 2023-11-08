#include "stm32f10x.h"
#include "AD.h"
#include "general.h"
#define ADCx ADC1

void AD_Init(void) {
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    if (ADCx == ADC1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    } else if (ADCx == ADC2) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
    }
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);//设置ADC时钟频率为12MHz
    GPIO_startUp(GPIOA, GPIO_Pin_4, GPIO_Mode_AIN);//PA0模拟输入
    
    ADC_RegularChannelConfig(ADCx, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);//ADC1,ADC通道0,第一个转换,采样时间为55.5周期
     
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//连续转换模式
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//不使用外部触发转换
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_Init(ADCx, &ADC_InitStructure);
    
    ADC_Cmd(ADCx, ENABLE);
    
    ADC_ResetCalibration(ADCx);//复位校准寄存器
    while (ADC_GetResetCalibrationStatus(ADCx));
    ADC_StartCalibration(ADCx);//校准ADC1
    while (ADC_GetCalibrationStatus(ADCx));//等待校准完成
    ADC_SoftwareStartConvCmd(ADCx, ENABLE);//使能指定的ADC1的软件转换启动功能
}

uint16_t AD_getValue(void) {
    /* while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //等待转换结束 */
    return ADC_GetConversionValue(ADCx);
}
