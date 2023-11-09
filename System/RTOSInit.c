#include "RTOSInit.h"
#include "FreeRTOS.h"
#include "task.h"
#include "LED.h"
#include "OLED.h"
#include "Kalman.h"
#define START_TASK_STACK_SIZE 128
#define START_TASK_PRIORITY   1
TaskHandle_t StartTask_Handler;
#define TASK_1_STACK_SIZE 128
#define TASK_1_PRIORITY   2
TaskHandle_t Task1_Handler;
#define TASK_2_STACK_SIZE 128
#define TASK_2_PRIORITY   3
TaskHandle_t Task2_Handler;

void RTOS_Init(void) {
    xTaskCreate((TaskFunction_t)startTask, "StartTask",
                START_TASK_STACK_SIZE, NULL,
                START_TASK_PRIORITY, &StartTask_Handler);
    vTaskStartScheduler();
}

void startTask(void *pvParameters) {
    taskENTER_CRITICAL();
    xTaskCreate((TaskFunction_t)task1, "Task1",
                TASK_1_STACK_SIZE, NULL,
                TASK_1_PRIORITY, &Task1_Handler);
    xTaskCreate((TaskFunction_t)task2, "Task2",
                TASK_2_STACK_SIZE, NULL,
                TASK_2_PRIORITY, &Task2_Handler);
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();
}

void task1(void *pvParameters) {
    LED_Init(GPIOC, GPIO_Pin_13);
    while (1) {
        LED_Switch(GPIOC, GPIO_Pin_13);
        vTaskDelay(50);
    }
}

void task2(void *pvParameters) {
    LED_Init(GPIOB, GPIO_Pin_12);
    while (1) {
        LED_Switch(GPIOB, GPIO_Pin_12);
        vTaskDelay(50);
    }
}
