//
//  SinglyLinkedList.c
//  libalgorithm
//
//  Created by sshe on 13-3-27.
//  Copyright (c) 2013年 sshe. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct singly_linked_list_s{
    struct singly_linked_list_s *next;
    int key;
}singly_linked_list_t;

/*
 * Creates a singly linked list.
 *    Creates a list head.
 * Returns: 0 -- Failed;
 *         Non-zero --  Successed
 */
unsigned long singly_linked_list_init(void)
{
    singly_linked_list_t *head;
    
    head = malloc(sizeof(singly_linked_list_t));
    if (!head){
        perror("Cannot allocate head");
        return 0;
    }
    
    head->next = head;
    head->key = 0;
    
    return (unsigned long)head;
}

/*
 * Destroys a singly linked list.
 *    Destroys all members of the list and the list head.
 */
void singly_linked_list_deinit(unsigned long h)
{
    singly_linked_list_t *head = (singly_linked_list_t *)h;
    singly_linked_list_t *p;
    
    if (!head)
        return;
    
    p = head->next;
    while (p != head) {
        head->next = p->next;
        free(p);
        p = head->next;
    }
    
    free(head);
}

/*
 * Searches in a list.
 *  h: The head of the list.
 *  value: The value to be searched.
 * Returns: 0 -- Cannot find the member
 *          Non-zero -- The member
 */
unsigned long singly_linked_list_search(unsigned long h, int value)
{
    singly_linked_list_t *head = (singly_linked_list_t *)h;
    singly_linked_list_t *p;
    
    if (!head){
        printf("Empty list\n");
        return 0;
    }
    
    p = head->next;
    while (p != head && p->key != value) {
        p = p->next;
    }
    
    if (p == head)
        p = NULL;
    
    return (unsigned long)p;
}

/*
 * Deletes one member in the list.
 *  h: The head of the list.
 *  value: The value to be deleted.
 * Returns: 0 -- Cannot find the member
 *          Non-zero -- The member
 */
unsigned long singly_linked_list_delete(unsigned long h, int value)
{
    singly_linked_list_t *head = (singly_linked_list_t *)h;
    singly_linked_list_t *p;
    
    if (!head || head->next == head){
        printf("Empty list\n");
        return 0;
    }
    
    p = head;
    while (p->next != head && p->next->key != value) {
        p = p->next;
    }
    
    if (p->next == head){
        p = NULL;
    }else{
        singly_linked_list_t *prev = p;
        p = p->next;
        prev->next = p->next;
    }
    
    return (unsigned long)p;
}

/*
 * Inserts into a list.
 *  h: The head of the list.
 *  value: The value to be inserted.
 * Returns: 0 -- The value is inserted.
 *          -1 -- Cannot insert the member
 */
int singly_linked_list_insert(unsigned long h, int value)
{
    singly_linked_list_t *head = (singly_linked_list_t *)h;
    singly_linked_list_t *member;
    
    if (!head){
        printf("Cannot insert: no list.\n");
        return -1;
    }
    
    member = malloc(sizeof(singly_linked_list_t));
    if (!member){
        perror("Cannot insert");
        return -1;
    }
    
    member->key = value;
    member->next = head->next;
    head->next = member;
    
    return 0;
}

/*
 * Get the key value of a list member
 *  l: The list member.
 *  value: To store the key value.
 * Returns: 0 -- Gets value successfully.
 *          -1 -- Cannot get the value.
 */
int singly_linked_list_get_value(unsigned long l, int *value)
{
    singly_linked_list_t *list = (singly_linked_list_t *)l;
    
    if (list){
        *value = list->key;
        return 0;
    }else{
        printf("NULL list\n");
        return -1;
    }
}

/*
 * Prints all members of the list.
 *  h: The head of the list.
 */
void singly_linked_list_print(unsigned long h)
{
    singly_linked_list_t *head = (singly_linked_list_t *)h;
    singly_linked_list_t *member;
    int i;
    
    if (!head)
        return;
    
    i = 0;
    member = head->next;
    while (member != head) {
        printf("%4d", member->key);
        member = member->next;
        if (!((++ i) % 10))
            printf("\n");
    }
    
    printf("\n");
}

/*
 * Reverses a list.
 *  h: The head of the list.
 * Returns: 0 -- The list is reversed.
 *          -1 -- Cannot reverse the list.
 */
int singly_linked_list_reverse(unsigned long h)
{
    singly_linked_list_t *head = (singly_linked_list_t *)h;
    singly_linked_list_t *member, *remain;
    int link_end_to_head = 1;
    
    if (!head)
        return -1;
    
    remain = head->next;
    while (remain != head) {
        member = remain;
        remain = remain->next;
        if (link_end_to_head){
            member->next = head;
            link_end_to_head = 0;
        }else{
            member->next = head->next;
            head->next = member;
        }
    }
    
    return 0;
}