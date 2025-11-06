//
// Created by user on 2025/11/4.
//
#include "FreeRTOS.h"
#include "user_tasks.h"
#include "cmsis_os2.h"  // cubemx里面选的2

uint32_t count = 0;
uint32_t send = 0;
uint32_t recv = 0;


osThreadId_t test_task_handle;
osThreadAttr_t test_task_attributes = {
    .name = "testTask",          // 任务名称
    .stack_size = 256 * 4,       // 堆栈大小：256*4=1024字节
    .priority = osPriorityNormal // 任务优先级：普通优先级
};

osMessageQueueId_t test_queue_handle;
osMessageQueueAttr_t test_queue_attributes = {
    .name = "test_queue"
};

osThreadId_t test1_task_handle;
osThreadAttr_t test1_task_attributes = {
    .name = "test1Task",          // 任务名称
    .stack_size = 256 * 4,       // 堆栈大小：256*4=1024字节
    .priority = osPriorityNormal // 任务优先级：普通优先级
};


void test_task(void *) {
    while (true) {
        const auto tick = osKernelGetTickCount();
        // ++count;
        send += 1;
        osMessageQueuePut(test_queue_handle, &send, 0, 0);  // timeout:如果满了的阻塞时间。但这里表示满了不阻塞，直接返回失败
        osDelayUntil(tick + 1);
    }
}

void test1_task(void *) {
    while (true) {
        osMessageQueueGet(test_queue_handle, &recv, nullptr, osWaitForever);  //如果队列为空，任务会一直阻塞
    }
}


void user_tasks_init() {
    test_task_handle = osThreadNew(test_task, nullptr, &test_task_attributes);
    test1_task_handle = osThreadNew(test1_task, nullptr, &test1_task_attributes);
    test_queue_handle = osMessageQueueNew(16, sizeof(uint32_t), &test_queue_attributes);  // 新建了message

}
