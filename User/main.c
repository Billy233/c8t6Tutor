#include "RTOSInit.h"
#include "stm32f10x.h"

int main(void) {

    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
    RTOS_Init();
}
