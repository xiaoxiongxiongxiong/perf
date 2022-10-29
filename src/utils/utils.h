#ifndef _OS_PERF_UTILS_H_
#define _OS_PERF_UTILS_H_

#include <stdio.h>
#include <stdint.h>

/**
* os_utils_q2d
* @brief  获取商
* @param  num 分子
* @param  den 分母
* @return 商
*/
double os_utils_q2d(int64_t num, int64_t den);

/**
* @brief  重申清内存
* @param  ptr  指针
* @param  size 内存大小
* @return 0-成功 < 0 错误码
*/
int os_utils_reallocp(void * ptr, size_t size);

/**
* os_utils_freep
* @brief   释放指针
* @param   ptr      待释放指针
*/
void os_utils_freep(void * ptr);

#endif
