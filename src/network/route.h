#ifndef _OS_PERF_ROUTE_H_
#define _OS_PERF_ROUTE_H_

#include <stdio.h>
#include <stdint.h>

#define OS_ROUTE_IPV4_MAX_LEN  16
#define OS_ROUTE_IPV6_MAX_LEN  46
#define OS_ROUTE_INTER_MAX_LEN 64

typedef enum _OS_ROUTE_MODE {
    OS_ROUTE_MODE_NONE,
    OS_ROUTE_MODE_NET,    // net
    OS_ROUTE_MODE_HOST,   // host
    OS_ROUTE_MODE_MAX
} OS_ROUTE_MODE;

// IPV4/IPV6 路由信息
typedef struct _os_route_into_t {
    int32_t metric;
    int32_t refcnt;
    int32_t use;
    char flags[OS_ROUTE_IPV4_MAX_LEN];   // flags
    char iface[OS_ROUTE_INTER_MAX_LEN];  // 网口
    char mask[OS_ROUTE_IPV4_MAX_LEN];    // 掩码，仅IPV4有
    union target {
        char ipv4[OS_ROUTE_IPV4_MAX_LEN];
        char ipv6[OS_ROUTE_IPV6_MAX_LEN];
    } target;
    union gateway {
        char ipv4[OS_ROUTE_IPV4_MAX_LEN];
        char ipv6[OS_ROUTE_IPV6_MAX_LEN];
    } gateway;
} os_route_ipv4_info_t, os_route_ipv6_info_t;

/**
* os_route_ipv4_ctrl
* @brief   IPV4路由操作
* @param   params   IPV4信息结构体指针
* @param   add      1-添加路由，0-删除路由
* @return  >=0--成功 <0--失败
*/
int os_route_ipv4_ctrl(const os_route_ipv4_info_t * params, OS_ROUTE_MODE mode, int add);

/**
* os_route_ipv6_ctrl
* @brief   IPV6路由操作
* @param   params   IPV6信息结构体指针
* @param   add      1-添加路由，0-删除路由
* @return  >=0--成功 <0--失败
*/
int os_route_ipv6_ctrl(const os_route_ipv6_info_t * params, int add);

/**
* os_route_get_ipv4_list
* @brief   获取IPV4路由表
* @param   lst      IPV4路由表指针
* @return  >=0--路由表长度 <0--失败
*/
int os_route_get_ipv4_list(os_route_ipv4_info_t ** lst);

/**
* os_route_get_ipv6_list
* @brief   获取IPV6路由表
* @param   lst      IPV6路由表指针
* @return  >=0--路由表长度 <0--失败
*/
int os_route_get_ipv6_list(os_route_ipv6_info_t ** lst);

/**
* os_route_freep
* @brief   释放IPV4/6路由表指针
* @param   ptr      IPV4/6路由表指针
*/
void os_route_freep(void * ptr);

#endif
