//
//  Inversion-count.c
//  libalgorithm
//
//  Created by sshe on 12-9-8.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

/*
 * Let A[1 .. n] be an array of n distinct numbers. 
 * If i < j and A[i] > A[j], then the
 * pair (i, j) is called an inversion of A.
 */

#include <stdio.h>
#include <sys/time.h>
#include <strings.h>
#include <stdlib.h>

static int merge_int_count(int array[], int p, int q, int r)
{
    int n1 = q - p + 1;
    int i, j, k, count = 0;
    int *tmp = malloc(n1 * sizeof(int));
    
    if (!tmp){
        perror("Failed to alloc temporary array");
        return -1;
    }
    
    memcpy(tmp, &array[p], n1 * sizeof(int));
    for (i = 0, k = p, j = q + 1; i < n1 && j <= r;){
        if (tmp[i] <= array[j]){
            array[k] = tmp[i];
            i ++;
        }else{  // Find a inversion
            array[k] = array[j];
            j ++;
            count += n1 - i;
        }
        
        k ++;
    }
    
    if (i < n1){ // count the remaining inversions
        memcpy(&array[k], &tmp[i], (n1 - i) * sizeof(int));
    }
    
    free(tmp);
    return count;
}

/*
 * Count the inversion in the array.
 * keys: the array to be counted
 * p: the start index of the array
 * r: the end index of the array
 */
int inversion_count_int(int keys[], int p, int r)
{
    int start, mid, end, n = r - p + 1;
    int interval = 1, count = 0, tmp;
    
    while (interval < n) {
        start = p;
        while (start < r) {
            mid = start + interval - 1;
            end = start + (interval << 1) - 1;
            if (end > r)
                end = r;
            
            if (end <= mid)
                break;
            
            tmp = merge_int_count(keys, start, mid, end);
            if (tmp >= 0)
                count += tmp;
            else
                return -1;
            start = end + 1;
        }
        
        interval <<= 1;
    }
    
    return count;
}
