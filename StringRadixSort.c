//
//  StringRadixSort.c
//  libalgorithm
//
//  Created by sshe on 12-11-4.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include <stdio.h>
#include <sys/time.h>
#include <strings.h>
#include <stdlib.h>

static void string_counting_sort(char **in, char **out, int *c, int len, int idx, int min, int max)
{
    int counter_len = max - min + 1;
    int i;
    
    for (i = 0; i < len; i ++)
        c[in[i][idx] - min] ++;
    
    for (i = 1; i < counter_len; i ++)
        c[i] += c[i - 1];
    
    for (i = len - 1; i >= 0; i --) {
        out[c[in[i][idx] - min] - 1] = in[i];
        c[in[i][idx] - min] --;
    }
}

int string_radix_sort(char **in, char **out, int key_count, int string_len, int min, int max)
{
    int *c, i, in_idx = 0, out_idx = 1;
    int counter_len = max - min + 1;
    char **buf_ptr[2] = {in, out};
    
    c = malloc(counter_len * sizeof(int));
    if (!c){
        perror("Cannot allocate counter array");
        return -1;
    }
    
    for (i = string_len - 1; i >= 0; i --) {
        memset(c, 0, counter_len * sizeof(int));
        string_counting_sort(buf_ptr[in_idx], buf_ptr[out_idx],
                             c, key_count, i, min, max);
        out_idx = in_idx;
        in_idx ^= 1;
    }
    
    free(c);
    
    return in_idx;
}