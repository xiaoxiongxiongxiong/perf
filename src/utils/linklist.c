#include "linklist.h"
#include "err.h"
#include <malloc.h>
#include <string.h>

struct _os_dlist_node_t {
    os_dlist_node_t * next;  // 下一个节点
    os_dlist_node_t * prev;  // 上一个节点
    char data[0];            // 数据
};

struct _os_dlist_t {
    size_t size;             // 链表长度
    size_t node_size;        // 节点数据大小
    os_dlist_node_t * head;  // 头节点
    os_dlist_node_t * tail;  // 尾节点
};

os_dlist_t * os_dlist_create(const size_t node_size)
{
    os_dlist_t * os_lst = (os_dlist_t *)malloc(sizeof(os_dlist_t));
    if (NULL != os_lst) {
        memset(os_lst, 0, sizeof(os_dlist_t));
        os_lst->node_size = node_size;
    }

    return os_lst;
}

void os_dlist_destroy(os_dlist_t ** os_lst)
{
    if (NULL == os_lst || NULL == *os_lst) {
        return;
    }

    if (!os_dlist_empty(*os_lst))
        os_dlist_clear(*os_lst);

    free(*os_lst);
    *os_lst = NULL;
}

void os_dlist_clear(os_dlist_t * os_lst)
{
    if (NULL == os_lst) {
        return;
    }

    os_dlist_node_t * node = NULL;
    os_dlist_node_t * head = os_lst->head;
    while (NULL != head) {
        node = head;
        head = head->next;
        free(node);
        --os_lst->size;
    }
}

bool os_dlist_empty(const os_dlist_t * os_lst)
{
    return os_lst ? (0 == os_lst->size) : true;
}

size_t os_dlist_size(const os_dlist_t * os_lst)
{
    return os_lst ? os_lst->size : 0;
}

bool os_dlist_add(os_dlist_t * os_lst, os_dlist_node_t * os_node, void * data)
{
    if (NULL == os_lst || NULL == data) {
        return false;
    }

    os_dlist_node_t * node = (os_dlist_node_t *)malloc(sizeof(os_dlist_node_t) + os_lst->node_size);
    if (NULL == node) {
        return false;
    }
    memcpy(node->data, data, os_lst->node_size);
    node->next = NULL;
    node->prev = NULL;

    if (0 == os_lst->size) {
        os_lst->head = node;
        os_lst->tail = node;
    } else {
        node->prev = os_lst->tail;
        os_lst->tail->next = node;
        os_lst->tail = node;
    }
    ++os_lst->size;

    return true;
}

bool os_dlist_delete(os_dlist_t * os_lst, os_dlist_node_t * os_node)
{
    if (NULL == os_lst || NULL == os_node) {
        return false;
    }

    if (os_node == os_lst->head) {
        os_lst->head = os_lst->head->next;
        if (NULL == os_lst->head)
            os_lst->tail = NULL;
        else
            os_lst->head->prev = NULL;
    } else if (os_node == os_lst->tail) {
        os_lst->tail = os_lst->tail->prev;
        if (NULL != os_lst->tail)
            os_lst->tail->next = NULL;
    } else {
        os_node->next->prev = os_node->prev;
        os_node->prev->next = os_node->next;
    }
    --os_lst->size;
    free(os_node);

    return true;
}

os_dlist_node_t * os_dlist_head(const os_dlist_t * os_lst)
{
    os_dlist_node_t * head = NULL;
    if (NULL != os_lst)
        head = os_lst->head;
    return head;
}

os_dlist_node_t * os_dlist_tail(const os_dlist_t * os_lst)
{
    os_dlist_node_t * tail = NULL;
    if (NULL != os_lst)
        tail = os_lst->tail;
    return tail;
}

os_dlist_node_t * os_dlist_next(const os_dlist_node_t * os_node)
{
    return os_node ? os_node->next : NULL;
}

os_dlist_node_t * os_dlist_prev(const os_dlist_node_t * os_node)
{
    return os_node ? os_node->prev : NULL;
}

void * os_dlist_getdata(const os_dlist_node_t * node)
{
    return node ? (void *)node->data : NULL;
}
