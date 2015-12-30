//
//  EqualDataQuickSort.c
//  libalgorithm
//
//  Created by sshe on 12-10-28.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include <stdio.h>

#define XCHG_KEYS(a, b) \
        do { \
            tmp = keys[(a)];  \
            keys[(a)] = keys[(b)]; \
            keys[(b)] = tmp;  \
        }while(0)

static int equal_data_partition_int(int keys[], int p, int r, int *t)
{
    int i, j, tmp;
    int x = keys[r], equal_end = -1;
    
    for (i = j = p; j < r; j ++) {
        if (x > keys[j]){
            if (equal_end >= 0){
                equal_end ++;
                tmp = keys[j];
                keys[j] = keys[equal_end];
                keys[equal_end] = keys[i];
                keys[i] = tmp;
            }else if (j != i){
                XCHG_KEYS(i, j);
            }
            
            i ++;
        }else if (x == keys[j]){
            if (equal_end < 0){
                if (j != i)
                    XCHG_KEYS(i, j);
                
                equal_end = i;
            }else{
                equal_end ++;
                if (equal_end < j)
                    XCHG_KEYS(equal_end, j);
            }
        }
    }
    
    if (equal_end < 0){
        keys[r] = keys[i];
        keys[i] = x;
        *t = i + 1;
    }else{
        equal_end ++;
        keys[r] = keys[equal_end];
        keys[equal_end] = x;
        *t = equal_end + 1;
    }
    
    
    return i - 1;
}

void equal_data_quick_sort_int(int keys[], int p, int r)
{
    int q, s = r - 1, t;
    
    if (p < s){
        q = equal_data_partition_int(keys, p, r, &t);
        equal_data_quick_sort_int(keys, p, q);
        equal_data_quick_sort_int(keys, t, r);
    }else if (p == s){
        if (keys[p] > keys[r]){
            int tmp = keys[p];
            keys[p] = keys[r];
            keys[r] = tmp;
        }
    }
}