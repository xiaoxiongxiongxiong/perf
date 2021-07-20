#ifndef _OS_PERF_H_
#define _OS_PERF_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
#define OS_PERF_API_BEGIN extern "C" {
#define OS_PERF_API_END }
#if defined(WIN32) || defined(_WIN32)
#if defined(OS_PERF_API_EXPORT)
#define OS_PERF_API __declspec(dllexport)
#else
#define OS_PERF_API __declspec(dllimport)
#endif
#else
#define OS_PERF_API
#endif
#else
#define OS_PERF_API_BEGIN
#define OS_PERF_API_END
#define OS_PERF_API
#endif

OS_PERF_API_BEGIN

#define OS_PERF_DISK          0x00000001u          // 磁盘
#define OS_PERF_MEMORY        0x00000010u          // 内存
#define OS_PERF_CPU           0x00000020u          // CPU
#define OS_PERF_NETWORK       0x00000200u          // 网络
#define OS_PERF_ALL           (OS_PERF_DISK | \
OS_PERF_MEMORY | OS_PERF_CPU | OS_PERF_NETWORK)    // 所有

/**
 * os_perf_init
 * @brief   性能模块初始化
 * @param   flags       需要初始化的模块
 * @return  true--成功 false--失败
 */
    OS_PERF_API bool os_perf_init(uint32_t flags);

OS_PERF_API_END


#endif
