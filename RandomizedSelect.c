//
//  RandomizedSelect.c
//  libalgorithm
//
//  Created by sshe on 12-12-10.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include <stdio.h>

//#define ENABLE_RECURSION

int randomized_partition_int(int keys[], int p, int r);

#ifdef ENABLE_RECURSION
static int randomized_select(int *array, int p, int r, int i)
{
    int q, len;
    
    if (i < 0){
        printf("Cannot find the %dth smallest value\n", i);
        return i;
    }
    
    if (p == r)
        return array[p];
    
    q = randomized_partition_int(array, p, r);
    len = q - p;
    if (i == len)
        return array[q];
    else if (i < len)
        return randomized_select(array, p, q - 1, i);
    else
        return randomized_select(array, q + 1, r, i - len);
}
#else
static int randomized_select(int *array, int p, int r, int i)
{
    int q, len;
    
    if (p == r)
        return array[p];
    
    while (p < r){
        q = randomized_partition_int(array, p, r);
        len = q - p;
        if (i == len){
            return array[q];
        }else if (i < len){
            r = q - 1;
        }else{
            p = q + 1;
            i = i - len - 1;
        }
    }
    
    return array[p];
}
#endif

/*
 * Select the ith smallest value
 *   The index ranges from 0 to (r - p)
 * array: The array to be searched. This array may be
 *        modified after search.
 * p: Start index of the array.
 * r: The ending index of the array.
 * i: The ith value to be searched.
 */
int general_rselect(int *array, int p, int r, int i)
{
    if (i < 0){
        printf("The index should start from 0\n");
        return 0;
    }
    
    if (i > (r - p)){
        printf("The index is out of range\n");
        return 0;
    }
    
    return randomized_select(array, p, r, i);
}