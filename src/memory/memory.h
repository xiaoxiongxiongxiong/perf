#ifndef _OS_PERF_MEMORY_H_
#define _OS_PERF_MEMORY_H_

#include <stdio.h>
#include <stdbool.h>

#define OS_PERF_MEMORY_MODULE_NAME "memory"

// 内存模块初始化
bool os_memory_init();

// 内存模块销毁
void os_perf_memory_uninit();

#endif
