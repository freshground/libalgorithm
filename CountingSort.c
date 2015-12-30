//
//  CountingSort.c
//  libalgorithm
//
//  Created by sshe on 12-11-3.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

void counting_sort_int(int *keys, int *out, int len, int max)
{
    int *c, i;
    
    c = malloc((max + 1) * sizeof(int));
    if (!c){
        perror("Cannot allocate count array");
        return;
    }
    
    memset(c, 0, (max + 1) * sizeof(int));
    for (i = 0; i < len; i ++)
        c[keys[i]] ++;
    
    for (i = 1; i <= max; i ++)
        c[i] += c[i - 1];
    
    for (i = len - 1; i >= 0; i --) { // Counts down to keep to be stable
        out[c[keys[i]] - 1] = keys[i];
        c[keys[i]] --;
    }
    
    free(c);
}

/*
 * This is in-place version of counting sort.
 * It's not stable but it is in place.
 * Its performance is 30% worse than original counting sort.
 */
void counting_sort_int_in_place(int *keys, int len, int max)
{
    int *c, *idx, curr_idx_idx, i;
    int holder = -1, tmp;
    /* Allocate ending index array of partitions */
    c = malloc((max + 1) * sizeof(int));
    if (!c){
        perror("Cannot allocate count array");
        return;
    }
    /* Allocate beginning index array of partitions */
    idx = malloc((max + 1) * sizeof(int));
    if (!idx){
        perror("Cannot allocate idx array");
        free(c);
        return;
    }
    /* Counting each element */
    memset(c, 0, (max + 1) * sizeof(int));
    for (i = 0; i < len; i ++)
        c[keys[i]] ++;
    /* Calculating ending index of each partition */
    for (i = 1; i <= max; i ++)
        c[i] += c[i - 1];
    /* Calculating beginning index of each partition */
    for (i = 1; i <= max; i ++)
        idx[i] = c[i - 1];
    
    idx[0] = 0;
    curr_idx_idx = 0;
    while (curr_idx_idx <= max) {
        if (idx[curr_idx_idx] >= c[curr_idx_idx]){
            curr_idx_idx ++;
            continue;
        }
        
        holder = keys[idx[curr_idx_idx]];
        while (holder != curr_idx_idx) {
            tmp = keys[idx[holder]];
            keys[idx[holder] ++] = holder;
            holder = tmp;
        }
        
        keys[idx[curr_idx_idx] ++] = holder;
    }
    /* Clean up */
    free(c);
    free(idx);
}