#ifndef _OS_PERF_DISK_H_
#define _OS_PERF_DISK_H_

#include <stdio.h>
#include <stdbool.h>

typedef struct _os_disk_info_t os_disk_info_t;
typedef struct _os_mount_info_t os_mount_info_t;

// 磁盘模块初始化
bool os_perf_module_disk_init();

// 磁盘模块销毁
void os_perf_module_disk_uninit();

// 获取磁盘信息
bool os_perf_module_get_disk_info(os_disk_info_t * infos, size_t size);

// 获取挂载信息
bool os_perf_moudle_get_mount_info(os_mount_info_t * infos, size_t size);

#endif
