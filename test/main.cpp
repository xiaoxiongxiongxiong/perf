#include "perf.h"
#include <stdlib.h>

int main(int argc, char * argv[])
{
    if (!os_perf_init(OS_PERF_DISK))
    {
        printf("os_perf_init failed for %s\n", os_perf_get_last_error());
        return EXIT_FAILURE;
    }

    // TODO 函数使用例程

    os_perf_uninit();

    return EXIT_SUCCESS;
}
