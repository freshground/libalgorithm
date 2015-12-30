//
//  ObjectWithoutPointer.c
//  libalgorithm
//
//  Created by sshe on 13-4-30.
//  Copyright (c) 2013年 sshe. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct object_array_item_s{
    unsigned long prev, next;
    char item_data[0];
}object_array_item_t;

typedef struct object_array_s{
    unsigned long list_in_use, free_list;
    long item_size, max_item_number, item_data_size, array_total_size;
    char item_array[0];
}object_array_t;

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:   the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) \
({ \
const typeof(((type *)0)->member) * __mptr = (ptr); \
(type *)((char *)__mptr - offsetof(type, member)); \
})

#define DEFAULT_ITEM_NUMBER 64
#define NULL_LIST ((unsigned long)(-1))
#define PREV(head, item) ((object_array_item_t *)(&(head->item_array[item->prev])))
#define NEXT(head, item) ((object_array_item_t *)(&(head->item_array[item->next])))
#define ITEM_BY_IDX(head, idx) ((object_array_item_t *)(&(head->item_array[head->item_size * (idx)])))
#define IDX_BY_ITEM(head, item) ((int)((unsigned long)item - (unsigned long)head->item_array) / head->item_size)
#define GET_OFFSET_BY_IDX(head, idx) ((idx) * (head->item_size))
#define GET_OFFSET_BY_ITEM(head, item) ((unsigned long)item - (unsigned long)head->item_array)
#define GET_ITEM_DATA_HEADER(data) container_of(data, object_array_item_t, item_data[0])

#define GET_ITEM_FROM_FREELIST(head) ((object_array_item_t *)(&(head->item_array[head->free_list])))
#define GET_ITEM_FROM_INUSE_LIST(head) ((object_array_item_t *)(&(head->item_array[head->list_in_use])))

/*
 * Creates a memory pool
 *  item_number: Numbers of the items the pool can contain.
 *  item_size: Size of each item.
 * Return:
 *  Nonzero: The pool is created successfully, and the handler is returned.
 *  0: Failed to allocate the pool.
 */
unsigned long object_array_init(int item_number, int item_size)
{
    object_array_t *head;
    object_array_item_t *item;
    int total_size, i;
    
    if (item_number < DEFAULT_ITEM_NUMBER)
        item_number = DEFAULT_ITEM_NUMBER;
    
    if (item_size <= 0){
        printf("Item size should be greater than zero\n");
        return 0;
    }
    
    item_size = (item_size + sizeof(object_array_item_t)); // Prepends header to item data.
    item_size = ((item_size + 7) >> 3) << 3; // Aligns each item to 8 bytes boundary.
    total_size = item_size * item_number + sizeof(object_array_t);
    
    head = malloc(total_size);
    if (!head){
        perror("Init failed");
        return 0;
    }
    
    head->item_size = item_size;
    head->max_item_number = item_number;
    head->item_data_size = item_size - sizeof(object_array_item_t);
    head->array_total_size = total_size;
    head->list_in_use = NULL_LIST;
    head->free_list = 0;
    for (i = 0; i < head->max_item_number; i ++) {
        item = ITEM_BY_IDX(head, i);
        if (i == 0)
            item->prev = NULL_LIST;
        else
            item->prev = GET_OFFSET_BY_IDX(head, i - 1);
        
        if (i == head->max_item_number - 1)
            item->next = NULL_LIST;
        else
            item->next = GET_OFFSET_BY_IDX(head, i + 1);
    }
    
    return (unsigned long)head;
}

/*
 * Destorys a memory pool
 *  head: The handler of the memory pool.
 */
void object_array_deinit(unsigned long head)
{
    if (head)
        free((object_array_t *)head);
}

/*
 * Extens the memory pool. (need to be completed)
 */
static int object_array_expand(object_array_t *head)
{
    return -1;
}

/*
 * Shrink the memory pool. (need to be completed)
 */
static int object_array_shrink(object_array_t *head)
{
    return -1;
}

/*
 * Adds an item to the front of the free list.
 *  head: The memory pool.
 *  item: The item to be added.
 */
