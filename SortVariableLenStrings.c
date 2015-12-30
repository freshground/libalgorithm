//
//  SortVariableLenStrings.c
//  libalgorithm
//
//  Created by sshe on 12-12-5.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

/*
 * Counting sort a variable-length string array by the idx-th char.
 * "a" < "ab" < "b"
 * in: Input array of the variable-length strings
 * out: Output of the sorted array of the variable-length strings
 * strlen_array_in: The array of strlen() of every string.
 * strlen_array_out: Output of the sorted array of strlen() of every string.
 * c: Buffer of the counting array. Caller is responsible for clearing this buffer.
 * len: Number of items of the input array.
 * idx: The idx-th char of each string to be sorted by. The strlen() of the string may shorter than idx.
 * min: The minimium value of the chars.
 * max: The maximium value of the chars.
 */
static void variable_len_string_counting_sort(char **in, char **out,
                                              unsigned long *strlen_array_in,
                                              unsigned long *strlen_array_out,
                                              int *c, int len, long idx, int min, int max)
{
    int counter_len = max - min + 1;
    int i;
    
    for (i = 0; i < len; i ++){
        if (strlen_array_in[i] <= idx)
            c[-1] ++;
        else
            c[in[i][idx] - min] ++;
    }
    
    for (i = 0; i < counter_len; i ++)
        c[i] += c[i - 1];
    
    for (i = len - 1; i >= 0; i --) {
        if (strlen_array_in[i] <= idx){
            out[c[-1] - 1] = in[i];
            strlen_array_out[c[-1] - 1] = strlen_array_in[i];
            c[-1] --;
        }else{
            out[c[in[i][idx] - min] - 1] = in[i];
            strlen_array_out[c[in[i][idx] - min] - 1] = strlen_array_in[i];
            c[in[i][idx] - min] --;
        }
    }
}

/*
 * Radix sort of a array of variable-length strings
 * in: Input array of the variable-length strings
 * out: Output of the sorted array of the variable-length strings
 * keys_count: Number of items of the input array.
 * min: The minimium value of the chars.
 * max: The maximium value of the chars.
 */
int variable_len_string_radix_sort(char **in, char **out, int key_count, int min, int max)
{
    int *c, in_idx = -1, out_idx;
    int counter_len = max - min + 2; // Additional slot for '\0'
    char **buf_ptr[2] = {in, out};
    long i;
    unsigned long *strlen_array_in, *strlen_array_out, max_string_len = 0;
    unsigned long **strlen_array[2] = {&strlen_array_in, &strlen_array_out};
    
    c = malloc(counter_len * sizeof(int));
    if (!c){
        perror("Cannot allocate counter array");
        goto c_failed;
    }
    
    strlen_array_in = malloc(key_count * sizeof(unsigned long));
    if (!strlen_array_in){
        perror("Cannot allocate input string length array");
        goto strlen_array_in_failed;
    }
    
    strlen_array_out = malloc(key_count * sizeof(unsigned long));
    if (!strlen_array_out){
        perror("Cannot allocate output string length array");
        goto strlen_array_out_failed;
    }
    
    in_idx = 0;
    out_idx = 1;
    for (i = 0; i < key_count; i ++){
        strlen_array_in[i] = strlen(in[i]);
        if (strlen_array_in[i] > max_string_len)
            max_string_len = strlen_array_in[i];
    }
    
    for (i = (long)(max_string_len - 1); i >= 0; i --) {
        memset(c, 0, counter_len * sizeof(int));
        variable_len_string_counting_sort(buf_ptr[in_idx], buf_ptr[out_idx],
                                          *strlen_array[in_idx], *strlen_array[out_idx],
                                          &c[1], key_count, i, min, max);
        out_idx = in_idx;
        in_idx ^= 1;
    }
    
    free(strlen_array_out);
strlen_array_out_failed:
    free(strlen_array_in);
strlen_array_in_failed:
    free(c);
c_failed:
    return in_idx;
}

/*
 * Sort of a array of variable-length strings
 * in: Input array of the variable-length strings
 * num_of_strings: Number of items of the input array.
 * min: The minimium value of the chars.
 * max: The maximium value of the chars.
 */
int sort_variable_len_string(char **in, int num_of_strings, int min, int max)
{
    char **out;
    int idx;
    
    out = malloc(num_of_strings * sizeof(char *));
    if (!out){
        perror("Cannot allocate output ptr");
        return -1;
    }
    
    idx = variable_len_string_radix_sort(in, out, num_of_strings, min, max);
    if (idx == 1)
        memcpy(in, out, num_of_strings * sizeof(char *));
    
    free(out);
    return 0;
}