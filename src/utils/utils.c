#include "utils.h"
#include "err.h"
#include <string.h>
#include <malloc.h>

double os_utils_q2d(int64_t num, int64_t den)
{
    return (double)num / (double)den;
}

void os_utils_freep(void * ptr)
{
    void * val;

    memcpy(&val, ptr, sizeof(val));
    memcpy(ptr, &(void *){ NULL }, sizeof(val));
    free(val);
}

int os_utils_reallocp(void * ptr, size_t size)
{
    void * val;

    if (!size) {
        os_utils_freep(ptr);
        return 0;
    }

    memcpy(&val, ptr, sizeof(val));
    val = realloc(val, size);

    if (!val) {
        os_utils_freep(ptr);
        return OS_PERF_ERROR(ENOMEM);
    }

    memcpy(ptr, &val, sizeof(val));
    return 0;
}