static void add_to_free_list_head(object_array_t *head, object_array_item_t *item)
{
    object_array_item_t *next;
    
    item->next = head->free_list;
    head->free_list = GET_OFFSET_BY_ITEM(head, item);
    item->prev = NULL_LIST;
    
    if (item->next != NULL_LIST){
        next = NEXT(head, item);
        next->prev = head->free_list; // Points to item.
    }
}

/*
 * Remove an item from the free list.
 *  head: The memory pool.
 *  item: The item to be removed.
 */
static void del_from_free_list(object_array_t *head, object_array_item_t *item)
{
    object_array_item_t *tmp;
    
    if (item->next != NULL_LIST){
        tmp = NEXT(head, item);
        tmp->prev = item->prev;
    }
    
    if (item->prev != NULL_LIST){
        tmp = PREV(head, item);
        tmp->next = item->next;
    }else{
        head->free_list = item->next;
    }
}

/*
 * Adds an item to the front of the in-use list.
 *  head: The memory pool.
 *  item: The item to be added.
 */
static void add_to_list_in_use_head(object_array_t *head, object_array_item_t *item)
{
    object_array_item_t *next;
    
    item->next = head->list_in_use;
    head->list_in_use = GET_OFFSET_BY_ITEM(head, item);
    item->prev = NULL_LIST;
    
    if (item->next != NULL_LIST){
        next = NEXT(head, item);
        next->prev = head->list_in_use; // Points to item.
    }
}

/*
 * Remove an item from the in-use list.
 *  head: The memory pool.
 *  item: The item to be removed.
 */
static void del_from_list_in_use(object_array_t *head, object_array_item_t *item)
{
    object_array_item_t *tmp;
    
    if (item->next != NULL_LIST){
        tmp = NEXT(head, item);
        tmp->prev = item->prev;
    }
    
    if (item->prev != NULL_LIST){
        tmp = PREV(head, item);
        tmp->next = item->next;
    }else{
        head->list_in_use = item->next;
    }
}

/*
 * Allocates an item.
 *  h: The handler of the memory pool.
 * Return:
 *  Non-NULL: The item is allocated successfully, and the item is returned.
 *  NULL: Failed to allocate an item.
 */
char *object_array_allocate_item(unsigned long h)
{
    object_array_t *head = (object_array_t *)h;
    object_array_item_t *item;
    
    if (head->free_list == NULL_LIST && object_array_expand(head) == -1){
        printf("Out of space\n");
        return NULL;
    }
    
    // Takes the item off from free list.
    item = GET_ITEM_FROM_FREELIST(head);
    del_from_free_list(head, item);
    add_to_list_in_use_head(head, item);
    
    return item->item_data;
}

/*
 * Frees an item.
 *  h: The handler of the memory pool.
 *  data:
 * Return:
 *  Non-NULL: The item is allocated successfully, and the item is returned.
 *  NULL: Failed to allocate an item.
 */
void object_array_free_item(unsigned long h, char *data)
{
    object_array_t *head = (object_array_t *)h;
    object_array_item_t *item = GET_ITEM_DATA_HEADER(data);
    
    if (!head || !data){
        printf("Wrong object array header or data\n");
        return;
    }
    
    del_from_list_in_use(head, item);
    add_to_free_list_head(head, item);
}

/*
 * Prints all items in the free list.
 *  h: The handler of the memory pool.
 */
void object_array_dump_free_items(unsigned long h)
{
    object_array_t *head = (object_array_t *)h;
    object_array_item_t *item;
    
    if (head->free_list == NULL_LIST)
        return;
    
    item = GET_ITEM_FROM_FREELIST(head);
    printf("%4ld(%d)", IDX_BY_ITEM(head, item),item->item_data[0]);
    while (item->next != NULL_LIST) {
        item = NEXT(head, item);
        printf("%4ld(%d)", IDX_BY_ITEM(head, item), item->item_data[0]);
    }
    
    printf("\n");
}

/*
 * Prints all items in the in-use list.
 *  h: The handler of the memory pool.
 */
