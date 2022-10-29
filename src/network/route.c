#include "route.h"
#include "utils/config.h"
#include "utils/err.h"
#include "utils/linklist.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <net/route.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#ifndef __USE_XOPEN2K
#define __USE_XOPEN2K
#endif
#include <sys/socket.h>
#include <netdb.h>
#ifndef __USE_MISC
#define __USE_MISC
#endif
#include <net/if.h>

static void os_route_flag(uint32_t flag, char * data);
static char * os_route_ipv6_destination(const char * target, char * dst);

int os_route_ipv4_ctrl(const os_route_ipv4_info_t * params, const OS_ROUTE_MODE mode, const int add)
{
    int fd = -1;
    int ret;
    size_t flag;
    struct rtentry routes = {};
    struct sockaddr_in * addr = NULL;

    if (NULL == params) {
        ret = OS_PERF_ERR_NULLPTR;
        goto end;
    }

    assert(OS_ROUTE_MODE_HOST == mode || OS_ROUTE_MODE_NET == mode);
    if (OS_ROUTE_MODE_HOST != mode && OS_ROUTE_MODE_NET != mode) {
        ret = OS_PERF_ERR_INVALID;
        goto end;
    }

    if ('\0' != params->target.ipv4[0]) {
        addr = (struct sockaddr_in *)&routes.rt_dst;
        addr->sin_family = AF_INET;
        addr->sin_addr.s_addr = inet_addr(params->target.ipv4);
        if (OS_ROUTE_MODE_HOST == mode)
            routes.rt_flags |= RTF_HOST;
    }

    if ('\0' != params->mask[0]) {
        addr = (struct sockaddr_in *)&routes.rt_genmask;
        addr->sin_family = AF_INET;
        addr->sin_addr.s_addr = inet_addr(params->mask);
    }

    if ('\0' != params->gateway.ipv4[0]) {
        addr = (struct sockaddr_in *)&routes.rt_gateway;
        addr->sin_family = AF_INET;
        addr->sin_addr.s_addr = inet_addr(params->gateway.ipv4);
        routes.rt_flags |= RTF_GATEWAY;
    }

    if ('\0' != params->iface[0]) {
        routes.rt_dev = (char *)params->iface;
    }

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        ret = OS_PERF_ERROR(errno);
        goto end;
    }

    flag = add ? SIOCADDRT : SIOCDELRT;
    if (ioctl(fd, flag, &routes) < 0) {
        ret = OS_PERF_ERROR(errno);
    }

end:
    if (fd > 0) {
        close(fd);
        fd = -1;
    }

    return ret;
}

