#ifndef _OS_PERF_ERR_H_
#define _OS_PERF_ERR_H_

#include <stdio.h>
#include <errno.h>

#define OS_PERF_ERROR(e) (-(e))
#define OS_MKTAG(a,b,c,d) ((a) | ((b) << 8) | ((c) << 16) | ((unsigned)(d) << 24))
#define OS_PERF_MKTAG(a,b,c,d) (-(int)OS_MKTAG(a, b, c, d))

#define OS_PERF_ERR_NULLPTR OS_PERF_MKTAG('N','U','L','L')
#define OS_PERF_ERR_INVALID OS_PERF_MKTAG('I','V','D','A')
#define OS_PERF_ERR_NOREADY OS_PERF_MKTAG('N','R','D','Y')

#define OS_PERF_ERROR_MAX_SIZE 64 // 错误信息最大长度

int os_perf_make_error_string(int code, char * buf, size_t size);

#endif
