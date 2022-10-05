#include "network/route.h"
#include "utils/err.h"

static void get_ipv4_list_test();
static void ipv4_add_test();
static void ipv4_del_test();
static void get_ipv6_list_test();
static void ipv6_add_test();
static void ipv6_del_test();

int main(int argc, char * argv[])
{
    get_ipv4_list_test();
    ipv4_add_test();
    get_ipv4_list_test();
    ipv4_del_test();
    get_ipv4_list_test();

    get_ipv6_list_test();
    ipv6_add_test();
    get_ipv6_list_test();
    ipv6_del_test();
    get_ipv6_list_test();

    return 0;
}

void get_ipv4_list_test()
{
    os_route_ipv4_info_t * lst = NULL;
    int cnt = os_route_get_ipv4_list(&lst);
    if (cnt < 0) {
        char buff[OS_PERF_ERROR_MAX_SIZE] = { 0 };
        os_perf_make_error_string(cnt, buff, OS_PERF_ERROR_MAX_SIZE);
        printf("Get ipv4 route table failed for %s\n", buff);
        return;
    }

    printf("\nKernel IP routing table\n");
    printf("%-16s\t%-16s\t%-16s\t%-8s\t\%-8s\t\%-8s\t%-8s\t%-12s\n",
        "Destination", "Gateway", "Genmask", "Flags", "Metric", "Ref", "Use", "Iface");
    for (int i = 0; i < cnt; i++)
    {
        printf("%-16s\t%-16s\t%-16s\t%-8s\t\%-8d\t\%-8d\t%-8d\t%-12s\n",
            lst[i].target.ipv4, lst[i].gateway.ipv4, lst[i].mask, lst[i].flags, lst[i].metric,
            lst[i].refcnt, lst[i].use, lst[i].iface);
    }
    os_route_freep(&lst);
}

void ipv4_add_test()
{
}

void ipv4_del_test()
{
}

void get_ipv6_list_test()
{
    os_route_ipv6_info_t * lst = NULL;
    int cnt = os_route_get_ipv6_list(&lst);
    if (cnt < 0) {
        char buff[OS_PERF_ERROR_MAX_SIZE] = { 0 };
        os_perf_make_error_string(cnt, buff, OS_PERF_ERROR_MAX_SIZE);
        printf("Get ipv4 route table failed for %s\n", buff);
        return;
    }

    printf("\nKernel IPv6 routing table\n");
    printf("%-46s\t%-8s\t%-8s\t%-8s\t\%-8s\t\%-8s\n",
        "Destination", "Flag", "Met", "Ref", "Use", "If");
    for (int i = 0; i < cnt; i++)
    {
        printf("%-46s\t%-8s\t%-8d\t%-8d\t\%-8d\t\%-12s\n",
            lst[i].target.ipv6, lst[i].flags, lst[i].metric,
            lst[i].refcnt, lst[i].use, lst[i].iface);
    }
    os_route_freep(&lst);
}

void ipv6_add_test()
{
}

void ipv6_del_test()
{
}
