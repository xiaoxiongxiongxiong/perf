#include "os_utils.h"
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define OS_UTILS_CACHE_MAX 4096  // 缓冲最大长度

char * os_utils_read_file(const char * file, size_t * size)
{
    if (NULL == file)
        return NULL;

    int fd = open(file, O_RDONLY);
    if (fd < 0)
    {
        return NULL;
    }

    char * buff = (char *)malloc(OS_UTILS_CACHE_MAX);
    if (NULL == buff)
    {
        close(fd);
        return NULL;
    }
    memset(buff, 0, OS_UTILS_CACHE_MAX);

    char cache[OS_UTILS_CACHE_MAX] = { 0 };
    ssize_t bytes = 0;
    size_t total = 0;
    do
    {
        bytes = read(fd, cache, OS_UTILS_CACHE_MAX);
        if (bytes <= 0)
            break;

        const size_t len = (size_t)bytes;
        total += len;
        if (bytes < OS_UTILS_CACHE_MAX && total == len)
        {
            memcpy(buff, cache, len);
            break;
        }

        char * ptr = (char *)realloc(buff, total + 1);
        if (NULL == ptr)
        {
            free(buff);
            close(fd);
            return NULL;
        }
        buff = ptr;
        strncat(buff, cache, len);
        buff[total] = '\0';
    } while (bytes > 0);

    close(fd);
    if (size)
        *size = total;

    return buff;
}
