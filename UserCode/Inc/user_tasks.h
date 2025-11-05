//
// Created by user on 2025/11/4.
//

#ifndef JIAOLOONGFINALTASK_USER_TASKS_H
#define JIAOLOONGFINALTASK_USER_TASKS_H


#ifdef __cplusplus
extern "C"{
#endif

    // 混合编译说明：
    // C和C++混合编译时，如果C文件要使用C++的函数，需要用extern "C"
    // 告诉编译器按C的方式编译，避免因名字改编规则不同导致链接错误
    // 同样，如果C++文件要使用C的函数，也要用extern "C"包含C的头文件
    // 这就是为什么直接调用driver里的头文件"stm32f4xx_hal_xx.h"时可能编译失败

    void user_tasks_init();

#ifdef __cplusplus
}
#endif


#endif //JIAOLOONGFINALTASK_USER_TASKS_H