//
//  DoublyLinkedList.c
//  libalgorithm
//
//  Created by sshe on 13-4-5.
//  Copyright (c) 2013年 sshe. All rights reserved.
//
//  The reverse operation is O(1).

#include <stdio.h>
#include <stdlib.h>

typedef struct list_header_s{
    struct list_header_s *next;
}list_header_t;

typedef struct doubly_linked_list_head_s{
    list_header_t np;
    list_header_t prev;
}doubly_linked_list_head_t;

typedef struct doubly_linked_list_member_s{
    list_header_t np;
    int key;
}doubly_linked_list_member_t;

/*
 * Calcuates and sets the next pointer.
 *   list: The header or member, whose next pointer needs to be calcuated and be set to.
 *   next_list: The header or member succeeds list.
 *   prev_list: The header or member procedes list.
 */
#define SET_NEXT(list, next_list, prev_list) \
do{list->np.next = (list_header_t*)((unsigned long)(next_list) ^ (unsigned long)(prev_list));}while(0)

/*
 * Calcuates and gets the next pointer.
 *   list_type: The type of the list to be returned.
 *   list: The header or member, whose next pointer needs to be calcuated and be got from.
 *   prev_list: The header or member procedes list.
 * Returns the next doubly_linked_list_member_t *.
 */
#define GET_NEXT(list_type, list, prev_list) \
((typeof(list_type))((unsigned long)(list->np.next) ^ (unsigned long)(prev_list)))

/*
 * Recalcuate list->next.
 *   list: list->next is being recalcuated.
 *   orig_prev: The previous item of list before recalcuating.
 *   new_prev: The previous item of list after recalcuating.
 */
#define RECALCUATE_NEXT(list, orig_prev, new_prev) do \
{   \
list->np.next = (list_header_t *)((unsigned long)list->np.next ^ (unsigned long)orig_prev); \
list->np.next = (list_header_t *)((unsigned long)list->np.next ^ (unsigned long)new_prev); \
}while(0)

/*
 * Gets the previous pointer.
 *   list_type: The type of the list to be returned.
 *   head: The header whose previous pointer needs to be got from.
 */
#define GET_PREV(list_type, head) ((typeof(list_type))(head->prev.next))

/*
 * Sets the previous pointer.
 *   head: The header whose previous pointer needs to be set to.
 *   prev_list: The header or member procedes list.
 */
#define SET_PREV(head, prev_list) do{head->prev.next = (list_header_t *)prev_list;}while(0)

/*
 * Decides if the two lists equal.
 */
#define IS_EQUAL(list1, list2) ((list_header_t *)list1 == (list_header_t *)list2)

/*
 * Creates a doubly linked list.
 * Return:
 *  Non-zero -- The newly created list head.
 *  0 -- Failed to create the list.
 */
unsigned long doubly_linked_list_init(void)
{
    doubly_linked_list_head_t *head;
    
    head = malloc(sizeof(doubly_linked_list_head_t));
    if (!head){
        perror("Cannot allocate head");
        return 0;
    }
    
    SET_PREV(head, head);
    SET_NEXT(head, head, head);
    
    return (unsigned long)head;
}

/*
 * Destroys a doubly linked list.
 *   h: The list to be destroyed.
 */
void doubly_linked_list_deinit(unsigned long h)
{
    doubly_linked_list_head_t *head = (doubly_linked_list_head_t *)h;
    doubly_linked_list_member_t *next, *prev, *nn, *pp;
    
    if (!head)
        return;
    
    prev = (doubly_linked_list_member_t *)head;
    pp = GET_PREV(pp, head);
    next = GET_NEXT(next, head, GET_PREV(next, head));
    while (!IS_EQUAL(next, head)) {
        // Removes *next* from the list.
        nn = GET_NEXT(nn, prev, next);
        SET_NEXT(prev, nn, pp);
        RECALCUATE_NEXT(nn, next, prev);
        // Frees *next*, and sets it to nn.
        free(next);
        next = nn;
    }
    
    free(head);
}

/*
 * Inserts an item into the doubly linked list.
 *   h: The list to be inserted into.
 *   value: The value to be added.
 * Return:
 *  Non-zero -- Fails to insert the new item.
 *  0 -- The new item is inserted successfully.
 */
