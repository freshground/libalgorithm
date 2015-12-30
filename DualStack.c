//
//  DualStack.c
//  libalgorithm
//
//  Created by sshe on 13-3-15.
//  Copyright (c) 2013年 sshe. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct dual_stack_s{
    int *array;
    int upper_bottom, upper_top;
    int lower_bottom, lower_top;
}dual_stack_t;

/*
 * Creates a stack.
 *  len: The number of "int"s of the stack.
 *
 *  return: A stack handler for success, or 0 for failed.
 */
unsigned long dual_stack_init(int len)
{
    dual_stack_t *stack = NULL;
    
    stack = malloc(sizeof(dual_stack_t));
    if (!stack){
        perror("Cannot allocate stack structure.");
        return 0;
    }
    
    stack->array = malloc(sizeof(int) * len);
    if (!stack->array){
        free(stack);
        perror("Cannot allocate array.");
        return 0;
    }
    
    stack->upper_bottom = len - 1;
    stack->upper_top = len;
    stack->lower_bottom = 0;
    stack->lower_top = -1;
    
    return (unsigned long)stack;
}

/*
 * Destroys a stack.
 *  s: The stack to be destroyed.
 */
void dual_stack_deinit(unsigned long s)
{
    dual_stack_t *stack = (dual_stack_t *)s;
    
    if (!stack)
        return;
    
    if (stack->array)
        free(stack->array);
    
    free(stack);
}

static int dual_stack_upper_is_empty(dual_stack_t *stack)
{
    return (stack->upper_top > stack->upper_bottom);
}

static int dual_stack_upper_is_full(dual_stack_t *stack)
{
    return (stack->upper_top <= stack->lower_top + 1);
}

static int dual_stack_lower_is_empty(dual_stack_t *stack)
{
    return (stack->lower_top < 0);
}

static int dual_stack_lower_is_full(dual_stack_t *stack)
{
    return (stack->lower_top >= stack->upper_top - 1);
}

/*
 * Push
 * s: The stack.
 * key: The value to be pushed
 *
 * return: 0 for success, or -1 for failed.
 */
int dual_stack_push_upper(unsigned long s, int key)
{
    dual_stack_t *stack = (dual_stack_t *)s;
    
    if (!stack){
        printf("Stack does not exist.\n");
        return -1;
    }
    
    if (dual_stack_upper_is_full(stack)){
        printf("stack is full.\n");
        return -1;
    }
    
    stack->array[--(stack->upper_top)] = key;
    
    return 0;
}

/*
 * Pop
 * s: The stack.
 * key: Used to store the popped value.
 *
 * return: 0 for success, or -1 for failed.
 */
int dual_stack_pop_upper(unsigned long s, int *key)
{
    dual_stack_t *stack = (dual_stack_t *)s;
    
    if (!stack){
        printf("Stack does not exist.\n");
        return -1;
    }
    
    if (dual_stack_upper_is_empty(stack)){
        printf("stack is empty.\n");
        return -1;
    }
    
    *key = stack->array[(stack->upper_top) ++];
    
    return 0;
}

/*
 * Push
 * s: The stack.
 * key: The value to be pushed
 *
 * return: 0 for success, or -1 for failed.
 */
int dual_stack_push_lower(unsigned long s, int key)
{
    dual_stack_t *stack = (dual_stack_t *)s;
    
    if (!stack){
        printf("Stack does not exist.\n");
        return -1;
    }
    
    if (dual_stack_lower_is_full(stack)){
        printf("stack is full.\n");
        return -1;
    }
    
    stack->array[++(stack->lower_top)] = key;
    
    return 0;
}

/*
 * Pop
 * s: The stack.
 * key: Used to store the popped value.
 *
 * return: 0 for success, or -1 for failed.
 */
int dual_stack_pop_lower(unsigned long s, int *key)
{
    dual_stack_t *stack = (dual_stack_t *)s;
    
    if (!stack){
        printf("Stack does not exist.\n");
        return -1;
    }
    
    if (dual_stack_lower_is_empty(stack)){
        printf("stack is empty.\n");
        return -1;
    }
    
    *key = stack->array[(stack->lower_top) --];
    
    return 0;
}