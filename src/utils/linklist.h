#ifndef _OS_LINK_LIST_H_
#define _OS_LINK_LIST_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct _os_dlist_node_t os_dlist_node_t;
typedef struct _os_dlist_t os_dlist_t;

/*
* os_dlist_create
* @brief  创建链表
* @param  node_size  节点大小
* @return 链表指针或者为NULL
*/
os_dlist_t * os_dlist_create(size_t node_size);

/*
* os_dlist_destroy
* @brief  销毁链表
* @param  os_lst  指向链表指针的指针
*/
void os_dlist_destroy(os_dlist_t ** os_lst);

/*
* os_dlist_clear
* @brief  清空链表
* @param  os_lst  链表指针
*/
void os_dlist_clear(os_dlist_t * os_lst);

/*
* os_dlist_empty
* @brief  判断链表是否为空
* @param  os_lst  链表指针
* @return true--成功 false--失败
*/
bool os_dlist_empty(const os_dlist_t * os_lst);

/*
* os_dlist_size
* @brief  获取链表长度
* @param  os_lst  链表指针
* @return 链表长度
*/
size_t os_dlist_size(const os_dlist_t * os_lst);

/*
* os_dlist_add
* @brief  向链表中添加数据
* @param  os_lst   链表指针
* @param  os_node  节点指针 可为空，不为空则添加到该节点之后
* @param  data     数据
* @return true--成功 false--失败
*/
bool os_dlist_add(os_dlist_t * os_lst, os_dlist_node_t * os_node, void * data);

/*
* os_dlist_delete
* @brief  从链表中删除数据
* @param  os_lst   链表指针
* @param  os_node  要删除的节点指针
* @return true--成功 false--失败
*/
bool os_dlist_delete(os_dlist_t * os_lst, os_dlist_node_t * os_node);

/*
* os_dlist_head
* @brief  获取链表头节点
* @param  os_lst   链表指针
* @return 头节点指针或者NULL
*/
os_dlist_node_t * os_dlist_head(const os_dlist_t * os_lst);

/*
* os_dlist_tail
* @brief  获取链表尾节点
* @param  os_lst   链表指针
* @return 尾节点指针或者NULL
*/
os_dlist_node_t * os_dlist_tail(const os_dlist_t * os_lst);

/*
* os_dlist_next
* @brief  获取节点的下一个节点
* @param  os_node   节点指针
* @return 节点下一个节点指针或者NULL
*/
os_dlist_node_t * os_dlist_next(const os_dlist_node_t * os_node);

/*
* os_dlist_prev
* @brief  获取节点的上一个节点
* @param  os_node   节点指针
* @return 节点上一个节点指针或者NULL
*/
os_dlist_node_t * os_dlist_prev(const os_dlist_node_t * os_node);

/*
* os_dlist_getdata
* @brief  获取节点的数据指针
* @param  os_node   节点指针
* @return 节点数据指针或者NULL
*/
void * os_dlist_getdata(const os_dlist_node_t * node);

#endif
