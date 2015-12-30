//
//  HeapMergeSortedQueue.c
//  libalgorithm
//
//  Created by sshe on 12-12-6.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

typedef struct sorted_queue_s{
    int *keys;
    int len;
    int curr_idx;
    int k;
}sorted_queue_t;
#define VALUE(q)    (q->keys[q->curr_idx])
#define IS_EMPTY(q) (q->curr_idx >= q->len)

static inline int POP(sorted_queue_t *q)
{
    int tmp = q->keys[q->curr_idx];
    q->curr_idx += q->k;
    return tmp;
}


typedef struct sq_heap_queue_s{
    sorted_queue_t **sq_keys;
    int len, max, k;
}sq_heap_queue_t;

#define PARENT(i)   ((i - 1) >> 1)
#define LEFT(i)     (((i + 1) << 1) - 1)
#define RIGHT(i)    ((i + 1) << 1)

/* sq stands for Sorted queue */
static void min_sq_heapify(sorted_queue_t **sq_keys, int len, int root_idx)
{
    int l, r, smallest = root_idx, v;
    sorted_queue_t *tmp;
    
    l = LEFT(root_idx);
    r = RIGHT(root_idx);
    
    tmp = sq_keys[l];
    if (l < len)
        v = VALUE(tmp);
    tmp = sq_keys[smallest];
    v = VALUE(tmp);
    while (l < len || r < len) {
        if (l < len && VALUE(sq_keys[l]) < VALUE(sq_keys[smallest]))
            smallest = l;
        
        if (r < len && VALUE(sq_keys[r]) < VALUE(sq_keys[smallest]))
            smallest = r;
        
        if (smallest != root_idx){
            tmp = sq_keys[root_idx];
            sq_keys[root_idx] = sq_keys[smallest];
            sq_keys[smallest] = tmp;
            
            root_idx = smallest;
        } else {
            break;
        }
        
        l = LEFT(root_idx);
        r = RIGHT(root_idx);
    }
}

static void buile_min_sq_heap(sorted_queue_t **sq_keys, int len)
{
    int i;
    
    for (i = len / 2; i >= 0; i --) {
        min_sq_heapify(sq_keys, len, i);
    }
}

static int sq_heap_minimum(sorted_queue_t *q)
{
    return VALUE(q);
}

static int sq_heap_extract_min(sq_heap_queue_t *sq_array)
{
    sorted_queue_t *q, *tmp;
    int min;
    
    if (sq_array->len == 0){
        printf("sorted queue underflow\n");
        exit(-1);
    }
    
    q = sq_array->sq_keys[0];
    if (IS_EMPTY(q)){
        printf("empty queue item\n");
        exit(-1);
    }
    
    min = POP(q);
    if (IS_EMPTY(q)){
        tmp = sq_array->sq_keys[0];
        sq_array->sq_keys[0] = sq_array->sq_keys[sq_array->len - 1];
        sq_array->sq_keys[sq_array->len - 1] = tmp;
        sq_array->len --;
    }
    
    if (sq_array->len)
        min_sq_heapify(sq_array->sq_keys, sq_array->len, 0);
    
    return min;
}

static sq_heap_queue_t *sq_array_create(int array_len, int k_len)
{
    sq_heap_queue_t *sq_array;
    
    sq_array = malloc(sizeof(sq_heap_queue_t));
    if (!sq_array){
        perror("Cannot allocate sq heap");
        return NULL;
    }
    
    memset(sq_array, 0, sizeof(sq_heap_queue_t));
    sq_array->sq_keys = malloc(sizeof(sorted_queue_t *) * array_len);
    if (!sq_array->sq_keys){
        perror("Cannot allocate sq array");
        free(sq_array);
        return NULL;
    }
    
    sq_array->len = 0;
    sq_array->max = array_len;
    sq_array->k = k_len;
    
    return sq_array;
}

static void sq_array_release(sq_heap_queue_t *sq_array)
{
    int i;
    
    for (i = 0; i < sq_array->len; i ++) {
        free(sq_array->sq_keys[i]);
    }
    
    free(sq_array);
}

static int sq_array_is_empty(sq_heap_queue_t *sq_array)
{
    return (sq_array->len == 0);
}

static int sq_array_is_full(sq_heap_queue_t *sq_array)
{
    return (sq_array->len == sq_array->max);
}

static int sq_array_add_queue(sq_heap_queue_t *sq_array, int *keys, int len)
{
    sorted_queue_t *q;
    
    if (sq_array_is_full(sq_array)){
        printf("sq array is full\n");
        return -1;
    }
    
    q = malloc(sizeof(sorted_queue_t));
    if (!q){
        perror("Cannot sq item");
        return -1;
    }
    
    memset(q, 0, sizeof(sorted_queue_t));
    q->keys = keys;
    q->curr_idx = 0;
    q->len = len;
    q->k = sq_array->k;
    
    sq_array->sq_keys[sq_array->len] = q;
    sq_array->len ++;
    
    return 0;
}

static void heap_merge_sorted_queue(sq_heap_queue_t * sq_array, int *result, int result_len)
{
    int i;
    
    buile_min_sq_heap(sq_array->sq_keys, sq_array->len);
    for (i = 0; i < result_len; i ++) {
        if (sq_array_is_empty(sq_array)){
            break;
        }
        
        result[i] = sq_heap_extract_min(sq_array);
    }
}

int heap_merge_k_sorted_array(int *k_array, int k, int len)
{
    sq_heap_queue_t *sq_array;
    int i, *result;
    int retval = -1;
    
    if (len <= k)
        return 0;
    
    result = malloc(len * (sizeof(int)));
    if (!result)
        goto out;
    
    sq_array = sq_array_create(k, k);
    if (!sq_array){
        printf("Cannot create sq array\n");
        goto sq_create_failed;
    }
    
    for (i = 0; i < k; i ++)
        if (sq_array_add_queue(sq_array, &k_array[i], len - i) < 0){
            printf("add queue item failed\n");
            goto add_queue_failed;
        }
    
    heap_merge_sorted_queue(sq_array, result, len);
    memcpy(k_array, result, len * sizeof(int));
    
    retval = 0;
    
add_queue_failed:
    sq_array_release(sq_array);
sq_create_failed:
    free(result);
out:
    return retval;
}