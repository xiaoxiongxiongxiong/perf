#include "network.h"
#include "utils/err.h"
#include "utils/config.h"
#include "utils/utils.h"
#include "utils/linklist.h"
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <pthread.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/ethtool.h>
#include <linux/sockios.h>
#ifndef __USE_MISC
#define __USE_MISC
#endif
#include <net/if.h>

static int g_inited = 0;
static pthread_t g_net_thr = 0;
static pthread_mutex_t g_net_mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_net_cond = PTHREAD_COND_INITIALIZER;
static os_dlist_t * g_net_dev_lst = NULL;

typedef struct _os_net_iface_t {
    uint32_t recv_pkts;      // 接受包数量
    uint32_t send_pkts;      // 发送包数量
    uint32_t recv_err_pkts;  // 接收错误数量
    uint32_t send_err_ptks;  // 发送错误数量
    uint64_t recv_bytes;     // 接收byte数量
    uint64_t send_bytes;     // 发送byte数量
    os_net_info_t info;      // 网口信息
} os_net_iface_t;

// 获取网口信息
static int os_net_get_iface_info(os_dlist_t ** lst);

// 网络数据处理线程
static void os_net_thr(void * args);

int os_net_init()
{
    int ret = 0;
    if (g_inited)
        return ret;

    if (0 != (ret = os_net_get_iface_info(&g_net_dev_lst))) {
        return ret;
    }

    ret = pthread_mutex_init(&g_net_mtx, NULL);
    if (ret < 0) {
        ret = OS_PERF_ERROR(errno);
        goto fail;
    }

    ret = pthread_cond_init(&g_net_cond, NULL);
    if (ret < 0) {
        pthread_mutex_destroy(&g_net_mtx);
        ret = OS_PERF_ERROR(errno);
        goto fail;
    }

    g_inited = 1;
    ret = pthread_create(&g_net_thr, NULL, (void *)os_net_thr, NULL);
    if (ret < 0) {
        pthread_mutex_destroy(&g_net_mtx);
        pthread_cond_destroy(&g_net_cond);
        ret = OS_PERF_ERROR(errno);
        goto fail;
    }

    return 0;

fail:
    g_inited = 0;

    if (NULL != g_net_dev_lst) {
        os_dlist_destroy(&g_net_dev_lst);
    }

    return ret;
}

void os_net_uninit()
{
    if (!g_inited)
        return;

    g_inited = 0;
    pthread_cond_signal(&g_net_cond);
    pthread_join(g_net_thr, NULL);
    pthread_mutex_destroy(&g_net_mtx);
    pthread_cond_destroy(&g_net_cond);
    os_dlist_destroy(&g_net_dev_lst);
}

int os_net_get_config_info(os_net_ip_config_t ** configs)
{
    int ret = 0;
    struct ifaddrs * if_addr = NULL;
    struct ifaddrs * addr = NULL;
    os_net_ip_config_t ic = {};

    if (0 != getifaddrs(&if_addr)) {
        ret = OS_PERF_ERROR(errno);
        return ret;
    }
    addr = if_addr;

    while (if_addr) {
        if (NULL == if_addr->ifa_addr) {
            if_addr = if_addr->ifa_next;
            continue;
        }

        memset(&ic, 0, sizeof(ic));
        memcpy(ic.iface, if_addr->ifa_name, OS_NET_IPV4_MAX_LEN);
        if (AF_INET == if_addr->ifa_addr->sa_family) {
            void * addr_ptr = &((struct sockaddr_in *)if_addr->ifa_addr)->sin_addr;
            if (addr_ptr)
                inet_ntop(AF_INET, addr_ptr, ic.ipv4, INET_ADDRSTRLEN);

            void * mask_ptr = &((struct sockaddr_in *)if_addr->ifa_netmask)->sin_addr;
            if (mask_ptr)
                inet_ntop(AF_INET, mask_ptr, ic.mask, INET_ADDRSTRLEN);
            os_net_get_mac(ic.iface, ic.mac);
            printf("ipv4 iface:%s\tip:%s\tmask:%s\n", ic.iface, ic.ipv4, ic.mask);
        } else if (if_addr->ifa_addr->sa_family == AF_INET6) {
            void * addr_ptr = &((struct sockaddr_in6 *)if_addr->ifa_addr)->sin6_addr;
            if (addr_ptr)
                inet_ntop(AF_INET6, addr_ptr, ic.ipv6, INET6_ADDRSTRLEN);
            printf("ipv6 iface:%s\tip:%s\n", ic.iface, ic.ipv6);
        }
        if_addr = if_addr->ifa_next;
    }

    freeifaddrs(addr);
    addr = NULL;

    return ret;
}

int os_net_get_dev_info(os_net_info_t ** devs, size_t * cnt)
{
    int ret = 0;
    os_dlist_node_t * node = NULL;
    os_net_iface_t * iface = NULL;

    if (NULL == devs || NULL != *devs || NULL == cnt) {
        return OS_PERF_ERR_INVALID;
    }

    if (!g_inited) {
        return OS_PERF_ERR_NOREADY;
    }

    pthread_mutex_lock(&g_net_mtx);
    *cnt = os_dlist_size(g_net_dev_lst);
    if (0 != (ret = os_utils_reallocp(devs, *cnt * sizeof(os_net_info_t)))) {
        pthread_mutex_unlock(&g_net_mtx);
        return ret;
    }
    node = os_dlist_head(g_net_dev_lst);
    for (int i = 0; i < *cnt; ++i) {
        iface = (os_net_iface_t *)os_dlist_getdata(node);
        memcpy(&(*devs)[i], &iface->info, sizeof(os_net_info_t));
        node = os_dlist_next(node);
    }
    pthread_mutex_unlock(&g_net_mtx);

    return 0;
}