int os_route_ipv6_ctrl(const os_route_ipv6_info_t * params, int add)
{
    int ret;
    int fd = -1;
    int prefix_len;
    size_t flag;
    char * ptr = NULL;
    struct sockaddr_in6 addr = {};
    struct sockaddr_in6 * sin6 = NULL;
    struct addrinfo req = {};
    struct addrinfo * ai = NULL;
    struct ifreq ifr = {};
    struct in6_rtmsg rtmsg;

    if (NULL == params) {
        ret = OS_PERF_ERR_NULLPTR;
        goto end;
    }

    if (0 == strcmp(params->target.ipv6, "default")) {
        prefix_len = 0;
        memset(&addr, 0, sizeof(addr));
    } else {
        ptr = strchr(params->target.ipv6, '/');
        if (ptr) {
            prefix_len = atoi(ptr + 1);
            if ((prefix_len < 0) || (prefix_len > 128)) {
                ret = OS_PERF_ERR_INVALID;
                goto end;
            }
            *ptr = '\0';
        } else {
            prefix_len = 128;
        }

        sin6 = (struct sockaddr_in6 *)&addr;
        sin6->sin6_family = AF_INET6;
        sin6->sin6_port = 0;
        if (inet_pton(AF_INET6, params->target.ipv6, sin6->sin6_addr.s6_addr) <= 0) {
            ret = OS_PERF_ERROR(errno);
            goto end;
        }

        req.ai_family = AF_INET6;
        if (getaddrinfo(params->target.ipv6, NULL, &req, &ai)) {
            ret = errno;
            goto end;
        }
        memcpy(sin6, ai->ai_addr, sizeof(struct sockaddr_in6));
        freeaddrinfo(ai);
    }

    memset((char *)&rtmsg, 0, sizeof(struct in6_rtmsg));
    memcpy(&rtmsg.rtmsg_dst, addr.sin6_addr.s6_addr, sizeof(struct in6_addr));

    rtmsg.rtmsg_flags = RTF_UP;
    if (prefix_len == 128)
        rtmsg.rtmsg_flags |= RTF_HOST;
    rtmsg.rtmsg_metric = 1;
    rtmsg.rtmsg_dst_len = (uint16_t)prefix_len;

    if ('\0' != params->gateway.ipv6[0]) {
        if (rtmsg.rtmsg_flags & RTF_GATEWAY) {
            ret = OS_PERF_ERR_INVALID;
            goto end;
        }

        sin6 = (struct sockaddr_in6 *)&addr;
        sin6->sin6_family = AF_INET6;
        sin6->sin6_port = 0;
        if (inet_pton(AF_INET6, params->gateway.ipv6, sin6->sin6_addr.s6_addr) <= 0) {
            ret = OS_PERF_ERROR(errno);
            goto end;
        }

        memcpy(&rtmsg.rtmsg_gateway, addr.sin6_addr.s6_addr, sizeof(struct in6_addr));
        rtmsg.rtmsg_flags |= RTF_GATEWAY;
    }

    fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (fd < 0) {
        ret = OS_PERF_ERROR(errno);
        goto end;
    }

    if ('\0' != params->iface[0]) {
        memcpy(ifr.ifr_name, params->iface, IFNAMSIZ);
        if (ioctl(fd, SIOGIFINDEX, &ifr) < 0) {
            ret = OS_PERF_ERROR(errno);
            goto end;
        }
        rtmsg.rtmsg_ifindex = ifr.ifr_ifindex;
    }

    flag = add ? SIOCADDRT : SIOCDELRT;
    if (ioctl(fd, flag, &rtmsg) < 0) {
        ret = OS_PERF_ERROR(errno);
    }

end:
    if (fd > 0) {
        close(fd);
        fd = -1;
    }
    return ret;
}

int os_route_get_ipv4_list(os_route_ipv4_info_t ** lst)
{
    int ret = -1;
    char buff[1024] = {};
    os_route_ipv4_info_t route = {};
    os_dlist_t * cache = NULL;
    uint32_t target = 0;
    uint32_t gateway = 0;
    uint32_t netmask = 0;
    uint32_t flag = 0;

    FILE * fp = fopen(OS_PERF_ROUTE_IPV4_PATH, "rb");
    if (NULL == fp) {
        ret = OS_PERF_ERROR(errno);
        goto end;
    }

    cache = os_dlist_create(sizeof(os_route_ipv4_info_t));
    if (NULL == cache) {
        ret = OS_PERF_ERROR(errno);
        goto end;
    }

    while (!feof(fp) && fgets(buff, 1023, fp)) {
        if (0 == strncmp(buff, "Iface", 5) || '\0' == buff)
            continue;

        memset(&route, 0, sizeof(os_route_ipv4_info_t));
        if (8 != sscanf(buff, "%16s\t%081X\t%081X\t%041X\t%d\t%d\t%d\t%081X\t%*d\t%*d\t%*d", &route.iface,
                        &target, &gateway, &flag, &route.refcnt, &route.use, &route.metric, &netmask)) {
            continue;
        }

        inet_ntop(AF_INET, (void *)&gateway, route.gateway.ipv4, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, (void *)&target, route.target.ipv4, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, (void *)&netmask, route.mask, INET_ADDRSTRLEN);
        os_route_flag(flag, route.flags);
        os_dlist_add(cache, NULL, &route);
        memset(buff, 0, 1024);
    }

    fclose(fp);
    fp = NULL;

    ret = (int)os_dlist_size(cache);
    if (ret < 1) {
        os_dlist_destroy(&cache);
        return ret;
    }

    *lst = (os_route_ipv4_info_t *)calloc((size_t)ret, sizeof(os_route_ipv4_info_t));
    if (NULL == *lst) {
        ret = OS_PERF_ERROR(errno);
        goto end;
    }

    os_dlist_node_t * head = os_dlist_head(cache);
    os_route_ipv4_info_t * data = (os_route_ipv4_info_t *)os_dlist_getdata(head);
    for (int i = 0; i < ret; ++i) {
        memcpy(&(*lst)[i], data, sizeof(os_route_ipv4_info_t));
        head = os_dlist_next(head);
        data = (os_route_ipv4_info_t *)os_dlist_getdata(head);
    }

end:
    if (NULL != fp) {
        fclose(fp);
        fp = NULL;
    }

    if (NULL != cache) {
        os_dlist_destroy(&cache);
    }

    return ret;
}

