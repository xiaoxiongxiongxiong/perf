#ifndef _OS_PERF_MEMORY_H_
#define _OS_PERF_MEMORY_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// 系统内存信息
typedef struct _os_mem_sys_info_t {
    uint64_t total_kb;
    uint64_t free_kb;
    uint64_t avail_kb;
    uint64_t buffers_kb;
    uint64_t cached_kb;
    uint64_t swap_cached_kb;
    uint64_t active_kb;
    uint64_t inactive_kb;
    uint64_t anon_active_kb;
    uint64_t anon_inactive_kb;
    uint64_t file_active_kb;
    uint64_t file_inactive_kb;
    uint64_t unevictable_kb;
    uint64_t mlocked_kb;
    uint64_t swap_total_kb;
    uint64_t swap_free_kb;
    uint64_t dirty_kb;
    uint64_t write_back_kb;
    uint64_t anon_pages_kb;
    uint64_t mapped_kb;
    uint64_t shmem_kb;
    uint64_t k_reclaimable_kb;
    uint64_t slab_kb;
    uint64_t s_reclaimable_kb;
    uint64_t s_unreclaim_kb;
    uint64_t kernel_stack_kb;
    uint64_t page_tables;
    uint64_t nfs_unstable_kb;
    uint64_t bounce_kb;
    uint64_t write_back_tmp_kb;
    uint64_t commit_limit_kb;
    uint64_t commited_as_kb;
    uint64_t vmalloc_total_kb;
    uint64_t vmalloc_used_kb;
    uint64_t vmalloc_chunk_kb;
    uint64_t percpu_kb;
    uint64_t hardware_corrupted_kb;
    uint64_t anon_huge_pages_kb;
    uint64_t shmem_huge_pages_kb;
    uint64_t shmem_pmd_mapped_kb;
    uint64_t file_huge_pages_kb;
    uint64_t file_pmd_mapped_kb;
    uint64_t huge_pages_total;
    uint64_t huge_pages_free;
    uint64_t huge_pages_rsvd;
    uint64_t huge_pages_surp;
    uint64_t huge_page_size_kb;
    uint64_t hugetlb_kb;
    uint64_t direct_map_4k_kb;
    uint64_t direct_map_2m_kb;
    uint64_t direct_map_1g_kb;
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
int os_mem_get_sys_info(os_mem_sys_info_t* info);

/**
* os_mem_get_proc_info
* @brief   获取系统内存信息
* @param   pid    进程id
* @param   info   进程内存信息结构体指针
* @return  >=0--成功 <0--失败
*/
int os_mem_get_proc_info(int32_t pid, os_mem_proc_info_t* info);

#endif
