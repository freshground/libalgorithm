//
//  StackInt.c
//  libalgorithm
//
//  Created by sshe on 12-10-21.
//  Copyright (c) 2012年 sshe. All rights reserved.
//

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

typedef struct int_stack_s{
#define DEFAULT_SIZE 1024
    int *keys;
    int len;
    int max;
}int_stack_t;

unsigned long create_int_stack(int default_size)
{
    int_stack_t *st;
    
    if (default_size < DEFAULT_SIZE)
        default_size = DEFAULT_SIZE;
    
    st = malloc(sizeof(int_stack_t));
    if (st){
        st->keys = malloc(default_size * sizeof(int));
        if (!st->keys){
            free(st);
            return 0;
        }
        
        st->len = 0;
        st->max = default_size;
    }
    
    return (unsigned long)st;
}

void release_int_stack(unsigned long stack)
{
    int_stack_t *st = (int_stack_t *)stack;
    
    if (st->keys)
        free(st->keys);
    
    free(st);
}

int int_stack_pop(unsigned long stack)
{
    int_stack_t *st = (int_stack_t *)stack;
    int val, *new_keys;
    
    if (!st->len){
        printf("stack underflow\n");
        return 0;
    }
    
    val = st->keys[st->len - 1];
    st->len --;
    if (st->len > DEFAULT_SIZE && st->len < (st->max >> 2)){
        new_keys = realloc(st->keys, (st->max >> 1) * sizeof(int));
        if (new_keys){
            st->keys = new_keys;
            st->max >>= 1;
        }
    }
    
    return val;
}

void int_stack_push(unsigned long stack, int value)
{
    int_stack_t *st = (int_stack_t *)stack;
    int *new_keys;
    
    if (st->len == st->max){
        new_keys = realloc(st->keys, (st->max << 1) * sizeof(int));
        if (!new_keys){
            printf("stack overflow\n");
            return;
        }
        
        st->max <<= 1;
        st->keys = new_keys;
    }
    
    st->keys[st->len ++] = value;
}

int int_stack_is_empty(unsigned long stack)
{
    int_stack_t *st = (int_stack_t *)stack;
    
    return (st->len == 0);
}

#ifdef TRACE_STACK_SIZE
int int_stack_get_size(unsigned long stack)
{
    int_stack_t *st = (int_stack_t *)stack;
    
    return st->max;
}
#endif