#ifndef _OS_PERF_CONFIG_H_
#define _OS_PERF_CONFIG_H_

#include <math.h>

#define OS_PERF_KILO_BYTE (pow(2.0f, 10.0f)) // KB
#define OS_PERF_MEGA_BTTE (pow(2.0f, 20.0f)) // MB
#define OS_PERF_GIGA_BYTE (pow(2.0f, 30.0f)) // GB

#define OS_PERF_FLOAT_ZERO    (1e-6)  // float 0.0f
#define OS_PERF_DOUBLE_ZERO   (1e-15) // double 0.0

#define OS_PERF_ERROR_MAX 260   // 错误信息最大长度
#define OS_PERF_BUFF_MAX  4096  // 缓冲区最大长度
#define OS_PERF_LINE_MAX  1024  // 每行最大长度
#define OS_PROC_PATH_MAX  64    // /proc路径最大长度

#define OS_PERF_NET_DEV_PATH    "/proc/net/dev"
#define OS_PERF_ROUTE_IPV4_PATH "/proc/net/route"
#define OS_PERF_ROUTE_IPV6_PATH "/proc/net/ipv6_route"
#define OS_PERF_MEMINFO_PATH    "/proc/meminfo"
#define OS_PERF_PARTITIONS_PATH "/proc/partitions"
#define OS_PERF_DISKSTATS_PATH  "/proc/diskstats"
#define OS_PERF_MOUNT_PATH      "/etc/mtab"

#endif
