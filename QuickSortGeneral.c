//
//  QuickSortGeneral.c
//  libalgorithm
//
//  Created by sshe on 15/9/27.
//  Copyright (c) 2015年 sshe. All rights reserved.
//

#include <stdio.h>

typedef int (*sort_compare_t)(void *, void *);

int partition_general(void *keys[], int p, int r, sort_compare_t compare)
{
    int i, j, equal_cnt = 0;
    void *tmp;
    
    for (i = j = p; j < r; j ++) {
        if (compare(keys[r], keys[j]) > 0) {
            if (j != i){
                tmp = keys[j];
                keys[j] = keys[i];
                keys[i] = tmp;
            }
            
            i ++;
        } else if (compare(keys[r], keys[j]) == 0){
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

void quick_sort_general(void *keys[], int p, int r, sort_compare_t compare)
{
    int q, s = r - 1;
    
    if (p < s){
        q = partition_general(keys, p, r, compare);
        quick_sort_general(keys, p, q - 1, compare);
        quick_sort_general(keys, q + 1, r, compare);
    }else if (p == s){
        if (keys[p] > keys[r]){
            void *tmp = keys[p];
            keys[p] = keys[r];
            keys[r] = tmp;
        }
    }
}
