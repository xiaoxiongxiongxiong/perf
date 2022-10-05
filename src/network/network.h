#ifndef _OS_PERF_NETWORK_H_
#define _OS_PERF_NETWORK_H_

#include <stdio.h>
#include <stdbool.h>

// 网络模块初始化
bool os_net_init();

// 网络模块销毁
void os_net_uninit();

#endif
