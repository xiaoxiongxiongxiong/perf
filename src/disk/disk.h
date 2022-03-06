#ifndef _OS_PERF_DISK_H_
#define _OS_PERF_DISK_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "perf.h"

#define OS_DISK_MODULE_NAME "disk" // 模块名称
#define OS_DISK_NAME_MAX    256    // 磁盘名称最大长度
#define OS_DISK_PATH_MAX    4096   // 磁盘路径最大长度

// 磁盘分区信息
typedef struct _os_disk_info_t
{
    int major;                    // 主设备号
    int minor;                    // 次设备号
    os_ulonglong bytes;           // 总容量
    char name[OS_DISK_NAME_MAX];  // 设备名称
} os_disk_info_t;

// 磁盘分区状态
typedef struct _os_disk_stat_t
{
    char name[OS_DISK_NAME_MAX];   // 设备名称
    uint64_t total;                // 总容量    单位:Byte
    uint64_t used;                 // 已用容量  单位:Byte
    double write_speed;            // 写速度    单位:KB/s
    double read_speed;             // 读速度    单位:KB/s
} os_disk_stat_t;

// 挂载信息
typedef struct _os_mnt_info_t
{
    char dev[OS_DISK_NAME_MAX];    // 设备名称
    char point[OS_DISK_PATH_MAX];  // 挂载路径
    uint64_t total;                // 总容量    单位:Byte
    uint64_t used;                 // 已用容量  单位:Byte
    uint64_t avail;                // 可用空间  单位:Byte
    double usage;                  // 使用率
} os_mnt_info_t;

OS_PERF_API_BEGIN

/**
* os_disk_init
* @brief   磁盘模块初始化
* @return  true--成功 false--失败
*/
OS_PERF_API bool os_disk_init();

/**
* os_disk_uninit
* @brief   磁盘模块销毁
*/
void os_disk_uninit();

/**
* os_disk_get_partitions
* @brief   获取系统分区信息
* @param   partitions  分区信息结构体指针 程序内部申请的内存，需调用os_disk_free释放掉
* @param   num         分区数量
* @return  true--成功 false--失败
*/
OS_PERF_API bool os_disk_get_partitions(os_disk_info_t ** partitions, size_t * num);

/**
* os_disk_get_partitions_stats
* @brief   获取系统分区状态
* @param   stats       分区信息结构体指针 程序内部申请的内存，需调用os_disk_free释放掉
* @param   num         数量
* @return  true--成功 false--失败
*/
OS_PERF_API bool os_disk_get_partitions_stats(os_disk_stat_t ** stats, size_t * num);

/**
* os_disk_get_mnt
* @brief   获取挂载信息
* @param   mnt         挂载信息结构体指针 程序内部申请的内存，需调用os_disk_free释放掉
* @param   num         数量
* @return  true--成功 false--失败
*/
OS_PERF_API bool os_disk_get_mnt(os_mnt_info_t ** mnt, size_t * num);

/**
* os_disk_free
* @brief   释放获取到的系统分区信息
* @param   partitions  分区信息结构体指针
*/
OS_PERF_API void os_disk_free(void * ptr);

OS_PERF_API_END

#endif
