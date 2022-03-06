#ifndef _OS_PERF_NETWORK_H_
#define _OS_PERF_NETWORK_H_

#include <stdio.h>
#include <stdbool.h>

#define OS_NETWORK_MODULE_NAME "network"

// 网络模块初始化
bool os_network_init();

// 网络模块销毁
void os_network_uninit();

#endif
