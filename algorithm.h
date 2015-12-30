//
//  algorithm.h
//  libalgorithm
//
//  Created by sshe on 12-9-2.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#ifndef libalgorithm_algorithm_h
#define libalgorithm_algorithm_h

typedef struct list_int_key_s
{
    struct list_int_key_s *prev;
    struct list_int_key_s *next;
    int key;
}list_int_key_t;

void list_int_init(list_int_key_t *list);
int list_int_is_empty(list_int_key_t *list);
void list_int_add_before(list_int_key_t *head, list_int_key_t *list);
void list_int_add_after(list_int_key_t *head, list_int_key_t *list);
void list_int_del_one(list_int_key_t *list);
list_int_key_t *list_int_del_first(list_int_key_t *head);
int build_list(list_int_key_t *head, int *data, int len);
void free_list(list_int_key_t *head);
void insert_sort_int_list(list_int_key_t *head);
void list_int_del_one_partial(list_int_key_t *list);
#endif
