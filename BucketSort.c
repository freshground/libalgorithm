//
//  BucketSort.c
//  libalgorithm
//
//  Created by sshe on 12-11-24.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "algorithm.h"

typedef struct bucket_list_s
{
    unsigned int len;
    unsigned int curr_idx;
    list_int_key_t *curr_ptr;
    list_int_key_t array[0];
}bucket_list_t;
#define BUCKET_LIST_PTR(b, i) (&(b->array[i]))

void quick_sort_int_list(list_int_key_t *p, list_int_key_t *r);

unsigned long bucket_list_create(int max_item_value)
{
    int i;
    bucket_list_t *bucket;
    
    bucket = malloc(sizeof(bucket_list_t) + max_item_value * sizeof(list_int_key_t));
    if (!bucket){
        perror("Cannot allocate bucket");
        return 0;
    }
    
    for (i = 0; i < max_item_value; i ++){
        list_int_init(BUCKET_LIST_PTR(bucket, i));
    }
    
    bucket->len = max_item_value;
    bucket->curr_idx = 0;
    bucket->curr_ptr = BUCKET_LIST_PTR(bucket, 0);
    
    return (unsigned long)bucket;
}

void bucket_list_insert(unsigned long handle, list_int_key_t *item)
{
    bucket_list_t *bucket = (bucket_list_t *)handle;
    
    if (item->key >= bucket->len)
        printf("insert: item value is bigger than length\n");
    else
        list_int_add_after(BUCKET_LIST_PTR(bucket, item->key), item);
}

int bucket_list_build(unsigned long handle, int *keys, int number)
{
    list_int_key_t *item;
    int i;
    
    for (i = 0; i < number; i ++) {
        item = malloc(sizeof(list_int_key_t));
        if (!item){
            perror("Cannot allocate item");
            return -1;
        }
        
        item->key = keys[i];
        bucket_list_insert(handle, item);
    }
    
    return 0;
}

void bucket_list_release(unsigned long handle)
{
    bucket_list_t *bucket = (bucket_list_t *)handle;
    int i, len = bucket->len;
    
    for (i = 0; i < len; i ++)
        free_list(BUCKET_LIST_PTR(bucket, i));
    
    free(bucket);
}

void bucket_list_sort(unsigned long handle)
{
    bucket_list_t *bucket = (bucket_list_t *)handle;
    int i, len = bucket->len;
    
    for (i = 0; i < len; i ++) {
        list_int_key_t *head = BUCKET_LIST_PTR(bucket, i);
        if (!list_int_is_empty(head))
            quick_sort_int_list(head->next, head->prev);
    }
}

void bucket_list_iter_start(unsigned long handle)
{
    bucket_list_t *bucket = (bucket_list_t *)handle;
    int i = 0, len = bucket->len;
    
    while (list_int_is_empty(BUCKET_LIST_PTR(bucket, i)) && i < len)
        i ++;
    
    bucket->curr_idx = i;
    bucket->curr_ptr = BUCKET_LIST_PTR(bucket, i);
    
}

int bucket_list_iter_next(unsigned long handle, int *value)
{
    bucket_list_t *bucket = (bucket_list_t *)handle;
    int i, len = bucket->len;
    int idx = bucket->curr_idx;
    list_int_key_t *curr = bucket->curr_ptr;
    
    i = idx;
    while (curr->next == BUCKET_LIST_PTR(bucket, i) && i < len) {
        i ++;
        curr = BUCKET_LIST_PTR(bucket, i);
    }
    
    if (i == len)
        return 0;
    
    curr = curr->next;
    *value = curr->key;
    bucket->curr_idx = i;
    bucket->curr_ptr = curr;
    
    return 1;
}