//
//  FinalMaxSubArray.c
//  libalgorithm
//
//  Created by sshe on 12-9-24.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

/*
 * Find the max sub-array of keys[]
 * keys: the array to be scaned
 * p: IN -- start index in keys[]
 *    OUT -- return start index of the sub-array
 * r: IN -- end index in keys[]
 *    OUT -- return end index of the sub-array
 * ret_sum: return the sum of the sub-array
 */
static void find_max_subarray(int keys[], int *p, int *r, int *ret_sum)
{
    int start_idx = *p, end_index = start_idx + 1, array_end = *r;
    int left_start_idx, left_end_idx, left_sum;
    int cross_start_idx, cross_sum, right_sum;
    int tmp_cross_sum = 0, tmp_cross_start_idx, dropped_sum = 0;
    
    if (end_index > array_end){
        *ret_sum = keys[start_idx];
        return;
    }
    
    left_start_idx = left_end_idx = start_idx;
    left_sum = keys[start_idx];
    tmp_cross_start_idx = end_index;
    
    while (end_index <= array_end) {
        right_sum = keys[end_index];
        tmp_cross_sum += right_sum;
        if (tmp_cross_start_idx < end_index){
            if (keys[tmp_cross_start_idx] <= 0){
                tmp_cross_sum -= keys[tmp_cross_start_idx];
                dropped_sum += keys[tmp_cross_start_idx];
                tmp_cross_start_idx ++;
            }
            
            if ((tmp_cross_sum - right_sum) < 0){
                dropped_sum += tmp_cross_sum - right_sum;
                tmp_cross_sum = right_sum;
                tmp_cross_start_idx = end_index;
            }
        }
        
        cross_sum = left_sum + dropped_sum + tmp_cross_sum;
        cross_start_idx = left_start_idx;
        
        if (cross_sum <= (dropped_sum + tmp_cross_sum)){
            cross_sum = dropped_sum + tmp_cross_sum;
            cross_start_idx = left_start_idx + 1;
        }
        
        if (cross_sum <= tmp_cross_sum){
            cross_sum = tmp_cross_sum;
            cross_start_idx = tmp_cross_start_idx;
        }
        
        if (right_sum >= left_sum && right_sum >= cross_sum){
            left_sum = right_sum;
            left_start_idx = left_end_idx = end_index;
            dropped_sum = tmp_cross_sum = 0;
            tmp_cross_start_idx = end_index + 1;
        } else if (cross_sum >= left_sum && cross_sum >= right_sum){
            left_sum = cross_sum;
            left_start_idx = cross_start_idx;
            left_end_idx = end_index;
            dropped_sum = tmp_cross_sum = 0;
            tmp_cross_start_idx = end_index + 1;
        }
        
        end_index ++;
    }
    
    *p = left_start_idx;
    *r = left_end_idx;
    *ret_sum = left_sum;
}

