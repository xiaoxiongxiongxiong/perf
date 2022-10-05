#include "memory.h"
#include <unistd.h>
#include <string.h>
#include "utils/err.h"

#define OS_MEM_BUFF_MAX_LEN 128
#define OS_MEM_MEMINFO_PATH "/proc/meminfo"

int os_mem_get_sys_info(os_mem_sys_info_t * info)
{
    int ret = 0;
    FILE * fp = NULL;
    uint64_t value = 0;
    char key[OS_MEM_BUFF_MAX_LEN] = {};
    char buff[OS_MEM_BUFF_MAX_LEN] = {};

    if (NULL == info) {
        ret = OS_PERF_ERR_NULLPTR;
        goto end;
    }

    fp = fopen(OS_MEM_MEMINFO_PATH, "r");
    if (NULL == fp) {
        ret = OS_PERF_ERROR(errno);
        goto end;
    }

    while (!feof(fp) && fgets(buff, OS_MEM_BUFF_MAX_LEN, fp)) {
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
        else if (0 == strcmp("SwapTotal:", key))
            info->swap_total_kb = value;
        else if (0 == strcmp("SwapFree:", key))
            info->swap_free_kb = value;
        else if (0 == strcmp("VmallocTotal:", key))
            info->vmalloc_total_kb = value;
        else if (0 == strcmp("VmallocUsed:", key))
            info->vmalloc_used_kb = value;
        else if (0 == strcmp("VmallocChunk:", key))
            info->vmalloc_chunk_kb = value;
        value = 0;
        memset(key, 0, OS_MEM_BUFF_MAX_LEN);
        memset(buff, 0, OS_MEM_BUFF_MAX_LEN);
    }

end:
    if (NULL != fp) {
        fclose(fp);
        fp = NULL;
    }

    return ret;
}

int os_mem_get_proc_info(const int32_t pid, os_mem_proc_info_t * info)
{
    int ret = 0;
    FILE * fp = NULL;
    uint64_t value = 0;
    char key[OS_MEM_BUFF_MAX_LEN] = {};
    char buff[OS_MEM_BUFF_MAX_LEN] = {};
    char path[OS_MEM_BUFF_MAX_LEN] = {};

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

    while (!feof(fp) && fgets(buff, OS_MEM_BUFF_MAX_LEN, fp)) {
        if (2 != sscanf(buff, "%s %Lu", key, &value))
            continue;

        if (0 == strcmp("VmPeak:", key))
            info->peak_kb = value;
        else if (0 == strcmp("VmSize:", key))
            info->vr_kb = value;
        else if (0 == strcmp("VmRSS:", key))
            info->rss_kb = value;
        value = 0;
        memset(key, 0, OS_MEM_BUFF_MAX_LEN);
        memset(buff, 0, OS_MEM_BUFF_MAX_LEN);
    }

end:
    if (NULL != fp) {
        fclose(fp);
        fp = NULL;
    }

    return ret;
}
