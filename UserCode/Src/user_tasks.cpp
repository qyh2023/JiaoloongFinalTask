//
// Created by user on 2025/11/4.
//
#include "FreeRTOS.h"
#include "user_tasks.h"

#include "cmsis_os.h"
#include "cmsis_os2.h"  // cubemx里面选的2

uint32_t count = 0;
uint32_t send = 0;
uint32_t recv = 0;
// 1u：无符号整数1
constexpr auto flag_1 = 1u << 0;  // 00000001
constexpr auto flag_2 = 1u << 1;  // 00000010
bool flag_status = false;


osThreadId_t test_task_handle;
osThreadAttr_t test_task_attributes = {
    .name = "testTask",          // 任务名称
    .stack_size = 256 * 4,       // 堆栈大小：256*4=1024字节
    .priority = osPriorityNormal // 任务优先级：普通优先级
};

osThreadId_t test1_task_handle;
osThreadAttr_t test1_task_attributes = {
    .name = "test1Task",          // 任务名称
    .stack_size = 256 * 4,       // 堆栈大小：256*4=1024字节
    .priority = osPriorityNormal // 任务优先级：普通优先级
};

osThreadId_t test2_task_handle;
osThreadAttr_t test2_task_attributes = {
    .name = "testTask",          // 任务名称
    .stack_size = 256 * 4,       // 堆栈大小：256*4=1024字节
    .priority = osPriorityNormal // 任务优先级：普通优先级
};

osMessageQueueId_t test_queue_handle;
osMessageQueueAttr_t test_queue_attributes = {
    .name = "test_queue"
};

osSemaphoreId_t test_semaphore_handle;
osSemaphoreAttr_t test_semaphore_attributes = {
    .name = "test_semaphore"
};

//事件
osEventFlagsAttr_t test_event_flags_attributes = { .name = "test_event_flags"};
osEventFlagsId_t test_event_flags_handle;





// 给对列push东西的线程
// void test_task(void *) {
//     while (true) {
//         const auto tick = osKernelGetTickCount();
//         // ++count;
//         send += 1;
//         osMessageQueuePut(test_queue_handle, &send, 0, 0);  // timeout:如果队列满了的阻塞时间。但这里表示满了不阻塞，直接返回失败
//         osDelayUntil(tick + 1);
//     }
// }

// 释放信号量的线程
// void test_task(void *) {
//     while (true) {
//         const auto tick = osKernelGetTickCount();
//         // ++count;
//         if (send++ % 5 == 0) {
//             osSemaphoreRelease(test_semaphore_handle);  // 释放信号量
//         }
//         osDelayUntil(tick + 1);
//     }
// }

// 等 flag_1 | flag_2 事件 ，触发任务
void test_task(void *) {
    while (true) {
        osEventFlagsWait(test_event_flags_handle, flag_1 | flag_2, osFlagsWaitAll, osWaitForever);
        osEventFlagsClear(test_task_handle, flag_1 | flag_2);  // 触发事件之后清除标志位
        flag_status = !flag_status;
    }
}

// 消息队列
// void test1_task(void *) {
//     while (true) {
//         osMessageQueueGet(test_queue_handle, &recv, nullptr, osWaitForever);  //如果队列为空，任务会一直阻塞
//     }
// }

//
void test1_task(void *) {
    while (true) {
        const auto tick = osKernelGetTickCount();
        osEventFlagsSet(test_event_flags_handle, flag_1);
        osDelayUntil(tick + 1000);
    }
}

// 获取信号量并做事的线程
// void test2_task(void *) {
//     while (true) {
//         osSemaphoreAcquire(test_semaphore_handle, osWaitForever);
//         recv++;
//     }
// }

// set事件的task
void test2_task(void *) {
    while (true) {
        const auto tick = osKernelGetTickCount();
        osEventFlagsSet(test_event_flags_handle, flag_2);
        osDelayUntil(tick + 7000);
    }
}

void user_tasks_init() {
    // 1.消息队列：
    // test_task_handle = osThreadNew(test_task, nullptr, &test_task_attributes);  //线程test_task_handle，向 test_queue_handle 队列中push
    // test1_task_handle = osThreadNew(test1_task, nullptr, &test1_task_attributes);  // 线程test1_task_handle，从 test1_task_attributes 中pull

    // 消息队列ID = new(消息数量，消息大小，属性)
    // test_queue_handle = osMessageQueueNew(16, sizeof(uint32_t), &test_queue_attributes);  // 新建了message

    // 2.信号量
    // test_task_handle = osThreadNew(test_task, nullptr, &test_task_attributes);  //线程test_task_handle，释放信号量
    // test2_task_handle = osThreadNew(test2_task, nullptr, &test1_task_attributes);  // 线程test2_task_handle，获取信号量

    // 3.事件
    test_task_handle = osThreadNew(test_task, nullptr, &test_task_attributes);     // 等 flag_1 | flag_2 事件 ，触发任务
    test1_task_handle = osThreadNew(test1_task, nullptr, &test1_task_attributes);  // 线程test1_task_handle，flag1事件
    test2_task_handle = osThreadNew(test2_task, nullptr, &test2_task_attributes);  // 线程test2_task_handle，flag2事件

    // 信号量
    test_semaphore_handle = osSemaphoreNew(1, 0, &test_semaphore_attributes);
    // 二值信号量和技术信号狼的区别只是osSemaphoreNew的前两个参数，它们的类型在底层没有明显的区分

    // 事件
    test_event_flags_handle = osEventFlagsNew(&test_event_flags_attributes);


}
