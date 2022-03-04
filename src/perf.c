#include "perf.h"
#include "cpu/cpu.h"
#include "disk/disk.h"
#include "memory/memory.h"
#include "network/network.h"
#include <string.h>
#include <stdarg.h>

// 采样时间 单位: 毫秒(ms)
long g_sample_time = 1000;

// 错误信息
static char g_err_msg[OS_PERF_ERROR_MAX_LEN] = { 0 };

// 设置最后一次错误信息
void os_perf_set_last_error(const char * fmt, ...);

bool os_perf_init(const uint32_t flags)
{
    if (flags & OS_PERF_DISK && !os_disk_init())
    {
        os_perf_set_last_error("initialize disk module failed");
        os_perf_uninit();
        return false;
    }

    if (flags & OS_PERF_MEMORY && !os_memory_init())
    {
        os_perf_set_last_error("initialize memory module failed");
        os_perf_uninit();
        return false;
    }

    if (flags & OS_PERF_CPU && !os_cpu_init())
    {
        os_perf_set_last_error("initialize cpu module failed");
        os_perf_uninit();
        return false;
    }

    if (flags & OS_PERF_NETWORK && !os_network_init())
    {
        os_perf_set_last_error("initialize network module failed");
        os_perf_uninit();
        return false;
    }

    return true;
}

void os_perf_uninit()
{
    os_cpu_uninit();
    os_disk_uninit();
    os_perf_memory_uninit();
    os_network_uninit();
}

void os_perf_set_callback(os_log_callback cb)
{
}

void os_perf_set_last_error(const char * fmt, ...)
{
    char buff[OS_PERF_ERROR_MAX_LEN] = { 0 };
    va_list vl;

    va_start(vl, fmt);
    vsnprintf(buff, OS_PERF_ERROR_MAX_LEN, fmt, vl);
    va_end(vl);

    //os_mutex_lock(g_perf_mtx);
    memcpy(g_err_msg, buff, OS_PERF_ERROR_MAX_LEN);
    //os_mutex_unlock(g_perf_mtx);
}

const char * os_perf_get_last_error()
{
    return g_err_msg;
}
