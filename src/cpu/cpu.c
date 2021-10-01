#include "cpu.h"
#include "libos.h"
#include "perf.h"

// 日志回调
extern void os_perf_set_last_error(const char * fmt, ...);

bool os_perf_module_cpu_init()
{
    return true;
}

void os_perf_module_cpu_uninit()
{

}
