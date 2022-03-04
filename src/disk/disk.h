#ifndef _OS_PERF_DISK_H_
#define _OS_PERF_DISK_H_

#include <stdio.h>
#include <stdbool.h>

// 模块名称
#define OS_PERF_DISK_MODULE_NAME "disk"

typedef struct _os_disk_info_t os_disk_info_t;
typedef struct _os_mount_info_t os_mount_info_t;

// 磁盘模块初始化
bool os_disk_init();

// 磁盘模块销毁
void os_disk_uninit();

#endif
