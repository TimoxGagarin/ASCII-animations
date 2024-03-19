#pragma once
#include <stdlib.h>
#include "vector2.h"
#include "list.h"

typedef struct node
{
    vector2_t data;
    struct node *next;
} node_t;

typedef struct list
{
    node_t *start;
    node_t *end;
} list_t;

list_t *new_list();
node_t *new_node(vector2_t data);
void push_end(list_t *list, vector2_t data);
node_t *pop_start(list_t *list);
void clear_list(list_t *list);