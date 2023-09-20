#ifndef PERFORMANCE_TESTING_H
#define PERFORMANCE_TESTING_H

#include <stdio.h>
#include <time.h>

// 定义测试开始时间
#define START_TIMER clock_t start_time = clock()

// 定义测试结束时间
#define END_TIMER clock_t end_time = clock()

// 定义计算运行时间的宏
#define ELAPSED_TIME_MS ((double)(end_time - start_time) * 1000.0 / CLOCKS_PER_SEC)

// 定义测试函数性能的宏
#define TEST_FUNCTION_PERFORMANCE(func)                                 \
    do {                                                                \
        printf("Testing function: %s\n", #func);                         \
        START_TIMER;                                                    \
        func;                                                           \
        END_TIMER;                                                      \
        printf("Elapsed time: %.2f ms\n", ELAPSED_TIME_MS);              \
    } while (0)

#endif

