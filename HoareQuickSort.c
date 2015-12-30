//
//  HoareQuickSort.c
//  libalgorithm
//
//  Created by sshe on 12-10-27.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include <stdio.h>

int hoare_partition_int(int keys[], int p, int r)
{
    int i, j, x, tmp;
    
    i = p - 1;
    j = r + 1;
    x = keys[p];
    while (1) {
        do{
            j --;
        }while (keys[j] > x);
        
        do {
            i ++;
        } while (keys[i] < x);
        
        if (i < j){
            tmp = keys[i];
            keys[i] = keys[j];
            keys[j] = tmp;
        }else{
            return j;
        }
    }
}

void hoare_quick_sort_int(int keys[], int p, int r)
{
    int q, s = r - 1;
    
    if (p < s){
        q = hoare_partition_int(keys, p, r);
        hoare_quick_sort_int(keys, p, q);
        hoare_quick_sort_int(keys, q + 1, r);
    }else if (p == s){
        if (keys[p] > keys[r]){
            int tmp = keys[p];
            keys[p] = keys[r];
            keys[r] = tmp;
        }
    }
}