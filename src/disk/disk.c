#include "disk.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <unistd.h>
#include <mntent.h>
#include <sys/vfs.h>

#include "../utils/os_linklist.h"

// 日志回调
extern void os_perf_set_last_error(const char * fmt, ...);

bool os_disk_init()
{
    return true;
}

void os_disk_uninit()
{
}

bool os_disk_get_partitions(os_disk_info_t ** partitions, size_t * num)
{
    if (NULL == partitions || NULL != *partitions)
    {
        os_perf_set_last_error("Input param is invalid");
        return false;
    }

    if (0 != access(OS_PERF_PARTITIONS_PATH, F_OK))
    {
        os_perf_set_last_error("Path %s isn't exist", OS_PERF_PARTITIONS_PATH);
        return false;
    }

    return true;
}

bool os_disk_get_partitions_stats(os_disk_stat_t ** stats, size_t * num)
{
    if (NULL == stats || NULL != *stats)
    {
        os_perf_set_last_error("Input param is invalid");
        return false;
    }

    return true;
}

bool os_disk_get_mnt(os_mnt_info_t ** mnt, size_t * num)
{
    if (NULL == mnt || NULL != *mnt)
    {
        os_perf_set_last_error("Input param is invalid");
        return false;
    }

    os_dlist_t * lst = os_dlist_create(sizeof(os_mnt_info_t));
    if (NULL == lst)
    {
        const int code = errno;
        os_perf_set_last_error("%s", strerror(code));
        return false;
    }

    FILE * fp = setmntent(OS_PERF_MOUNT_PATH, "r");
    if (NULL == fp)
    {
        os_perf_set_last_error("Read %s failed", OS_PERF_MOUNT_PATH);
        os_dlist_destroy(&lst);
        return false;
    }

    struct mntent * mount_entry = NULL;
    while (NULL != (mount_entry = getmntent(fp)))
    {
        if (0 == strcmp(mount_entry->mnt_fsname, "rootfs"))
        {
            continue;
        }

        struct statfs fs = {};
        if (0 != statfs(mount_entry->mnt_dir, &fs) || fs.f_blocks <= 0)
        {
            continue;
        }

        os_mnt_info_t mi = { };
        memcpy(mi.dev, mount_entry->mnt_fsname, OS_DISK_NAME_MAX);
        memcpy(mi.point, mount_entry->mnt_dir, OS_DISK_PATH_MAX);
        mi.total = fs.f_blocks * (uint64_t)fs.f_bsize;
        mi.used = (fs.f_blocks - fs.f_bfree) * (uint64_t)fs.f_bsize;
        mi.avail = fs.f_bfree * (uint64_t)fs.f_bsize;
        mi.usage = (double)mi.used / (double)mi.total;
        os_dlist_add(lst, NULL, (void *)&mi);
    }
    endmntent(fp);
    fp = NULL;

    const size_t cnt = os_dlist_size(lst);
    bool succ = false;
    do
    {
        if (0 == cnt)
        {
            succ = true;
            break;
        }

        *mnt = (os_mnt_info_t *)calloc(cnt, sizeof(os_mnt_info_t));
        if (NULL == *mnt)
        {
            const int code = errno;
            os_perf_set_last_error("%s", strerror(code));
            break;
        }

        int index = 0;
        os_dlist_node_t * head = os_dlist_head(lst);
        while (NULL != head)
        {
            os_mnt_info_t * info = (os_mnt_info_t *)os_dlist_getdata(head);
            memcpy((*mnt)[index].dev, info->dev, OS_DISK_NAME_MAX);
            memcpy((*mnt)[index].point, info->point, OS_DISK_PATH_MAX);
            (*mnt)[index].total = info->total;
            (*mnt)[index].used = info->used;
            (*mnt)[index].avail = info->avail;
            (*mnt)[index].usage = info->usage;
            ++index;
            head = os_dlist_next(head);
        }

        succ = true;
    } while (false);

    os_dlist_destroy(&lst);
    if (NULL != num)
        *num = cnt;

    return succ;
}

void os_disk_free(void * ptr)
{
    if (NULL != ptr)
    {
        free(ptr);
    }
}
