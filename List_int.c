//
//  List_int.c
//  libalgorithm
//
//  Created by sshe on 12-11-10.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include <stdio.h>
#include "algorithm.h"
#include <stdlib.h>

void list_int_init(list_int_key_t *list)
{
    list->prev = list;
    list->next = list;
}

int list_int_is_empty(list_int_key_t *list)
{
    if (list->prev == list && list->next == list)
        return 1;
    
    return 0;
}

void list_int_add_before(list_int_key_t *head, list_int_key_t *list)
{
    list->next = head;
    list->prev = head->prev;
    head->prev->next = list;
    head->prev = list;
}

void list_int_add_after(list_int_key_t *head, list_int_key_t *list)
{
    list->next = head->next;
    list->prev = head;
    head->next->prev = list;
    head->next = list;
}

void list_int_del_one(list_int_key_t *list)
{
    list->prev->next = list->next;
    list->next->prev = list->prev;
    list->prev = list;
    list->next = list;
}

void list_int_del_one_partial(list_int_key_t *list)
{
    list->prev->next = list->next;
    list->next->prev = list->prev;
}

list_int_key_t *list_int_del_first(list_int_key_t *head)
{
    list_int_key_t *list;
    
    if (list_int_is_empty(head))
        return NULL;
    
    list = head->next;
    list_int_del_one(list);
    
    return list;
}

int build_list(list_int_key_t *head, int *data, int len)
{
    int i;
    list_int_key_t *list;
    
    for (i = 0; i < len; i ++) {
        list = malloc(sizeof(list_int_key_t));
        if (!list){
            perror("Cannot allocate list");
            return -1;
        }
        
        list->key = data[i];
        list_int_add_before(head, list);
    }
    
    return 0;
}

void free_list(list_int_key_t *head)
{
    list_int_key_t *list;
    
    while ((list = list_int_del_first(head)))
        free(list);
}
