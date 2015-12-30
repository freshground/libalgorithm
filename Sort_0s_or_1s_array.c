//
//  Sort_0s_or_1s_array.c
//  libalgorithm
//
//  Created by sshe on 12-11-27.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

/*
 * Sort an array with only 0s and 1s.
 * This sort is not stable, but it is faster than quick sort.
 * It is more than 20% faster than equal data quick sort.
 */
void sort_0s_or_1s(int *keys, int p, int r)
{
    int idx0, idx1, tmp;
    
    idx0 = p;
    idx1 = r;
    while (idx0 < idx1) {
        if (keys[idx0] == 0){
            idx0 ++;
            continue;
        }
        
        if (keys[idx1] == 1){
            idx1 --;
            continue;
        }
        
        tmp = keys[idx0];
        keys[idx0] = keys[idx1];
        keys[idx1] = tmp;
    }
}