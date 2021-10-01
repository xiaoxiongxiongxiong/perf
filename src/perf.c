#include "perf.h"
#include "libos.h"
#include "cpu/cpu.h"
#include "disk/disk.h"
#include "memory/memory.h"
#include "network/network.h"
#include <string.h>

// 错误信息
static char g_err_msg[OS_PERF_ERROR_MAX_LEN] = { 0 };
// 线程锁
static os_mutex_t * g_perf_mtx = NULL;
// 设置最后一次错误信息
void os_perf_set_last_error(const char * fmt, ...);

bool os_perf_init(const uint32_t flags)
{
    g_perf_mtx = os_mutex_create();
    if (NULL == g_perf_mtx)
    {
        os_perf_set_last_error("create mutex failed");
        log_msg_warn("%s.", g_err_msg);
        return false;
    }

    if (flags & OS_PERF_DISK && !os_perf_module_disk_init())
    {
        os_perf_set_last_error("initialize disk module failed");
        log_msg_warn("%s.", g_err_msg);
        os_perf_uninit();
        return false;
    }

    if (flags & OS_PERF_MEMORY && !os_perf_module_memory_init())
    {
        os_perf_set_last_error("initialize memory module failed");
        log_msg_warn("%s.", g_err_msg);
        os_perf_uninit();
        return false;
    }

    if (flags & OS_PERF_CPU && !os_perf_module_cpu_init())
    {
        os_perf_set_last_error("initialize cpu module failed");
        log_msg_warn("%s.", g_err_msg);
        os_perf_uninit();
        return false;
    }

    if (flags & OS_PERF_NETWORK && !os_perf_module_network_init())
    {
        os_perf_set_last_error("initialize network module failed");
        log_msg_warn("%s.", g_err_msg);
        os_perf_uninit();
        return false;
    }

    return true;
}

void os_perf_uninit()
{
    os_perf_module_cpu_uninit();
    os_perf_module_disk_uninit();
    os_perf_module_memory_uninit();
    os_perf_module_network_uninit();

    if (NULL != g_perf_mtx)
    {
        os_mutex_destroy(&g_perf_mtx);
        g_perf_mtx = NULL;
    }
}

void os_perf_set_last_error(const char * fmt, ...)
{
    char buff[OS_PERF_ERROR_MAX_LEN] = { 0 };
    va_list vl;

    va_start(vl, fmt);
    vsnprintf(buff, OS_PERF_ERROR_MAX_LEN, fmt, vl);
    va_end(vl);

    os_mutex_lock(g_perf_mtx);
    memcpy(g_err_msg, buff, OS_PERF_ERROR_MAX_LEN);
    os_mutex_unlock(g_perf_mtx);
}

const char * os_perf_get_last_error()
{
    return g_err_msg;
}
