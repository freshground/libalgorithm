//
//  FindTheSecondSmallest.c
//  libalgorithm
//
//  Created by sshe on 12-12-10.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include <stdio.h>

int find_second_smallest_int(int *array, int p, int r, int *value)
{
    int first, second, len = r - p + 1;
    int i;
    
    if (len < 2){
        printf("The array has too less items\n");
        return -1;
    }else if (len == 2){
        if (array[r] > array[p])
            *value = array[r];
        else
            *value = array[p];
        
        return 0;
    }
    
    if (array[p] < array[p + 1]){
        first = array[p];
        second = array[p + 1];
    }else{
        first = array[p + 1];
        second = array[p];
    }
    
    for (i = p + 2; i <= r; i ++) {
        if (array[i] < second) {
            if (array[i] > first){
                second = array[i];
            }else if (array[i] < first){
                second = first;
                first = array[i];
            }
        }
    }
    
    *value = second;
    return 0;
}

int find_max_min_int(int *array, int p, int r, int *max, int *min)
{
    int first, second, len = r - p + 1;
    int i;
    
    if (len < 2){
        printf("The array has too less items\n");
        return -1;
    }else if (len == 2){
        if (array[r] > array[p]){
            *max = array[r];
            *min = array[p];
        }else{
            *max = array[p];
            *min = array[r];
        }
        
        return 0;
    }
    
    if (array[p] < array[p + 1]){
        first = array[p];
        second = array[p + 1];
    }else{
        first = array[p + 1];
        second = array[p];
    }
    
    for (i = p + 2; i <= r; i ++) {
        if (array[i] > second)
            second = array[i];
        else if (array[i] < first)
            first = array[i];
    }
    
    *min = first;
    *max = second;
    
    return 0;
}