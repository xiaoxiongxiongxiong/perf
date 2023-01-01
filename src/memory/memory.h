#ifndef _OS_PERF_MEMORY_H_
#define _OS_PERF_MEMORY_H_

#include <stdio.h>
#include <stdint.h>

// 系统内存信息
typedef struct _os_mem_sys_info_t {
    uint64_t total_kb;
    uint64_t free_kb;
    uint64_t avail_kb;
    uint64_t buffers_kb;
    uint64_t cached_kb;
    uint64_t swap_cached_kb;
    uint64_t swap_total_kb;
    uint64_t swap_free_kb;
    uint64_t vmalloc_total_kb;
    uint64_t vmalloc_used_kb;
    uint64_t vmalloc_chunk_kb;
} os_mem_sys_info_t;

// 进程内存信息
typedef struct _os_mem_proc_info_t {
    uint64_t peak_kb;  // 进程运行过程中占用内存峰值
    uint64_t vr_kb;    // 进程虚拟内存
    uint64_t rss_kb;   // 进程物理内存
} os_mem_proc_info_t;

/**
* os_memory_sys_info
* @brief   获取系统内存信息
* @param   info   系统内存信息结构体指针
* @return  >=0--成功 <0--失败
*/
int os_mem_get_sys_info(os_mem_sys_info_t * info);

/**
* os_mem_get_proc_info
* @brief   获取系统内存信息
* @param   pid    进程id
* @param   info   进程内存信息结构体指针
* @return  >=0--成功 <0--失败
*/
int os_mem_get_proc_info(int32_t pid, os_mem_proc_info_t * info);

#endif