void object_array_dump_inuse_items(unsigned long h)
{
    object_array_t *head = (object_array_t *)h;
    object_array_item_t *item;
    
    if (head->list_in_use == NULL_LIST)
        return;
    
    item = GET_ITEM_FROM_INUSE_LIST(head);
    printf("%4ld(%d)", IDX_BY_ITEM(head, item), item->item_data[0]);
    while (item->next != NULL_LIST) {
        item = NEXT(head, item);
        printf("%4ld(%d)", IDX_BY_ITEM(head, item), item->item_data[0]);
    }
    
    printf("\n");
}

/*
 * Moves an item.
 *  head: The memory pool.
 *  dst: The destination to be moved. If dst is NULL, it chooses an item from the free list.
 *  src: The source position to be move.
 *  next_avaiable_free_item: Returns the next available item in free list.
 *  max_off: The max offset of items in in-use list should be less than max_off.
 * Return:
 *  Non-NULL: The item is moved successfully, and the dst is returned.
 *  NULL: Failed to move the item.
 */
static object_array_item_t *object_item_move(object_array_t *head, object_array_item_t *dst, object_array_item_t *src, object_array_item_t **next_avaiable_free_item, long max_off)
{
    if (!dst){
        if (head->free_list == NULL_LIST)
            dst = NULL;
        else
            dst = GET_ITEM_FROM_FREELIST(head);
    }
    
    while (dst && GET_OFFSET_BY_ITEM(head, dst) >= max_off) {
        if (dst->next == NULL_LIST)
            dst = NULL;
        else
            dst = NEXT(head, dst);
    }
    
    if (dst){
        if (dst->next == NULL_LIST)
            *next_avaiable_free_item = NULL;
        else
            *next_avaiable_free_item = NEXT(head, dst);
        
        del_from_free_list(head, dst);
        del_from_list_in_use(head, src);
        memcpy(dst->item_data, src->item_data, head->item_data_size);
        add_to_free_list_head(head, src);
        add_to_list_in_use_head(head, dst);
    }
    
    return dst;
}

/*
 * Splits the allocated_data array into 2 parts: the first half contains all
 *    allocated items; the second half contains all empty position.
 *  allocated_data: The array to be de-fragmented.
 *  max_count: The size of the array.
 * Return:
 *   Number of the allocated items in the array.
 */
static long allocated_data_defrag(char **allocated_data, long max_count)
{
    long i, j;
    
    for (i = 0, j = -1; i < max_count; i ++) {
        if (allocated_data[i]) {
            j ++;
            if (i != j){
                allocated_data[j] = allocated_data[i];
                allocated_data[i] = NULL;
            }
        }
    }
    
    return j + 1;
}

/*
 * Compatifies an memory pool.
 *  h: The handler of the memory pool.
 *  allocated_data: The array contains all pointers to the allocated item data.
 *  max_count: The size of the array.
 * Return:
 *  0: The memory pool is compactified successfully.
 *  Non-zero: Fail to compactify the memory pool.
 */
int object_array_compactify(unsigned long h, char **allocated_data, long max_count)
{
    object_array_t *head = (object_array_t *)h;
    object_array_item_t *dst = NULL, *src, *next_avaiable_free_item;
    unsigned long free_list_start_off;
    long i, allocated_item_count;
    
    allocated_item_count = allocated_data_defrag(allocated_data, max_count);
    free_list_start_off = GET_OFFSET_BY_IDX(head, allocated_item_count);
    for (i = 0; i < allocated_item_count; i ++) {
        src = GET_ITEM_DATA_HEADER(allocated_data[i]);
        if (GET_OFFSET_BY_ITEM(head, src) < free_list_start_off)
            continue;
        
        dst = object_item_move(head, dst, src, &next_avaiable_free_item, free_list_start_off);
        if (!dst){
            printf("Compactify failed: cannot move the item\n");
            break;
        }
        
        allocated_data[i] = dst->item_data;
        if (!next_avaiable_free_item)
            break;
        
        dst = next_avaiable_free_item;
    }
    
    if (i < allocated_item_count - 1) {
        printf("Compactify failed: too many items\n");
        return -1;
    }
    
    return 0;
}