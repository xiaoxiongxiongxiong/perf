#ifndef _OS_PERF_NETWORK_H_
#define _OS_PERF_NETWORK_H_

#include <stdio.h>
#include <stdint.h>

#define OS_NET_IPV4_MAX_LEN  16
#define OS_NET_IPV6_MAX_LEN  46
#define OS_NET_INTER_MAX_LEN 64

// IPV4/IPV6信息
typedef struct _os_net_ip_config_t {
    char ipv4[OS_NET_IPV4_MAX_LEN];    // IPV4地址
    char ipv6[OS_NET_IPV6_MAX_LEN];    // IPV6地址
    char iface[OS_NET_INTER_MAX_LEN];  // 网口名称
    char mask[OS_NET_IPV4_MAX_LEN];    // 掩码 IPV4
    char mac[OS_NET_IPV6_MAX_LEN];     // mac 地址
} os_net_ip_config_t;

// 网口信息
typedef struct _os_net_info_t {
    uint32_t recv_drop_pkts;           // 接收丢包数量
    uint32_t send_drop_pkts;           // 发送丢包数量
    double rxpkts;                     // 收包速度
    double txpkts;                     // 发包速度 
    double rxkb;                       // 接收速度 单位:kb/s
    double txkb;                       // 发送速度 单位:kb/s
    char iface[OS_NET_INTER_MAX_LEN];  // 接口名称
} os_net_info_t;

/**
* os_net_init
* @brief   网络模块初始化
* @return  >=0--成功 <0--失败
*/
int os_net_init();

/**
* os_net_uninit
* @brief   网络模块销毁
*/
void os_net_uninit();

/**
* os_net_get_ip_config_info
* @brief   获取网络配置
* @param   configs  配置表指针
* @param   cnt      个数指针
* @return  0成功 <0--失败
*/
int os_net_get_ip_config_info(os_net_ip_config_t ** configs, size_t * cnt);

/**
* os_net_get_dev_info
* @brief   获取网口信息
* @param   devs  网口指针
* @param   cnt   个数指针
* @return  0成功 <0--失败
*/
int os_net_get_dev_info(os_net_info_t ** devs, size_t * cnt);

/**
* os_net_get_bandwidth_and_duplex
* @brief   根据网口名称获取双工/单双工和带宽
* @param   iface       网口名称
* @param   full        0-半双工 其它-双工
* @param   bandwidth   带宽
* @return  0成功 <0--失败
*/
int os_net_get_bandwidth_and_duplex(const char * iface, uint8_t * full, uint16_t * bandwidth);

#endif
