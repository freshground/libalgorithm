//
//  Heap.c
//  libalgorithm
//
//  Created by sshe on 12-10-11.
//  Copyright (c) 2012年 sshe. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>

#define PARENT(i)   ((i - 1) >> 1)
#define LEFT(i)     (((i + 1) << 1) - 1)
#define RIGHT(i)    ((i + 1) << 1)

#ifdef ENABLE_RECURSION
static void max_heapify(int *keys, int len, int root_idx)
{
    int l, r, largest = root_idx, tmp;
    
    l = LEFT(root_idx);
    r = RIGHT(root_idx);
    if (l < len && keys[l] > keys[largest])
        largest = l;
    
    if (r < len && keys[r] > keys[largest])
        largest = r;
    
    if (largest != root_idx){
        tmp = keys[root_idx];
        keys[root_idx] = keys[largest];
        keys[largest] = tmp;
        
        max_heapify(keys, len, largest);
    }
}
#else
static void max_heapify(int *keys, int len, int root_idx)
{
    int l, r, largest = root_idx, tmp;
    
    l = LEFT(root_idx);
    r = RIGHT(root_idx);
    while (l < len || r < len) {
        if (l < len && keys[l] > keys[largest])
            largest = l;
        
        if (r < len && keys[r] > keys[largest])
            largest = r;
        
        if (largest != root_idx){
            tmp = keys[root_idx];
            keys[root_idx] = keys[largest];
            keys[largest] = tmp;
            
            root_idx = largest;
        } else {
            break;
        }
        
        l = LEFT(root_idx);
        r = RIGHT(root_idx);
    }
}

void min_heapify(int *keys, int len, int root_idx)
{
    int l, r, smallest = root_idx, tmp;
    
    l = LEFT(root_idx);
    r = RIGHT(root_idx);
    while (l < len || r < len) {
        if (l < len && keys[l] < keys[smallest])
            smallest = l;
        
        if (r < len && keys[r] < keys[smallest])
            smallest = r;
        
        if (smallest != root_idx){
            tmp = keys[root_idx];
            keys[root_idx] = keys[smallest];
            keys[smallest] = tmp;
            
            root_idx = smallest;
        } else {
            break;
        }
        
        l = LEFT(root_idx);
        r = RIGHT(root_idx);
    }
}
#endif

static void buile_max_heap(int *keys, int len)
{
    int i;
    
    for (i = len / 2; i >= 0; i --) {
        max_heapify(keys, len, i);
    }
}

static void buile_min_heap(int *keys, int len)
{
    int i;
    
    for (i = len / 2; i >= 0; i --) {
        min_heapify(keys, len, i);
    }
}

void heap_sort(int *keys, int len)
{
    int i, tmp;
    
    buile_max_heap(keys, len);
    for (i = len; i > 0; i --) {
        tmp = keys[len - 1];
        keys[len - 1] = keys[0];
        keys[0] = tmp;
        len --;
        max_heapify(keys, len, 0);
    }
}

typedef struct heap_queue_s{
    int *keys;
    int len;
    int max;
}heap_queue_t;

unsigned long create_heap_queue(int len)
{
    heap_queue_t *q = malloc(sizeof(heap_queue_t));
    if (!q){
        perror("Cannot allocate heap queue struct");
        return 0;
    }
    
    q->keys = malloc(len * sizeof(int));
    if (!q->keys){
        perror("Cannot allocate heap queue");
        free(q);
        return 0;
    }
    
    q->len = 0;
    q->max = len;
    
    return (unsigned long)q;
}

void release_heap_queue(unsigned long queue_id)
{
    heap_queue_t *q = (heap_queue_t *)queue_id;
    
    if (q->keys)
        free(q->keys);
    
    if (q)
        free(q);
}

int heap_maximum(unsigned long queue_id)
{
    heap_queue_t *q = (heap_queue_t *)queue_id;
    
    return q->keys[0];
}

int heap_minimum(unsigned long queue_id)
{
    heap_queue_t *q = (heap_queue_t *)queue_id;
    
    return q->keys[0];
}

int heap_extract_max(unsigned long queue_id)
{
    heap_queue_t *q = (heap_queue_t *)queue_id;
    int max, *keys = q->keys, len = q->len;
    
    if (len < 0){
        printf("heap underflow\n");
        exit(-1);
    }
    
    max = keys[0];
    len --;
    keys[0] = keys[len];
    max_heapify(keys, len, 0);
    q->len = len;
    
    return max;
}

int heap_extract_min(unsigned long queue_id)
{
    heap_queue_t *q = (heap_queue_t *)queue_id;
    int max, *keys = q->keys, len = q->len;
    
    if (len < 0){
        printf("heap underflow\n");
        exit(-1);
    }
    
    max = keys[0];
    len --;
    keys[0] = keys[len];
    min_heapify(keys, len, 0);
    q->len = len;
    
    return max;
}

static int heap_increase_key(unsigned long queue_id, int key_idx, int new_value)
{
    heap_queue_t *q = (heap_queue_t *)queue_id;
    int *keys = q->keys;
    
    if (new_value < keys[key_idx]){
        printf("new key is smaller than current key\n");
        return -1;
    }
    
    while (key_idx > 0 && keys[PARENT(key_idx)] < new_value) {
        keys[key_idx] = keys[PARENT(key_idx)];
        key_idx = PARENT(key_idx);
    }
    
    keys[key_idx] = new_value;
    
    return 0;
}

int max_heap_insert(unsigned long queue_id, int value)
{
    heap_queue_t *q = (heap_queue_t *)queue_id;
    int *keys = q->keys, len = q->len;
    
    if (len >= q->max){
        printf("heap overflow\n");
        exit(-1); // We can expand the heap in the future.
    }
    
    keys[len] = value;
    q->len ++;
    
    return heap_increase_key(queue_id, len, value);
}

int max_heap_delete(unsigned long queue_id, int key_idx)
{
    heap_queue_t *q = (heap_queue_t *)queue_id;
    int *keys = q->keys;
    int deleted_value = keys[key_idx];
    
    q->len --;
    if (key_idx == q->len)
        return deleted_value;
    
    deleted_value = keys[key_idx];
    keys[key_idx] = keys[q->len];
    max_heapify(keys, q->len, key_idx);
    
    return deleted_value;
}

int heap_is_empty(unsigned long queue_id)
{
    heap_queue_t *q = (heap_queue_t *)queue_id;
    
    return (q->len == 0);
}

int heap_is_full(unsigned long queue_id)
{
    heap_queue_t *q = (heap_queue_t *)queue_id;
    
    return (q->len == q->max);
}
