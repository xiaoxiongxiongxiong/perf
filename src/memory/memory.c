#include "memory.h"
#include <unistd.h>
#include <string.h>
#include "utils/err.h"

#define OS_MEM_BUFF_MAX_LEN 128
#define OS_MEM_MEMINFO_PATH "/proc/meminfo"

int os_mem_get_sys_info(os_mem_sys_info_t* info)
{
    int ret = 0;
    FILE* fp = NULL;
    uint64_t value = 0;
    char key[OS_MEM_BUFF_MAX_LEN] = { 0 };
    char buff[OS_MEM_BUFF_MAX_LEN] = { 0 };

    if (NULL == info) {
        ret = OS_PERF_ERR_NULLPTR;
        goto end;
    }

    fp = fopen(OS_MEM_MEMINFO_PATH, "r");
    if (NULL == fp) {
        ret = OS_PERF_ERROR(errno);
        goto end;
    }

    while (!feof(fp)) {
        memset(buff, 0, OS_MEM_BUFF_MAX_LEN);
        fgets(buff, OS_MEM_BUFF_MAX_LEN, fp);

        value = 0;
        memset(key, 0, OS_MEM_BUFF_MAX_LEN);
        if (2 != sscanf(buff, "%s %Lu", key, &value))
            continue;

        if (0 == strcmp("MemTotal:", key))
            info->total_kb = value;
        else if (0 == strcmp("MemFree:", key))
            info->free_kb = value;
        else if (0 == strcmp("MemAvailable:", key))
            info->avail_kb = value;
        else if (0 == strcmp("Buffers:", key))
            info->buffers_kb = value;
        else if (0 == strcmp("Cached:", key))
            info->cached_kb = value;
        else if (0 == strcmp("SwapCached:", key))
            info->swap_cached_kb = value;
        else if (0 == strcmp("Active:", key))
            info->active_kb = value;
        else if (0 == strcmp("Inactive:", key))
            info->inactive_kb = value;
        else if (0 == strcmp("Active(anon):", key))
            info->anon_active_kb = value;
        else if (0 == strcmp("Inactive(anon):", key))
            info->anon_inactive_kb = value;
        else if (0 == strcmp("Active(file):", key))
            info->file_active_kb = value;
        else if (0 == strcmp("Inactive(file):", key))
            info->file_inactive_kb = value;
        else if (0 == strcmp("Unevictable:", key))
            info->unevictable_kb = value;
        else if (0 == strcmp("Mlocked:", key))
            info->mlocked_kb = value;
        else if (0 == strcmp("SwapTotal:", key))
            info->swap_total_kb = value;
        else if (0 == strcmp("SwapFree:", key))
            info->swap_free_kb = value;
        else if (0 == strcmp("Dirty:", key))
            info->dirty_kb = value;
        else if (0 == strcmp("Writeback:", key))
            info->write_back_kb = value;
        else if (0 == strcmp("AnonPages:", key))
            info->anon_pages_kb = value;
        else if (0 == strcmp("Mapped:", key))
            info->mapped_kb = value;
        else if (0 == strcmp("Shmem:", key))
            info->shmem_kb = value;
        else if (0 == strcmp("KReclaimable:", key))
            info->k_reclaimable_kb = value;
        else if (0 == strcmp("Slab:", key))
            info->slab_kb = value;
        else if (0 == strcmp("SReclaimable:", key))
            info->s_reclaimable_kb = value;
        else if (0 == strcmp("SUnreclaim:", key))
            info->s_unreclaim_kb = value;
        else if (0 == strcmp("KernelStack:", key))
            info->kernel_stack_kb = value;
        else if (0 == strcmp("PageTables:", key))
            info->page_tables = value;
        else if (0 == strcmp("NFS_Unstable:", key))
            info->nfs_unstable_kb = value;
        else if (0 == strcmp("Bounce:", key))
            info->bounce_kb = value;
        else if (0 == strcmp("WritebackTmp:", key))
            info->write_back_tmp_kb = value;
        else if (0 == strcmp("CommitLimit:", key))
            info->commit_limit_kb = value;
        else if (0 == strcmp("Committed_AS:", key))
            info->commited_as_kb = value;
        else if (0 == strcmp("VmallocTotal:", key))
            info->vmalloc_total_kb = value;
        else if (0 == strcmp("VmallocUsed:", key))
            info->vmalloc_used_kb = value;
        else if (0 == strcmp("VmallocChunk:", key))
            info->vmalloc_chunk_kb = value;
        else if (0 == strcmp("Percpu:", key))
            info->percpu_kb = value;
        else if (0 == strcmp("HardwareCorrupted:", key))
            info->hardware_corrupted_kb = value;
        else if (0 == strcmp("AnonHugePages:", key))
            info->anon_huge_pages_kb = value;
        else if (0 == strcmp("ShmemHugePages:", key))
            info->shmem_huge_pages_kb = value;
        else if (0 == strcmp("ShmemPmdMapped:", key))
            info->shmem_pmd_mapped_kb = value;
        else if (0 == strcmp("FileHugePages:", key))
            info->file_huge_pages_kb = value;
        else if (0 == strcmp("FilePmdMapped:", key))
            info->file_pmd_mapped_kb = value;
        else if (0 == strcmp("HugePages_Total:", key))
            info->huge_pages_total = value;
        else if (0 == strcmp("HugePages_Free:", key))
            info->huge_pages_free = value;
        else if (0 == strcmp("HugePages_Rsvd:", key))
            info->huge_pages_rsvd = value;
        else if (0 == strcmp("HugePages_Surp:", key))
            info->huge_pages_surp = value;
        else if (0 == strcmp("Hugepagesize:", key))
            info->huge_page_size_kb = value;
        else if (0 == strcmp("Hugetlb:", key))
            info->hugetlb_kb = value;
        else if (0 == strcmp("DirectMap4k:", key))
            info->direct_map_4k_kb = value;
        else if (0 == strcmp("DirectMap2M:", key))
            info->direct_map_2m_kb = value;
        else if (0 == strcmp("DirectMap1G:", key))
            info->direct_map_1g_kb = value;
        else
            continue;
    }

end:
    if (NULL != fp) {
        fclose(fp);
        fp = NULL;
    }

    return ret;
}

