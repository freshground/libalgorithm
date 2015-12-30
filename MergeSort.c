//
//  MergeSort.c
//  libalgorithm
//
//  Created by sshe on 12-9-2.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include <stdio.h>
#include <sys/time.h>
#include <strings.h>
#include <stdlib.h>

static void merge_int(int array[], int p, int q, int r)
{
    int n1 = q - p + 1;
    int i, j, k;
    int *tmp = malloc(n1 * sizeof(int));
    
    if (!tmp){
        perror("Failed to alloc temporary array");
        return;
    }
    
    memcpy(tmp, &array[p], n1 * sizeof(int));
    for (i = 0, k = p, j = q + 1; i < n1 && j <= r;){
        if (tmp[i] < array[j]){
            array[k] = tmp[i];
            i ++;
        }else{
            array[k] = array[j];
            j ++;
        }
        
        k ++;
    }
    
    if (i < n1){
        memcpy(&array[k], &tmp[i], (n1 - i) * sizeof(int));
    }
    
    free(tmp);
}

/*
 * Non-recursive merge sort
 * keys: the array to be sorted
 * p: the start index of the array
 * r: the end index of the array
 * interval: minimal sub-array to be merged
 */
void merge_sort_int(int keys[], int p, int r, int interval)
{
    int start, mid, end, n = r - p + 1;
    
    while (interval < n) {
        start = p;
        while (start < r) {
            mid = start + interval - 1;
            end = start + (interval << 1) - 1;
            if (end > r)
                end = r;
            
            if (end <= mid)
                break;
            
            merge_int(keys, start, mid, end);
            start = end + 1;
        }
        
        interval <<= 1;
    }
}
