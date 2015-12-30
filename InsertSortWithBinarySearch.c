//
//  InsertSortWithBinarySearch.c
//  libalgorithm
//
//  Created by sshe on 12-9-2.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include <stdio.h>
#include <sys/time.h>
#include <strings.h>
#include <stdlib.h>

/*
 * Binary search the array to get the rightmost number
 *    which is not greater than key_value.
 * If there is no number is less than or equal to key_value,
 *    then this function returns -1.
 * keys[]: the array to be searched
 * key_value: the value to be compared
 * p: start index in keys[]
 * r: end index in keys[]
 */
static inline int binary_search_int(int keys[], int key_value, int p, int r)
{
    int start_idx, mid_idx, end_idx;
    
    start_idx = p;
    end_idx = r;
    
    /*
     * If there are less than 3 items to be searched,
     *    we just compare p and r directly.
     */
    while (start_idx < end_idx - 1) {
        mid_idx = (start_idx + end_idx) / 2;
        if (keys[mid_idx] == key_value) // find the equal value, just return it
            return mid_idx;
        
        if (keys[mid_idx] > key_value)
            end_idx = mid_idx; // seach the lower half
        else
            start_idx = mid_idx; // seach the upper half
    }
    
    if (keys[end_idx] <= key_value) // compare the rightmost one firstly
        return end_idx;
    else if (keys[start_idx] <= key_value)
        return start_idx;
    else return -1;
}

/*
 * sort the array to increasingly
 * keys[]: the array to be sorted.
 * len: length of keys[]
 */
void binary_insert_sort_int(int keys[], int len)
{
    int i, current, key_idx;
    
    /*
     * To make use of memmove(), we should sort the array from right to left
     */
    for (i = len - 2; i >= 0; i --){
        current = keys[i]; // current value to be compared
        /*
         * To find the rightmost item which is not greater than current value,
         *    and move the part of array (i + 1 to key_idx) to left by
         *    one item.
         * Put the current value behind key_idx.
         */
        key_idx = binary_search_int(keys, current, i + 1, len - 1);
        /*
         * If there is no value is less than or equal to current value,
         *    then skip memmove().
         */
        if (key_idx >= 0){
            memmove(&keys[i], &keys[i + 1], (key_idx - i) * sizeof(int));
            keys[key_idx] = current;
        }
    }
}
