//
//  EqualDataQuickSortList.c
//  libalgorithm
//
//  Created by sshe on 12-11-18.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include <stdio.h>
#include "algorithm.h"

/* a <--> b */
#define _XCHG_LIST_KEYS_(a, b) \
            do { \
                list_int_key_t *tmp = a->prev;    \
                list_int_del_one_partial(a);    \
                list_int_add_after(b, a); \
                list_int_del_one_partial(b);  \
                list_int_add_after(tmp, b); \
                /* switch the ptrs back */  \
                tmp = a;    \
                a = b;  \
                b = tmp;    \
            }while(0)

/* a <--> b */
#define XCHG_LIST_KEYS(s, a, b) \
            do { \
                list_int_key_t *tmp = a->prev;    \
                if (a != b){                     \
                    list_int_del_one_partial(a);    \
                    list_int_add_after(b, a); \
                    list_int_del_one_partial(b);  \
                    list_int_add_after(tmp, b); \
                    /* switch the ptrs back */  \
                    tmp = a;    \
                    a = b;  \
                    b = tmp;    \
                    if (tmp == *s)  \
                    *s = a;     \
                }   \
            }while(0)

static list_int_key_t *equal_data_partition_int_list(list_int_key_t **start,
                                                     list_int_key_t **end,
                                                     list_int_key_t **t)
{
    list_int_key_t *i = *start, *j = *start, *equal_end = NULL;
    int x = (*end)->key;
    
    while (j != *end){
        if (x > j->key){
            if (equal_end){
                equal_end = equal_end->next;
                XCHG_LIST_KEYS(start, i, j);
                if (equal_end == i)
                    equal_end = j;
                else if (equal_end == j)
                    equal_end = i;
                
                XCHG_LIST_KEYS(start, equal_end, j);
            }else if (j != i){
                XCHG_LIST_KEYS(start, i, j);
            }
            
            i = i->next;
        }else if (x == j->key){
            if (!equal_end){
                if (j != i)
                    XCHG_LIST_KEYS(start, i, j);
                
                equal_end = i;
            }else{
                equal_end = equal_end->next;
                if (equal_end != j && equal_end->prev != j)
                    XCHG_LIST_KEYS(start, equal_end, j);
            }
        }
        
        j = j->next;
    }
    
    if (!equal_end){
        XCHG_LIST_KEYS(start, i, *end);
        
        if (i == *end)
            *t = *end;
        else
            *t = i->next;
    }else{
        equal_end = equal_end->next;
        XCHG_LIST_KEYS(start, equal_end, *end);
        if (equal_end == *end)
            *t = *end;
        else
            *t = equal_end->next;
    }
    
    if (i == *start)
        i = i->next;
    
    return i->prev;
}

void quick_sort_int_list(list_int_key_t *p, list_int_key_t *r)
{
    list_int_key_t *q, *t;
    
    if (p == r){
        return;
    }else if (p->next == r){
        if (p->key > r->key)
            _XCHG_LIST_KEYS_(p, r);
        
        return;
    }else{
        q = equal_data_partition_int_list(&p, &r, &t);
        quick_sort_int_list(p, q);
        quick_sort_int_list(t, r);
    }
}
