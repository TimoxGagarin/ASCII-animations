#include <stdio.h>
#include "headers/list.h"

list_t *new_list()
{
    list_t *list = (list_t *)malloc(sizeof(list_t));
    if (list == NULL)
    {
        printf("Ошибка выделения памяти для списка.\n");
        exit(1);
    }
    list->start = NULL;
    list->end = NULL;
    return list;
}

node_t *new_node(vector2_t data)
{
    node_t *node = (node_t *)malloc(sizeof(node_t));
    if (node == NULL)
    {
        printf("Ошибка выделения памяти для нового узла.\n");
        exit(1);
    }
    node->data = data;
    node->next = NULL;
    return node;
}

void push_end(list_t *list, vector2_t data)
{
    node_t *node = new_node(data);
    if (list->start == NULL)
    {
        list->start = node;
        list->end = node;
        return;
    }
    list->end->next = node;
    list->end = node;
}

node_t *pop_start(list_t *list)
{
    node_t *res = list->start;
    list->start = list->start->next;
    if (list->start == NULL)
        list->end = NULL;
    return res;
}

void clear_list(list_t *list)
{
    while (list->end)
        free(pop_start(list));
    free(list);
}