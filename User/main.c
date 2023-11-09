#include "RTOSInit.h"
#include "stm32f10x.h"
#include "Serial.h"

int main(void) {

    Serial_Init(9600);
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
    RTOS_Init();
}
