//
// Created by user on 2025/11/4.
//
#include "FreeRTOS.h"
#include "user_tasks.h"
#include "cmsis_os2.h"  // cubemx里面选的2


osThreadId_t test_task_handle;
uint32_t count = 0;
osThreadAttr_t test_task_attributes = {
    .name = "testTask",          // 任务名称
    .stack_size = 256 * 4,       // 堆栈大小：256*4=1024字节
    .priority = osPriorityNormal // 任务优先级：普通优先级
};

void test_task(void *) {
    while (true) {
        const auto tick = osKernelGetTickCount();
        ++count;
        osDelayUntil(tick + 1);
    }
}


void user_tasks_init() {
    test_task_handle = osThreadNew(test_task, nullptr, &test_task_attributes);
}
