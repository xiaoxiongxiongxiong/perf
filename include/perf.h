#ifndef _OS_PERF_H_
#define _OS_PERF_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

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

#define OS_PERF_CPU           0b00000001u     // CPU
#define OS_PERF_DISK          0b00000010u     // 磁盘
#define OS_PERF_MEMORY        0b00000100u     // 内存
#define OS_PERF_NETWORK       0b00001000u     // 网络
#define OS_PERF_ALL           0b00001111u     // 所有

#define OS_PERF_ERROR_MAX_LEN  256   // 错误信息最大长度

/**
 * os_log_callback
 * @brief   日志回调
 * @param   flags       需要初始化的模块
 * @return  true--成功 false--失败
 */
    typedef void (*os_log_callback)(const char * name, int level, const char * fmt, va_list vl);

    /**
    * os_perf_init
    * @brief   性能模块初始化
    * @param   flags       需要初始化的模块
    * @return  true--成功 false--失败
    */
OS_PERF_API bool os_perf_init(uint32_t flags);

/**
* os_perf_uninit
* @brief   性能模块销毁
*/
OS_PERF_API void os_perf_uninit();

/**
* os_perf_set_cb
* @brief   设置日志回调
*/
OS_PERF_API void os_perf_set_callback(os_log_callback cb);

/*
* os_perf_get_last_error
* @brief   获取最后一次错误信息
* @return  错误信息
*/
OS_PERF_API const char * os_perf_get_last_error();

OS_PERF_API_END

#endif