int os_net_get_bandwidth_and_duplex(const char * iface, uint8_t * full, uint16_t * bandwidth)
{
    int ret = 0;
    int fd = -1;
    struct ifreq req = {};
    struct ethtool_cmd ecmd = {};

    if (NULL == iface || NULL == full || NULL == bandwidth) {
        return OS_PERF_ERR_NULLPTR;
    }

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == fd) {
        return OS_PERF_ERROR(errno);
    }

    ecmd.cmd = ETHTOOL_GSET;
    req.ifr_data = (char *)&ecmd;
    strncpy(req.ifr_name, iface, IFNAMSIZ);
    if (0 == ioctl(fd, SIOCETHTOOL, &req)) {
        *full = (DUPLEX_FULL == ecmd.duplex);
        *bandwidth = ecmd.speed;
    } else {
        ret = OS_PERF_ERROR(errno);
    }
    close(fd);

    return ret;
}

int os_net_get_mac(const char * iface, char * mac)
{
    int ret = 0;
    uint8_t * tmp = NULL;
    struct ifreq req = {};

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == fd) {
        ret = OS_PERF_ERROR(errno);
        return ret;
    }

    strncpy(req.ifr_name, iface, IFNAMSIZ);
    if (0 == ioctl(fd, SIOCGIFHWADDR, &req)) {
        tmp = req.ifr_hwaddr.sa_data;
        snprintf(mac, OS_NET_IPV4_MAX_LEN, "%02X:%02X:%02X:%02X:%02X:%02X",
                 tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);
    } else {
        ret = OS_PERF_ERROR(errno);
    }
    close(fd);

    return ret;
}

int os_net_get_iface_info(os_dlist_t ** lst)
{
    int ret = 0;
    FILE * fp = NULL;
    os_net_iface_t dev = {};
    char iface[OS_NET_IPV4_MAX_LEN] = { 0 };
    char buff[OS_PERF_LINE_MAX] = { 0 };

    *lst = os_dlist_create(sizeof(os_net_iface_t));
    if (NULL == *lst) {
        ret = OS_PERF_ERROR(ENOMEM);
        goto end;
    }

    fp = fopen(OS_PERF_NET_DEV_PATH, "rb");
    if (NULL == fp) {
        ret = OS_PERF_ERROR(errno);
        goto end;
    }

    while (!feof(fp) && fgets(buff, OS_PERF_LINE_MAX - 1, fp)) {
        if (NULL == strstr(buff, ":")) {
            memset(buff, 0, OS_PERF_LINE_MAX);
            continue;
        }

        if (9 != sscanf(buff, "%s %lu %d %d %d %*s %*s %*s %*s %lu %d %d %d",
                        &iface, &dev.recv_bytes, &dev.recv_pkts, &dev.recv_err_pkts,
                        &dev.info.recv_drop_pkts, &dev.send_bytes, &dev.send_pkts,
                        &dev.send_err_ptks, &dev.info.send_drop_pkts)) {
            ret = OS_PERF_ERR_INVALID;
            break;
        }

        strncpy(dev.info.iface, strtok(iface, ":"), OS_NET_IPV4_MAX_LEN - 1);
        if (!os_dlist_add(*lst, NULL, (void *)&dev)) {
            ret = OS_PERF_ERROR(ENOMEM);
            break;
        }
    }

end:
    if (NULL != fp) {
        fclose(fp);
        fp = NULL;
    }

    if (0 != ret && NULL != *lst) {
        os_dlist_destroy(lst);
        *lst = NULL;
    }

    return ret;
}

void os_net_thr(void * args)
{
    while (g_inited) {
        os_dlist_t * dev_lst = NULL;
        struct timespec ts = {};
        struct timeval tv = {};
        gettimeofday(&tv, NULL);
        pthread_mutex_lock(&g_net_mtx);
        ts.tv_sec = tv.tv_sec + 1;
        ts.tv_nsec = tv.tv_usec * 1000 + 1000000;
        pthread_cond_timedwait(&g_net_cond, &g_net_mtx, &ts);

        if (0 != os_net_get_iface_info(&dev_lst)) {
            g_inited = 0;
            break;
        }

        int succ = 1;
        os_dlist_node_t * node = os_dlist_head(g_net_dev_lst);
        os_dlist_node_t * tmp_node = os_dlist_head(dev_lst);
        while (node && tmp_node) {
            os_net_iface_t * data = (os_net_iface_t *)os_dlist_getdata(node);
            os_net_iface_t * tmp_data = (os_net_iface_t *)os_dlist_getdata(tmp_node);

            if (0 != strcmp(data->info.iface, tmp_data->info.iface)) {
                fprintf(stderr, "system error\n");
                succ = 0;
                break;
            }

            data->info.rxkb = os_utils_q2d((int64_t)(tmp_data->recv_bytes - data->recv_bytes), 1024L);
            data->info.txkb = os_utils_q2d((int64_t)(tmp_data->send_bytes - data->send_bytes), 1024L);
            data->info.rxpkts = tmp_data->recv_pkts - data->recv_pkts;
            data->info.txpkts = tmp_data->send_pkts - data->send_pkts;
            data->recv_bytes = tmp_data->recv_bytes;
            data->send_bytes = tmp_data->send_bytes;
            data->recv_pkts = tmp_data->recv_pkts;
            data->send_pkts = tmp_data->send_pkts;

            node = os_dlist_next(node);
            tmp_node = os_dlist_next(tmp_node);
        }

        pthread_mutex_unlock(&g_net_mtx);
        os_dlist_destroy(&dev_lst);

        if (1 != succ) {
            g_inited = 0;
            break;
        }
    }
}
