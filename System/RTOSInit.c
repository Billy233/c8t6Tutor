#include "RTOSInit.h"
#include "FreeRTOS.h"
#include "Kalman.h"
#include "LED.h"
#include "OLED.h"
#include "task.h"

#define START_TASK_STACK_SIZE 128
#define START_TASK_PRIORITY   1
TaskHandle_t StartTask_Handler;                    // 开始任务句柄
StackType_t startTaskStack[START_TASK_STACK_SIZE]; // 定义开始任务栈空间
StaticTask_t startTaskTCB;                         // 定义开始任务控制块

#define TASK_1_STACK_SIZE 128
#define TASK_1_PRIORITY   2
TaskHandle_t Task1_Handler;
StackType_t task1_Stack[TASK_1_STACK_SIZE];
StaticTask_t task1_TCB;

#define TASK_2_STACK_SIZE 128
#define TASK_2_PRIORITY   3
TaskHandle_t Task2_Handler;
StackType_t task2_Stack[TASK_2_STACK_SIZE];
StaticTask_t task2_TCB;

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];
    *ppxIdleTaskTCBBuffer   = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize   = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];
    *ppxTimerTaskTCBBuffer   = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize   = configTIMER_TASK_STACK_DEPTH;
}

void RTOS_Init(void)
{
    StartTask_Handler = xTaskCreateStatic((TaskFunction_t)startTask, "StarTask",
                                          START_TASK_STACK_SIZE, NULL, START_TASK_PRIORITY,
                                          startTaskStack, &startTaskTCB);
    vTaskStartScheduler();
}

void startTask(void *pvParameters)
{
    taskENTER_CRITICAL(); // 进入临界区,防止创建任务期间被打断
    Task1_Handler = xTaskCreateStatic((TaskFunction_t)task1, "Task1",
                                      TASK_1_STACK_SIZE, NULL, TASK_1_PRIORITY,
                                      task1_Stack, &task1_TCB);

    Task2_Handler = xTaskCreateStatic((TaskFunction_t)task2, "Task2",
                                      TASK_2_STACK_SIZE, NULL, TASK_2_PRIORITY,
                                      task2_Stack, &task2_TCB);
    vTaskDelete(StartTask_Handler); // 删除开始任务
    taskEXIT_CRITICAL();
}

void task1(void *pvParameters)
{
    LED_Init(GPIOC, GPIO_Pin_13);
    while (1) {
        LED_Switch(GPIOC, GPIO_Pin_13);
        vTaskDelay(50);
    }
}

void task2(void *pvParameters)
{
    LED_Init(GPIOB, GPIO_Pin_12);
    while (1) {
        LED_Switch(GPIOB, GPIO_Pin_12);
        vTaskDelay(50);
    }
}
