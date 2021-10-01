#include "disk.h"
#include "libos.h"
#include "perf.h"
#include <pthread.h>
#include <string.h>
#include <errno.h>

// 日志回调
extern void os_perf_set_last_error(const char * fmt, ...);

// 是否已初始化
static bool g_is_init = false;
// 线程
static pthread_t g_disk_thr;
// 条件变量
static pthread_cond_t g_disk_cond;
// 磁盘模块互斥锁
static os_mutex_t * g_disk_mtx = NULL;
// 磁盘信息存放表
static os_hash_table_t * g_disk_table = NULL;

bool os_perf_module_disk_init()
{
    if (g_is_init)
        return true;

    g_disk_mtx = os_mutex_create();
    if (NULL == g_disk_mtx)
    {
        os_perf_set_last_error("create mutex failed in disk module");
        return false;
    }

    g_disk_table = os_hash_table_create();
    if (NULL == g_disk_table)
    {
        os_perf_set_last_error("create hashtable failed in disk module");
        os_perf_module_disk_uninit();
        return false;
    }

    int ret = pthread_create(&g_disk_thr, NULL, NULL, NULL);
    if (0 != ret)
    {
        const int code = errno;
        os_perf_set_last_error("create thread failed for %s in disk module", strerror(code));
        return false;
    }

    return true;
}

void os_perf_module_disk_uninit()
{
    if (!g_is_init)
        return;

    g_is_init = false;
    pthread_cond_signal(&g_disk_cond);
    pthread_join(g_disk_thr, NULL);

    if (NULL != g_disk_table)
    {
        os_hash_table_destroy(&g_disk_table);
        g_disk_table = NULL;
    }

    if (NULL != g_disk_mtx)
    {
        os_mutex_destroy(&g_disk_mtx);
        g_disk_mtx = NULL;
    }
}

bool os_perf_module_get_disk_info(os_disk_info_t * infos, size_t size)
{
    if (NULL == infos || 0 == size)
    {
        os_perf_set_last_error("input param is NULL or invalid");
        return false;
    }



    return true;
}

bool os_perf_moudle_get_mount_info(os_mount_info_t * infos, size_t size)
{
    if (NULL == infos || 0 == size)
    {
        os_perf_set_last_error("input param is NULL or invalid");
        return false;
    }

    return true;
}
