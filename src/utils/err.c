#include "err.h"
#include <string.h>

#define OS_PERF_ARRAY_ELEMS(a) (sizeof(a) / sizeof((a)[0]))

struct os_err_t {
    int code;
    const char * str;
};

static const struct os_err_t os_errs[] = {
    {OS_PERF_ERROR(EPERM),   "Operation not permitted"},
    {OS_PERF_ERROR(ENOENT),  "No such file or directory"},
    {OS_PERF_ERROR(ESRCH),   "No such process"},
    {OS_PERF_ERROR(EINTR),   "Interrupted system call"},
    {OS_PERF_ERROR(EIO),     "I/O error"},
    {OS_PERF_ERROR(ENXIO),   "No such device or address"},
    {OS_PERF_ERROR(E2BIG),   "Argument list too long"},
    {OS_PERF_ERROR(ENOEXEC), "Exec format error"},
    {OS_PERF_ERROR(EBADF),   "Bad file number"},
    {OS_PERF_ERROR(ECHILD),  "No child processes"},
    {OS_PERF_ERROR(EAGAIN),  "Try again"},
    {OS_PERF_ERROR(ENOMEM),  "Out of memory"},
    {OS_PERF_ERROR(EACCES),  "Permission denied"},
    {OS_PERF_ERROR(EFAULT),  "Bad address"},
    {OS_PERF_ERROR(ENOTBLK), "Block device required"},
    {OS_PERF_ERROR(EBUSY),   "Device or resource busy"},
    {OS_PERF_ERROR(EEXIST),  "File exists"},
    {OS_PERF_ERROR(EXDEV),   "Cross-device link"},
    {OS_PERF_ERROR(ENODEV),  "No such device"},
    {OS_PERF_ERROR(ENOTDIR), "Not a directory"},
    {OS_PERF_ERROR(EISDIR),  "Is a directory"},
    {OS_PERF_ERROR(EINVAL),  "Invalid argument"},
    {OS_PERF_ERROR(ENFILE),  "File table overflow"},
    {OS_PERF_ERROR(EMFILE),  "Too many open files"},
    {OS_PERF_ERROR(ENOTTY),  "Not a typewriter"},
    {OS_PERF_ERROR(ETXTBSY), "Text file busy"},
    {OS_PERF_ERROR(EFBIG),   "File too large"},
    {OS_PERF_ERROR(ENOSPC),  "No space left on device"},
    {OS_PERF_ERROR(ESPIPE),  "Illegal seek"},
    {OS_PERF_ERROR(EROFS),   "Read-only file system"},
    {OS_PERF_ERROR(EMLINK),  "Too many links"},
    {OS_PERF_ERROR(EPIPE),   "Broken pipe"},
    {OS_PERF_ERROR(EDOM),    "Math argument out of domain of func"},
    {OS_PERF_ERROR(ERANGE),  "Math result not representable"},
    {OS_PERF_ERR_NULLPTR,    "Param is nullptr"},
    {OS_PERF_ERR_INVALID,    "Invalid data"},
    {OS_PERF_ERR_NOREADY,    "Not initialized"},
};

int os_perf_make_error_string(int code, char * buf, const size_t size)
{
    int ret = 0, i;
    const struct os_err_t * entry = NULL;

    for (i = 0; i < OS_PERF_ARRAY_ELEMS(os_errs); i++) {
        if (code == os_errs[i].code) {
            entry = &os_errs[i];
            break;
        }
    }
    if (entry) {
        strncpy(buf, entry->str, size);
    } else {
        ret = -1;
        snprintf(buf, size, "Error number %d occurred", code);
    }

    return ret;
}
