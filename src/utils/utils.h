#ifndef _OS_PERF_UTILS_H_
#define _OS_PERF_UTILS_H_

#include <stdio.h>
#include <stdint.h>

// 读取文件内容
char * os_utils_read_file(const char * file, size_t * size);

// 获取文件行数
int os_utils_total_lines(FILE * fp);

#endif
