//
//  SinglStack.c
//  libalgorithm
//
//  Created by sshe on 13-3-10.
//  Copyright (c) 2013年 sshe. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct single_stack_s{
    int *array;
    int len;
    int top;
}single_stack_t;

/*
 * Creates a stack.
 *  len: The number of "int"s of the stack.
 *
 *  return: A stack handler for success, or 0 for failed.
 */
unsigned long stack_init(int len)
{
    single_stack_t *stack = NULL;
    
    stack = malloc(sizeof(single_stack_t));
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
    
    stack->len = len;
    stack->top = -1;
    
    return (unsigned long)stack;
}

/*
 * Destroys a stack.
 *  s: The stack to be destroyed.
 */
void stack_deinit(unsigned long s)
{
    single_stack_t *stack = (single_stack_t *)s;
    
    if (!stack)
        return;
    
    if (stack->array)
        free(stack->array);
    
    free(stack);
}

static int stack_is_empty(single_stack_t *stack)
{
    return (stack->top < 0);
}

static int stack_is_full(single_stack_t *stack)
{
    return (stack->top >= stack->len);
}

/*
 * Push
 * s: The stack.
 * key: The value to be pushed
 *
 * return: 0 for success, or -1 for failed.
 */
int stack_push(unsigned long s, int key)
{
    single_stack_t *stack = (single_stack_t *)s;
    
    if (!stack){
        printf("Stack does not exist.\n");
        return -1;
    }
    
    if (stack_is_full(stack)){
        printf("stack is full.\n");
        return -1;
    }
    
    stack->array[++(stack->top)] = key;
    
    return 0;
}

/*
 * Pop
 * s: The stack.
 * key: Used to store the popped value.
 *
 * return: 0 for success, or -1 for failed.
 */
int stack_pop(unsigned long s, int *key)
{
    single_stack_t *stack = (single_stack_t *)s;
    
    if (!stack){
        printf("Stack does not exist.\n");
        return -1;
    }
    
    if (stack_is_empty(stack)){
        printf("stack is empty.\n");
        return -1;
    }
    
    *key = stack->array[(stack->top) --];
    
    return 0;
}