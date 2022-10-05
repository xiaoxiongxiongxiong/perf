#include "network.h"

// 日志回调
extern void os_perf_set_last_error(const char * fmt, ...);

bool os_net_init()
{
    return true;
}

void os_net_uninit()
{

}
