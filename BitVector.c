//
//  BitVector.c
//  libalgorithm
//
//  Created by sshe on 13-5-8.
//  Copyright (c) 2013年 sshe. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BITS_OF_INT (sizeof(int) * 8)

typedef struct bit_vector_s{
    int size;
    int *vector;
}bit_vector_t;

/*
 * Creates a bit vector.
 *  size: Max number of bits in the vector.
 * Return:
 *  Non-zero: returns the handler of the vector.
 *  0: failed to create a bit vector.
 */
unsigned long bit_vector_init(int size)
{
    bit_vector_t *vec;
    
    vec = malloc(sizeof(bit_vector_t));
    if (!vec){
        perror("Cannot allocate vector");
        return 0;
    }
    
    vec->size = size;
    size = (size + BITS_OF_INT - 1) / BITS_OF_INT;
    vec->vector = malloc(size);
    if (!vec->vector){
        perror("Cannot allocate bitmap");
        free(vec);
        return 0;
    }
    
    memset(vec->vector, 0, size);
    
    return (unsigned long)vec;
}

/*
 * Destroys a bit vector.
 *  v: The vector to be destroyed.
 */
void bit_vector_deinit(unsigned long v)
{
    bit_vector_t *vec = (bit_vector_t *)v;
    
    if (!vec)
        return;
    
    if (vec->vector)
        free(vec->vector);
    
    free(vec);
}

/*
 * Inserts a bit in the vector.
 *  v: The vector to be inserted into.
 *  idx: The bit to be inserted.
 * Return:
 *  0: Inserted successfully.
 *  -1: Failed to insert.
 */
int bit_vector_insert(unsigned long v, int idx)
{
    bit_vector_t *vec = (bit_vector_t *)v;
    int byte_idx, bit_off;
    int *map;
    
    if (idx >= vec->size){
        printf("%s: Out of range\n", __FUNCTION__);
        return -1;
    }
    
    byte_idx = idx / BITS_OF_INT;
    bit_off = idx % BITS_OF_INT;
    map = &(vec->vector[byte_idx]);
    if (*map & (1 << bit_off)){
        printf("Bit %d already exists\n", idx);
        return -1;
    }
    
    *map |= 1 << bit_off;
    
    return 0;
}

/*
 * Searches a bit in the vector.
 *  v: The vector to be searched from.
 *  idx: The bit to be searched.
 * Return:
 *  1: Find it.
 *  0: Cannot find it.
 */
int bit_vector_search(unsigned long v, int idx)
{
    bit_vector_t *vec = (bit_vector_t *)v;
    int byte_idx, bit_off;
    int *map;
    
    if (idx >= vec->size){
        printf("%s: Out of range\n", __FUNCTION__);
        return 0;
    }
    
    byte_idx = idx / BITS_OF_INT;
    bit_off = idx % BITS_OF_INT;
    map = &(vec->vector[byte_idx]);
    if (*map & (1 << bit_off))
        return 1;
    else
        return 0;
}

/*
 * Deletes a bit in the vector.
 *  v: The vector to be deleted from.
 *  idx: The bit to be deleted.
 * Return:
 *  0: Deleted successfully.
 *  -1: Failed to delete.
 */
int bit_vector_delete(unsigned long v, int idx)
{
    bit_vector_t *vec = (bit_vector_t *)v;
    int byte_idx, bit_off;
    int *map;
    
    if (idx >= vec->size){
        printf("%s: Out of range\n", __FUNCTION__);
        return -1;
    }
    
    byte_idx = idx / BITS_OF_INT;
    bit_off = idx % BITS_OF_INT;
    map = &(vec->vector[byte_idx]);
    if (!(*map & (1 << bit_off))){
        printf("Bit %d does not exists\n", idx);
        return -1;
    }
    
    *map &= ~(1 << bit_off);
    
    return 0;
}

/*
 * Get a free bit in the vector.
 *  v: The vector to be deleted from.
 * Return:
 *  >= 0: The index of the bit.
 *  -1: Failed to get a bit.
 */
int bit_vector_get(unsigned long v)
{
    bit_vector_t *vec = (bit_vector_t *)v;
    int size = vec->size;
    int *map, byte_idx, bit_off;
    
    size = (size + BITS_OF_INT - 1) / BITS_OF_INT;
    for (byte_idx = 0; byte_idx < size; byte_idx ++) {
        map = &(vec->vector[byte_idx]);
        if (*map != -1)
            break;
    }
    
    if (byte_idx >= size)
        return -1;
    
    for (bit_off = 0; bit_off < BITS_OF_INT; bit_off ++) {
        if ((*map & (1 << bit_off)) == 0) {
            *map |= 1 << bit_off;
            break;
        }
    }
    
    return (BITS_OF_INT * byte_idx + bit_off);
}