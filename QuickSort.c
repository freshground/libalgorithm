//
//  QuickSort.c
//  libalgorithm
//
//  Created by sshe on 12-10-21.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include <stdio.h>

#define ENABLE_RECURSION

int partition_int(int keys[], int p, int r)
{
    int i, j, tmp, equal_cnt = 0;
    
    for (i = j = p; j < r; j ++) {
        if (keys[r] > keys[j]){
            if (j != i){
                tmp = keys[j];
                keys[j] = keys[i];
                keys[i] = tmp;
            }
            
            i ++;
        }else if (keys[r] == keys[j]){
            equal_cnt ++;
        }
    }
    
    if (equal_cnt == r - p)
        i = (r + p) >> 1;
    
    tmp = keys[i];
    keys[i] = keys[r];
    keys[r] = tmp;
    
    return i;
}

long int random_in_range_with_seed(int start, int end, unsigned int seed);
int randomized_partition_int(int keys[], int p, int r)
{
    int idx = (int)random_in_range_with_seed(p, r, r);
    
    if (idx < r && idx >= p){
        int tmp = keys[r];
        keys[r] = keys[idx];
        keys[idx] = tmp;
    }

    return partition_int(keys, p, r);
}

#ifdef ENABLE_RECURSION
void quick_sort_int(int keys[], int p, int r)
{
    int q, s = r - 1;
    
    if (p < s){
        q = partition_int(keys, p, r);
        quick_sort_int(keys, p, q - 1);
        quick_sort_int(keys, q + 1, r);
    }else if (p == s){
        if (keys[p] > keys[r]){
            int tmp = keys[p];
            keys[p] = keys[r];
            keys[r] = tmp;
        }
    }
}
#else
unsigned long create_int_stack(int default_size);
void release_int_stack(unsigned long stack);
int int_stack_pop(unsigned long stack);
void int_stack_push(unsigned long stack, int value);
int int_stack_is_empty(unsigned long stack);
#ifdef TRACE_STACK_SIZE
int int_stack_get_size(unsigned long stack);
#endif

void quick_sort_int(int keys[], int p, int r)
{
    unsigned long st = create_int_stack(0);
    int start, mid, end, tmp;
#ifdef TRACE_STACK_SIZE
    int max_stack_size = 0;
#endif
    
    int_stack_push(st, r);
    int_stack_push(st, p);
    while (!int_stack_is_empty(st)) {
        start = int_stack_pop(st);
        end = int_stack_pop(st);
        if (start < end - 1){
            mid = partition_int(keys, start, end);
            int_stack_push(st, end);
            int_stack_push(st, mid + 1);
            int_stack_push(st, mid - 1);
            int_stack_push(st, start);
#ifdef TRACE_STACK_SIZE
            if (int_stack_get_size(st) > max_stack_size)
                max_stack_size = int_stack_get_size(st);
#endif
        }else if (start == end - 1){
            if (keys[start] > keys[end]){
                tmp = keys[start];
                keys[start] = keys[end];
                keys[end] = tmp;
            }
        }
    }
    
    release_int_stack(st);
#ifdef TRACE_STACK_SIZE
    printf("Max stack size: %d\n", max_stack_size);
#endif
}
#endif