int doubly_linked_list_insert(unsigned long h, int value)
{
    doubly_linked_list_head_t *head = (doubly_linked_list_head_t *)h;
    doubly_linked_list_member_t *next, *prev, *new_item;
    
    if (!head){
        printf("head does not exist.\n");
        return -1;
    }
    
    new_item = malloc(sizeof(doubly_linked_list_member_t));
    if (!new_item){
        perror("Cannot allocate new item.");
        return -1;
    }
    
    new_item->key = value;
    prev = GET_PREV(prev, head);
    next = GET_NEXT(next, head, prev);
    // Inserts the new item between head and head->next.
    SET_NEXT(head, new_item, prev);
    SET_NEXT(new_item, next, head);
    RECALCUATE_NEXT(next, head, new_item);
    // Links head->prev to the last item of the list.
    if (IS_EQUAL(head, GET_PREV(head, head))){
        // RECALCUATE_NEXT(head, head, new_item);
        SET_PREV(head, new_item);
    }
    
    return 0;
}

/*
 * Deletes an item into the doubly linked list.
 *   h: The list to be deleted from.
 *   value: The value to be deleted.
 * Return:
 *  Non-zero -- Fails to delete the value.
 *  0 -- The value is deleted successfully.
 */
unsigned long doubly_linked_list_delete(unsigned long h, int value)
{
    doubly_linked_list_head_t *head = (doubly_linked_list_head_t *)h;
    doubly_linked_list_member_t *next, *prev, *pp, *nn;
    
    if (!head){
        printf("head does not exist.\n");
        return -1;
    }
    
    pp = GET_PREV(prev, head);
    next = GET_NEXT(next, head, pp);
    prev = (doubly_linked_list_member_t *)head;
    while (!IS_EQUAL(next, head) && next->key != value) {
        nn = GET_NEXT(nn, next, prev);
        pp = prev;
        prev = next;
        next = nn;
    }
    
    if (IS_EQUAL(next, head)){
        next = NULL;
    }else{
        nn = GET_NEXT(nn, next, prev);
        SET_NEXT(prev, nn, pp);
        RECALCUATE_NEXT(nn, next, prev);
        if (IS_EQUAL(nn, head)){
            SET_PREV(head, prev);
        }
    }
    
    return (unsigned long)next;
}

/*
 * Gets the value of the item.
 *   l: The item.
 *   value: To store the value of the item.
 * Return:
 *  Non-zero -- Fails to get the value.
 *  0 -- Gets the value successfully.
 */
int doubly_linked_list_get_value(unsigned long l, int *value)
{
    doubly_linked_list_member_t *item = (doubly_linked_list_member_t *)l;
    
    if (item){
        *value = item->key;
        return 0;
    }else{
        printf("NULL item\n");
        return -1;
    }
}

/*
 * Searches an item in the doubly linked list.
 *   h: The list to be searched in.
 *   value: The value to be searched.
 * Return:
 *  Non-zero -- The value is found.
 *  0 -- The value is not found.
 */
unsigned long doubly_linked_list_search(unsigned long h, int value)
{
    doubly_linked_list_head_t *head = (doubly_linked_list_head_t *)h;
    doubly_linked_list_member_t *next, *prev, *nn;
    
    if (!head){
        printf("head does not exist.\n");
        return -1;
    }
    
    prev = GET_PREV(prev, head);
    next = GET_NEXT(next, head, prev);
    prev = (doubly_linked_list_member_t *)head;
    while (!IS_EQUAL(next, head) && next->key != value) {
        nn = GET_NEXT(nn, next, prev);
        prev = next;
        next = nn;
    }
    
    if (IS_EQUAL(next, head)){
        next = NULL;
    }
    
    return (unsigned long)next;
}

/*
 * Reverses an doubly linked list.
 *   h: The list to be reversed.
 * Return:
 *  Non-zero -- Fails to reverse the list.
 *  0 -- The list is reversed successfully.
 */
int doubly_linked_list_reverse(unsigned long h)
{
    doubly_linked_list_head_t *head = (doubly_linked_list_head_t *)h;
    doubly_linked_list_member_t *next, *prev;
    
    if (!head){
        printf("head does not exist.\n");
        return -1;
    }
    
    prev = GET_PREV(prev, head);
    next = GET_NEXT(next, head, prev);
    SET_PREV(head, next);
    SET_NEXT(head, prev, next);
    
    return 0;
}

/*
 * Prints all values in the list.
 *   h: The list to be printed.
 */
void doubly_linked_list_print(unsigned long h)
{
    doubly_linked_list_head_t *head = (doubly_linked_list_head_t *)h;
    doubly_linked_list_member_t *next, *prev, *nn;
    int i;
    
    if (!head){
        printf("head does not exist.\n");
        return;
    }
    
    prev = GET_PREV(prev, head);
    next = GET_NEXT(next, head, prev);
    prev = (doubly_linked_list_member_t *)head;
    i = 0;
    while (!IS_EQUAL(next, head)){
        printf("%4d", next->key);
        if (!((++ i) % 10))
            printf("\n");
        
        nn = GET_NEXT(nn, next, prev);
        prev = next;
        next = nn;
    }
}