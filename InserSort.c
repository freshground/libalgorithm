//
//  InserSort.c
//  libalgorithm
//
//  Created by sshe on 12-9-2.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include "algorithm.h"

/*
 * Non-recursive insert sort
 * keys: array to be sorted
 * p: start index of the array
 * r: end index of the array
 */
void insert_sort_int(int keys[], int p, int r)
{
    int i, j, current;
    
    for (i = p + 1; i <= r; i ++){
        current = keys[i];
        j = i - 1;
        while (j >= p && keys[j] > current) {
            keys[j + 1] = keys[j];
            j --;
        }
        
        keys[j + 1] = current;
    }
}

void insert_sort_int_list(list_int_key_t *head)
{
    list_int_key_t *current = head->next, *backing_ptr, *next_ptr;
    int curr_val;
    
    while (current != head) {
        curr_val = current->key;
        backing_ptr = current->prev;
        next_ptr = current->next;
        while (backing_ptr != head && backing_ptr->key > curr_val)
            backing_ptr = backing_ptr->prev;
        
        if (backing_ptr->next != current){
            list_int_del_one(current);
            list_int_add_after(backing_ptr, current);
        }
        
        current = next_ptr;
    }
}