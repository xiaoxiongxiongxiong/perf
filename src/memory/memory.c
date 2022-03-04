#include "memory.h"

// 日志回调
extern void os_perf_set_last_error(const char * fmt, ...);

bool os_memory_init()
{
    return true;
}

void os_perf_memory_uninit()
{

}
