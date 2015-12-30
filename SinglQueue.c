//
//  SinglQueue.c
//  libalgorithm
//
//  Created by sshe on 13-3-19.
//  Copyright (c) 2013年 sshe. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct single_queue_s{
    int *array;
    int head, tail;
    int len;
}single_queue_t;

/*
 * Creates a queue.
 *  len: The number of "int"s of the queue.
 *
 *  return: A queue handler for success, or 0 for failed.
 */
unsigned long queue_init(int len)
{
    single_queue_t *queue;
    
    queue = malloc(sizeof(single_queue_t));
    if (!queue){
        perror("Cannot allocate queue structure");
        return 0;
    }
    
    len ++; // One more position for full condition
    queue->array = malloc(sizeof(int) * len);
    if (!queue->array){
        perror("Cannot allocate queue array");
        free(queue);
        return 0;
    }
    
    queue->head = queue->tail = 0;
    queue->len = len;
    
    return ((unsigned long)queue);
}

/*
 * Destroys a queue.
 *  q: The queue to be destroyed.
 */
void queue_deinit(unsigned long q)
{
    single_queue_t *queue = (single_queue_t *)q;
    
    if (!queue)
        return;
    
    if (queue->array)
        free(queue->array);
    
    free(queue);
}

static int queue_is_empty(single_queue_t *queue)
{
    return (queue->head == queue->tail);
}

static int queue_is_full(single_queue_t *queue)
{
    int t = (queue->tail + 1) % queue->len;
    
    return (t == queue->head);
}

/*
 * Queue
 * q: The queue.
 * key: The value to be queued
 *
 * return: 0 for success, or -1 for failed.
 */
int enqueue(unsigned long q, int key)
{
    single_queue_t *queue = (single_queue_t *)q;
    
    if (queue_is_full(queue)){
        printf("Queue is full\n");
        return -1;
    }
    
    queue->array[queue->tail ++] = key;
    queue->tail %= queue->len;
    
    return 0;
}

/*
 * Dequeue
 * q: The queue.
 * key: Used to store the dequeued value.
 *
 * return: 0 for success, or -1 for failed.
 */
int dequeue(unsigned long q, int *key)
{
    single_queue_t *queue = (single_queue_t *)q;
    
    if (queue_is_empty(queue)){
        printf("Queue is empty\n");
        return -1;
    }
    
    *key = queue->array[queue->head ++];
    queue->head %= queue->len;
    
    return 0;
}