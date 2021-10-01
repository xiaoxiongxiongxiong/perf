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

#define OS_PERF_SHORT_BUFF_LEN 32   // 短缓冲长度
#define OS_PERF_IPV4_MAX_LEN   16   // ipv4最大地址
#define OS_PERF_IPV6_MAX_LEN   46   // ipv6最大地址
#define OS_PERF_NAME_MAX_LEN   64   // 名称长度
#define OS_PERF_PROC_PATH_LEN  64   // /proc目录下的路径长度
#define OS_PERF_FILE_MAX_LEN   256  // 文件名最大长度
#define OS_PERF_PATH_MAX_LEN   4096 // 路径最大长度
#define OS_PERF_ERROR_MAX_LEN  256  // 错误信息最大长度
#define OS_PERF_BUFF_MAX_LEN   1024 // 临时缓冲区长度

// 系统信息
typedef struct _os_sys_cpu_t
{
    long total_cores;   // 总内核个数
    float loads[3];     // 系统负载
    float usage;        // 系统占用
} os_sys_cpu_t;

// 获取进程CPU占用情况
typedef struct _os_proc_cpu_t
{
    long total_cores;  // 系统总内核个数
    long cores;        // 当前占用内核个数
    float usage;       // CPU占用
} os_proc_cpu_t;

// 磁盘信息
typedef struct _os_disk_info_t
{
    uint32_t major;                     // 主设备号
    uint32_t minor;                     // 次设备号
    uint64_t total;                     // 总容量
    double write_speed;                 // 写速度    单位:KB/s
    double read_speed;                  // 读速度    单位:KB/s
    char dev[OS_PERF_NAME_MAX_LEN];     // 设备名称
} os_disk_info_t;

// 挂载点信息
typedef struct _os_mount_info_t
{
    char dev[OS_PERF_NAME_MAX_LEN];     // 卷标名称
    char point[OS_PERF_BUFF_MAX_LEN];   // 挂载点
    uint64_t total;                     // 总容量    单位:Byte
    uint64_t used;                      // 已用容量  单位:Byte
    uint64_t avail;                     // 可用容量
    double usage;                       // 使用率
} os_mount_info_t;

// 系统内存信息
typedef struct _os_sys_mem_t
{
    uint64_t total;    // 总内存     单位:KB
    uint64_t avail;    // 可用内存   单位:KB
    uint64_t used;     // 已用内存   单位:KB
} os_sys_mem_t;

// 系统内存信息
typedef struct _os_proc_mem_t
{
    char name[OS_PERF_NAME_MAX_LEN];     // 进程名称
    uint64_t vr_mem;  // 虚拟内存
    uint64_t rss_mem; // 实际内存
} os_proc_mem_t;

// 网卡信息
typedef struct _os_adapter_info_t
{
    float recv_rate;                      // 接收速度 单位:Byte/s
    float send_rate;                      // 发送速度 单位:Byte/s
    uint32_t rdpkts;                      // 接收丢包数量
    uint32_t sdpkts;                      // 发送丢包数量
    uint32_t bandwidth;                   // 带宽
    char duplex[OS_PERF_IPV4_MAX_LEN];    // 双工模式 full-全双工 half-半双工
    char iface[OS_PERF_NAME_MAX_LEN];     // 接口名称
    char ip[OS_PERF_IPV4_MAX_LEN];        // IP地址
} os_adapter_info_t;

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

/*
* os_perf_get_last_error
* @brief   获取最后一次错误信息
* @return  错误信息
*/
OS_PERF_API const char * os_perf_get_last_error();

OS_PERF_API_END

#endif
