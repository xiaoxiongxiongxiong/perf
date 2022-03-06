#ifndef _OS_UTILS_H_
#define _OS_UTILS_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// 读取文件内容
char * os_utils_read_file(const char * file, size_t * size);

#endif