int os_route_get_ipv6_list(os_route_ipv6_info_t ** lst)
{
    int ret = -1;
    char buff[1024] = {};
    os_route_ipv6_info_t route = {};
    os_dlist_t * cache = NULL;
    uint32_t prefix_len = 0;
    uint32_t flag = 0;
    char tmp[INET6_ADDRSTRLEN] = {};
    char target[INET6_ADDRSTRLEN] = {};

    FILE * fp = fopen(OS_PERF_ROUTE_IPV6_PATH, "rb");
    if (NULL == fp) {
        ret = OS_PERF_ERROR(errno);
        goto end;
    }

    cache = os_dlist_create(sizeof(os_route_ipv6_info_t));
    if (NULL == cache) {
        ret = OS_PERF_ERROR(errno);
        goto end;
    }

    while (!feof(fp) && fgets(buff, 1023, fp)) {
        memset(tmp, 0, 40);
        memset(&route, 0, sizeof(os_route_ipv6_info_t));
        if (7 != sscanf(buff, "%32s\t%021X\t%*0321X\t%*021X\t%*0321X\t%081X\t%081X\t%081X\t%081X\t%16s",
                        &target, &prefix_len, &route.metric, &route.refcnt, &route.use, &flag, &route.iface)) {
            continue;
        }

        os_route_flag(flag, route.flags);
        os_route_ipv6_destination(target, tmp);
        snprintf(route.target.ipv6, OS_NET_IPV6_MAX_LEN, "%s/%u", tmp, prefix_len);
        os_dlist_add(cache, NULL, &route);
        memset(buff, 0, 1024);
    }

    fclose(fp);
    fp = NULL;

    ret = (int)os_dlist_size(cache);
    if (ret < 1) {
        os_dlist_destroy(&cache);
        return ret;
    }

    *lst = (os_route_ipv6_info_t *)calloc((size_t)ret, sizeof(os_route_ipv6_info_t));
    if (NULL == *lst) {
        ret = OS_PERF_ERROR(errno);
        goto end;
    }

    os_dlist_node_t * head = os_dlist_head(cache);
    os_route_ipv6_info_t * data = (os_route_ipv6_info_t *)os_dlist_getdata(head);
    for (int i = 0; i < ret; ++i) {
        memcpy(&(*lst)[i], data, sizeof(os_route_ipv6_info_t));
        head = os_dlist_next(head);
        data = (os_route_ipv6_info_t *)os_dlist_getdata(head);
    }

end:
    if (NULL != fp) {
        fclose(fp);
        fp = NULL;
    }

    if (NULL != cache) {
        os_dlist_destroy(&cache);
    }
    return ret;
}

void os_route_flag(const uint32_t flag, char * data)
{
    if (flag & RTF_REJECT)
        strcat(data, "!");
    if (flag & RTF_UP)
        strcat(data, "U");
    if (flag & RTF_GATEWAY)
        strcat(data, "G");
    if (flag & RTF_HOST)
        strcat(data, "H");
    if (flag & RTF_REINSTATE)
        strcat(data, "R");
    if (flag & 0x0010)
        strcat(data, "D");
    if (flag & RTF_MODIFIED)
        strcat(data, "M");
    if (flag & RTF_ADDRCONF)
        strcat(data, "A");
    if (flag & RTF_CACHE)
        strcat(data, "C");
    if (flag & RTF_NONEXTHOP)
        strcat(data, "n");
}

char * os_route_ipv6_destination(const char * target, char * dst)
{
    const char * ptr = target;
    bool exist = false;
    while (*ptr != '\0') {
        if (0 == strncmp(ptr, "0000", 4)) {
            if (exist) {
                ptr += 4;
                continue;
            }

            if (ptr == target)
                strcat(dst, "::");
            else
                strcat(dst, ":");
            exist = true;
        } else {
            int i = 0;
            for (i = 0; i < 4; i++) {
                if (*(ptr + i) != '0')
                    break;
            }

            strncat(dst, ptr + i, (size_t)(4 - i));
            if (*(ptr + 4) != '\0')
                strcat(dst, ":");
        }
        ptr += 4;
    }

    return dst;
}
