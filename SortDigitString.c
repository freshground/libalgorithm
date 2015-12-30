//
//  SortDigitString.c
//  libalgorithm
//
//  Created by sshe on 12-12-2.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

int string_radix_sort(char **in, char **out, int key_count, int string_len, int min, int max);

/*
 * Partition a string array by their strlen
 * in: The array need to be partitioned
 * out: To save the partitioned array
 * saved_c: To save the partition indice
 * count: Number of items of the input array
 * max: Maximium strlen of the strings
 */
int partition_array_by_length(char **in, char **out, int *saved_c, int count, int max)
{
    int counter_len = max + 1;
    int i, *c;
    unsigned long *string_lens;
    
    c = malloc(counter_len * sizeof(int));
    if (!c) {
        perror("Cannot allocate counter array");
        return -1;
    }
    
    string_lens = malloc(count * sizeof(unsigned long));
    if (!string_lens){
        perror("Cannot allocate string len array");
        free(c);
        return -1;
    }
    
    memset(c, 0, counter_len * sizeof(int));
    for (i = 0; i < count; i ++){
        string_lens[i] = strlen(in[i]);
        c[string_lens[i]] ++;
    }
    
    for (i = 1; i < counter_len; i ++) {
        c[i] += c[i - 1];
    }
    
    memcpy(saved_c, c, counter_len * sizeof(int));
    for (i = count - 1; i >= 0; i --) {
        out[c[string_lens[i]] - 1] = in[i];
        c[string_lens[i]] --;
    }
    
    free(c);
    free(string_lens);
    
    return 0;
}

/*
 * Sort digit strings. For example: "9" < "89" < "789"
 * in: The array of digit strings to be sorted
 * num_of_strings: Number of items of the input array
 * max_string_len: Maximium strlen of the strings
 */
int sort_digit_string(char **in, int num_of_strings, int max_string_len)
{
    char **output_ptr;
    int i, retval = 0, *saved_c, idx;
    
    output_ptr = malloc(num_of_strings * sizeof(char *));
    if (!output_ptr){
        perror("Cannot allocate output ptr");
        retval = -1;
        goto out;
    }
    
    saved_c = malloc(max_string_len + 1);
    if (!saved_c){
        perror("Cannot allocate saved c array");
        retval = -1;
        goto saved_c_failed;
    }
    
    retval = partition_array_by_length(in, output_ptr, saved_c, num_of_strings, max_string_len);
    if (retval < 0)
        goto partition_failed;
    
    for (i = 1; i <= max_string_len; i ++) {
        idx = string_radix_sort(&output_ptr[saved_c[i - 1]], &in[saved_c[i - 1]],
                                saved_c[i] - saved_c[i - 1], i, '0', '9');
        if (idx == 0)
            memcpy(&in[saved_c[i - 1]], &output_ptr[saved_c[i - 1]],
                   (saved_c[i] - saved_c[i - 1]) * sizeof(char *));
    }
    
    
partition_failed:
    free(saved_c);
saved_c_failed:
    free(output_ptr);
out:
    return retval;
}