int os_mem_get_proc_info(const int32_t pid, os_mem_proc_info_t* info)
{
    int ret = 0;
    FILE* fp = NULL;
    uint64_t value = 0;
    char key[OS_MEM_BUFF_MAX_LEN] = { 0 };
    char buff[OS_MEM_BUFF_MAX_LEN] = { 0 };
    char path[OS_MEM_BUFF_MAX_LEN] = { 0 };

    if (NULL == info) {
        ret = OS_PERF_ERR_NULLPTR;
        goto end;
    }

    snprintf(path, OS_MEM_BUFF_MAX_LEN, "/proc/%d/status", pid);
    if (0 != access(path, F_OK)) {
        ret = OS_PERF_ERROR(ESRCH);
        goto end;
    }

    fp = fopen(path, "r");
    if (NULL == fp) {
        ret = OS_PERF_ERROR(errno);
        goto end;
    }

    while (!feof(fp)) {
        memset(buff, 0, OS_MEM_BUFF_MAX_LEN);
        fgets(buff, OS_MEM_BUFF_MAX_LEN, fp);

        value = 0;
        memset(key, 0, OS_MEM_BUFF_MAX_LEN);
        if (2 != sscanf(buff, "%s %Lu", key, &value))
            continue;

        if (0 == strcmp("VmPeak:", key))
            info->peak_kb = value;
        else if (0 == strcmp("VmSize:", key))
            info->vr_kb = value;
        else if (0 == strcmp("VmRSS:", key))
            info->rss_kb = value;
        else
            continue;
    }

end:
    if (NULL != fp) {
        fclose(fp);
        fp = NULL;
    }

    return ret;
}
