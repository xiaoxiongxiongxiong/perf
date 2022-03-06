#ifndef _OS_PERF_CPU_H_
#define _OS_PERF_CPU_H_

#include <stdio.h>
#include <stdbool.h>

#define OS_CPU_MODULE_NAME "cpu"

// 初始化CPU
bool os_cpu_init();

// 销毁
void os_cpu_uninit();

#endif
