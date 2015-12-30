//
//  AverageSort.c
//  libalgorithm
//
//  Created by sshe on 12-12-9.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

int heap_merge_k_sorted_array(int *k_array, int k, int len);
#define XCHG_KEYS(a, b) do { \
                            tmp = keys[(a)];  \
                            keys[(a)] = keys[(b)]; \
                            keys[(b)] = tmp;  \
                        }while(0)

static int k_partition_int(int keys[], int p, int r, int *t, int k)
{
    int i, j, tmp, len = r - p;
    int end_idx = p + len - (len % k);
    int x = keys[end_idx], equal_end = -1;
    
    for (i = j = p; j < end_idx; j += k) {
        if (x > keys[j]){
            if (equal_end >= 0){
                equal_end += k;
                tmp = keys[j];
                keys[j] = keys[equal_end];
                keys[equal_end] = keys[i];
                keys[i] = tmp;
            }else if (j != i){
                XCHG_KEYS(i, j);
            }
            
            i += k;
        }else if (x == keys[j]){
            if (equal_end < 0){
                if (j != i)
                    XCHG_KEYS(i, j);
                
                equal_end = i;
            }else{
                equal_end += k;
                if (equal_end < j)
                    XCHG_KEYS(equal_end, j);
            }
        }
    }
    
    if (equal_end < 0){
        keys[end_idx] = keys[i];
        keys[i] = x;
        *t = i + k;
    }else{
        equal_end += k;
        keys[end_idx] = keys[equal_end];
        keys[equal_end] = x;
        *t = equal_end + k;
    }
    
    
    return i - k;
}

static void k_quick_sort_int(int keys[], int p, int r, int k)
{
    int q, t;
    
    if (p < r - k){
        q = k_partition_int(keys, p, r, &t, k);
        k_quick_sort_int(keys, p, q, k);
        k_quick_sort_int(keys, t, r, k);
    }else if (p == r - k){
        if (keys[p] > keys[r]){
            int tmp = keys[p];
            keys[p] = keys[r];
            keys[r] = tmp;
        }
    }
}

void average_sort(int keys[], int p, int r, int k)
{
    int k_end = p + k;
    
    for (; p < k_end; p ++)
        k_quick_sort_int(keys, p, r, k);
}

void k_sort(int keys[], int p, int r, int k)
{
    int retval;
    
    average_sort(keys, p, r, k);
    retval = heap_merge_k_sorted_array(keys, k, r - p + 1);
    if (retval < 0){
        printf("heap merge failed\n");
    }
}