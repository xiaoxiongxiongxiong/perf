#ifndef _OS_CONFIG_H_
#define _OS_CONFIG_H_

#include <math.h>

typedef unsigned long long os_ulonglong;

#define OS_PERF_KILO_BYTE (pow(2.0f, 10.0f)) // KB
#define OS_PERF_MEGA_BTTE (pow(2.0f, 20.0f)) // MB
#define OS_PERF_GIGA_BYTE (pow(2.0f, 30.0f)) // GB

#define OS_PERF_FLOAT_ZERO    (1e-6)  // float 0.0f
#define OS_PERF_DOUBLE_ZERO   (1e-15) // double 0.0

#define OS_PERF_ERROR_MAX 256   // 错误信息最大长度
#define OS_PERF_BUFF_MAX  4096  // 缓冲区最大长度
#define OS_PROC_PATH_MAX  64    // /proc路径最大长度

#define OS_PERF_PARTITIONS_PATH "/proc/partitions"
#define OS_PERF_DISKSTATS_PATH  "/proc/diskstats"
#define OS_PERF_MOUNT_PATH      "/etc/mtab"

typedef enum _OS_PERF_ERR_CODE
{
    OS_PERF_PARAM_NULL = 0,  // 参数为空指针
    OS_PERF_PARAM_INVALID,   // 参数不合法
} OS_PERF_ERR_CODE;

#endif
