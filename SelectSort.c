//
//  SelectSort.c
//  libalgorithm
//
//  Created by sshe on 12-9-2.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

/*
 * In each loop, selects a least value from the remaining part
 *    of the array, and put it to the end of the
 *    sorted part of the array.
 * keys: the array to be sorted
 * len: number of the items of the array
 */
void selection_sort_int(int keys[], int len)
{
    int i, j, current, temp;
    
    for (i = 0; i < len - 1; i ++){
        current = i;
        for (j = i + 1; j < len; j ++) {
            if (keys[j] < keys[current]){
                current = j;
            }
        }
        
        temp = keys[i];
        keys[i] = keys[current];
        keys[current] = temp;
    }
}
