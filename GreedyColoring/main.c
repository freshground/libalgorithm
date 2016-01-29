//
//  main.c
//  ActivityScheduler
//
//  Created by sshe on 15/9/17.
//  Copyright (c) 2015年 sshe. All rights reserved.
//  16.1-4 in <<introduction to algorithm>>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "GreedyColoring.h"

typedef int (*sort_compare_t)(void *, void *);

#define ALGO_LIB "/Users/sheman/github/github-libalgorithm/build/Debug/libgithub-libalgorithm.dylib"
void *lib_handle;
static void load_algo_lib(void);
static void unload_algo_lib(void);
void (*quick_sort_general)(void *keys[], int p, int r, sort_compare_t compare);

// BitVector functions
unsigned long (*bit_vector_init)(int size);
void (*bit_vector_deinit)(unsigned long v);
int (*bit_vector_insert)(unsigned long v, int idx);
int (*bit_vector_search)(unsigned long v, int idx);
int (*bit_vector_delete)(unsigned long v, int idx);
int (*bit_vector_get)(unsigned long v);
int (*bit_vector_get_num_of_set_bit)(unsigned long v);
int (*bit_vector_get_num_of_common_bit)(unsigned long v1, unsigned long v2);

static unsigned long events_init(event_interval_t *e, int count);
static void events_deinit(unsigned long events);

static event_interval_t events[] = {{0, 40}, {10, 30}, {12, 28}, {13, 25}, {15, 19}, {21, 25}};

int main(int argc, const char * argv[])
{
    unsigned long event_set = 0;
    int event_count = sizeof(events) / sizeof(event_interval_t);
    
    load_algo_lib();
    event_set = events_init(events, event_count);
    events_coloring(event_set);
    events_show_colors(event_set);
    events_deinit(event_set);
    unload_algo_lib();
    
    return 0;
}

#define INTERVAL_OVERLAP(x, y) (((x).a <= (y).b) && ((x).b >= (y).a))

static unsigned long events_init(event_interval_t *e, int count)
{
    vertex_set_array_t *array;
    
    array = malloc(sizeof(vertex_set_array_t));
    if (!array) {
        perror("Failed to alloc array");
        goto fail_alloc_array;
    }
    
    array->event_intervals = malloc(sizeof(event_interval_t) * count);
    if (!array->event_intervals) {
        perror("Failed to alloc events");
        goto fail_alloc_events;
    }
    
    array->sets = malloc(sizeof(vertex_set_t *) * count);
    if (!array->sets) {
        perror("Failed to alloc sets");
        goto fail_alloc_sets;
    }
    
    array->sorted_vertex_sets = vertex_sorted_key_init(count);
    if (!array->sorted_vertex_sets) {
        perror("Failed to init sorted sets");
        goto fail_init_sorted_sets;
    }
    
    array->num_of_event = count;
    memcpy(array->event_intervals, e, sizeof(event_interval_t) * count);
    
    if (events_sets_init(array) < 0)
        goto fail_init_sets_array;
    
    if (build_neighbors_all_sets(array) < 0)
        goto fail_init_sets_array;
    
    return (unsigned long)array;
    
fail_init_sets_array:
    vertex_sorted_key_deinit(array->sorted_vertex_sets);
fail_init_sorted_sets:
    free(array->sets);
fail_alloc_sets:
    free(array->event_intervals);
fail_alloc_events:
    free(array);
fail_alloc_array:
    return 0;
}

static void events_deinit(unsigned long events)
{
    vertex_set_array_t *array = (vertex_set_array_t *)events;
    
    if (array) {
        if (array->event_intervals)
            free(array->event_intervals);
        
        if (array->sets) {
            events_sets_deinit(array);
            free(array->sets);
        }
        
        if (array->sorted_vertex_sets)
            vertex_sorted_key_deinit(array->sorted_vertex_sets);
        
        free(array);
    }
}

static void events_coloring(unsigned long events)
{
    vertex_set_array_t *array = (vertex_set_array_t *)events;
    
    if (array) {
        events_setup_keys(array);
        events_sort_keys(array);
        coloring_vertice(array);
    }
}

static void events_show_colors(unsigned long events)
{
    vertex_set_array_t *array = (vertex_set_array_t *)events;
    int i;
    
    for (i = 0; i < array->num_of_event; i ++) {
        printf("v%d [%d, %d]: %d\n", i, array->event_intervals[i].a,
               array->event_intervals[i].b, array->sets[i]->vertex->color);
    }
}

static int build_neighbors_all_sets(vertex_set_array_t *array)
{
    int i, ret = 0;
    
    for (i = 0; i < array->num_of_event; i ++) {
        ret = build_neighbors_one_set(array, i);
        if (ret < 0)
            break;
        
        (array->sorted_vertex_sets->keys)[i].event_idx = i;
    }
    
    return ret;
}

static int build_neighbors_one_set(vertex_set_array_t *array, int current_idx)
{
    vertex_set_t *current_set = (array->sets)[current_idx];
    event_interval_t current_interval;
    int array_idx, num_of_neighbor = 0;
    
    current_interval.a = array->event_intervals[current_idx].a;
    current_interval.b = array->event_intervals[current_idx].b;
    
    for (array_idx = 0; array_idx < array->num_of_event; array_idx ++) {
        if (array_idx == current_idx)
            continue;
        
        if (INTERVAL_OVERLAP(current_interval, array->event_intervals[array_idx])) {
            if (bit_vector_insert(current_set->neighbors, array_idx) < 0)
                return -1;
            
            (current_set->sorted_neighbors->keys)[num_of_neighbor].event_idx = array_idx;
            num_of_neighbor ++;
        }
    }
    
    current_set->num_of_neighbor = num_of_neighbor;
    
    return 0;
}

static vertex_t *vertex_init(int event_idx, int color)
{
    vertex_t *v = malloc(sizeof(vertex_t));
    
    if (!v) {
        perror("failed to alloc a vertex");
        return NULL;
    }
    
    v->color = color;
    v->event_idx = event_idx;
    v->key = 0;
    
    return v;
}

static void vertex_deinit(vertex_t *v)
{
    free(v);
}

static sorted_vertex_set_t *vertex_sorted_key_init(int count)
{
    sorted_vertex_set_t *k = malloc(sizeof(sorted_vertex_set_t));
    int i;
    
    if (!k) {
        perror("Failed to alloc sorted_vertex_set_t");
        goto fail_alloc_k;
    }
    
    k->keys = malloc(sizeof(vertex_t) * count);
    if (!k->keys) {
        perror("Failed to alloc vertex_key_t");
        goto fail_alloc_keys;
    }
    
    k->key_ptrs = malloc(sizeof(vertex_t *) * count);
    if (!k->key_ptrs) {
        perror("Failed to alloc key ptrs");
        goto fail_alloc_key_ptr;
    }
    
    k->num_of_vertex_set = count;
    memset(k->keys, 0, sizeof(vertex_t) * count);
    for (i = 0; i < count; i ++)
        k->key_ptrs[i] = &(k->keys[i]);
    
    return k;
    
fail_alloc_key_ptr:
    free(k->keys);
fail_alloc_keys:
    free(k);
fail_alloc_k:
    return NULL;
}

static void vertex_sorted_key_deinit(sorted_vertex_set_t *k)
{
    if (k) {
        if (k->keys)
            free(k->keys);
        
        if (k->key_ptrs)
            free(k->key_ptrs);
        
        free(k);
    }
}

static vertex_set_t *vertex_set_init(vertex_t *v, int max_num_of_neighbors)
{
    vertex_set_t *set = malloc(sizeof(vertex_set_t));
    
    if (!set) {
        perror("Failed to alloc vertex set");
        goto fail_alloc_set;
    }
    
    set->neighbors = bit_vector_init(max_num_of_neighbors);
    if (!set->neighbors) {
        perror("Failed to alloc neighbors");
        goto fail_init_neighbor;
    }
    
    set->colors_in_neighbors = bit_vector_init(max_num_of_neighbors);
    if (!set->colors_in_neighbors) {
        perror("Failed to alloc colors");
        goto fail_init_colors;
    }
    
    set->sorted_neighbors = vertex_sorted_key_init(max_num_of_neighbors);
    if (!set->sorted_neighbors) {
        perror("Failed to alloc sorted neighbors");
        goto fail_alloc_sorted_neighbors;
    }
    
    set->vertex = v;
    set->num_of_neighbor = 0;
    
    return set;
   
fail_alloc_sorted_neighbors:
    bit_vector_deinit(set->colors_in_neighbors);
fail_init_colors:
    bit_vector_deinit(set->neighbors);
fail_init_neighbor:
    free(set);
fail_alloc_set:
    return NULL;
}

static void vertex_set_deinit(vertex_set_t *set)
{
    if (set) {
        if (set->vertex)
            vertex_deinit(set->vertex);
        
        if (set->neighbors)
            bit_vector_deinit(set->neighbors);
        
        if (set->colors_in_neighbors)
            bit_vector_deinit(set->colors_in_neighbors);
        
        if (set->sorted_neighbors)
            vertex_sorted_key_deinit(set->sorted_neighbors);
        
        free(set);
    }
}

static int events_sets_init(vertex_set_array_t *array)
{
    int i, count = array->num_of_event;
    
    for (i = 0; i < count; i ++) {
        vertex_t *v = vertex_init(i, COLOR_UNKNOWN);
        
        if (!v) {
            perror("Failed to alloc vertex for events");
            goto failed;
        }
        
        (array->sets)[i] = vertex_set_init(v, count);
        if (!(array->sets)[i]) {
            perror("Failed to alloc vertex set for events");
            goto failed;
        }
    }
    
    return 0;
    
failed:
    for (i = 0; i < count; i ++) {
        vertex_set_deinit((array->sets)[i]);
    }
    
    return -1;
}

static void events_sets_deinit(vertex_set_array_t *array)
{
    int i;
    
    for (i = 0; i < array->num_of_event; i ++)
        vertex_set_deinit((array->sets[i]));
}

static void events_setup_keys(vertex_set_array_t *array)
{
    int i, j;
    
    for (i = 0; i < array->num_of_event; i ++) {
        vertex_set_t *current_set = array->sets[i];
        
        // Set keys in the array
        (array->sorted_vertex_sets->keys)[i].key = current_set->num_of_neighbor;
        // Set keys for each set's neighbor
        for (j = 0; j < current_set->num_of_neighbor; j ++) {
            int vetex_idx = current_set->sorted_neighbors->keys[j].event_idx;
            
            current_set->sorted_neighbors->keys[j].key =
            bit_vector_get_num_of_common_bit(current_set->neighbors,
                                             array->sets[vetex_idx]->neighbors);
        }
    }
}

static void events_sort_keys(vertex_set_array_t *array)
{
    int i;
    
    quick_sort_general((void **)(array->sorted_vertex_sets->key_ptrs), 0, array->num_of_event - 1, vetex_compare);
    
    for (i = 0; i < array->num_of_event; i ++) {
        vertex_set_t *current_set = array->sets[i];
        
        quick_sort_general((void **)(current_set->sorted_neighbors->key_ptrs),
                           0, current_set->num_of_neighbor - 1, vetex_compare);
    }
}

static void coloring_one_vertex(vertex_set_array_t *array, vertex_set_t *current_set)
{
    int color, i, found;
    
    color = bit_vector_get(current_set->colors_in_neighbors);
    current_set->vertex->color = color;
    
    // Add my color to my neighbors' colors_in_neighbors
    for (i = 0; i < current_set->num_of_neighbor; i ++) {
        int current_neighbor_idx = current_set->sorted_neighbors->keys[i].event_idx;
        vertex_set_t *current_neighbor = array->sets[current_neighbor_idx];
        
        found = bit_vector_search(current_neighbor->colors_in_neighbors, color);
        if (found)
            printf("Color %d is already in set %d's neighbor %d's neigcolors_in_neighborshbo\n",
                   color, current_set->vertex->event_idx, current_neighbor->vertex->event_idx);
        else
            bit_vector_insert(current_neighbor->colors_in_neighbors, color);
        
    }
}

static void coloring_vertice(vertex_set_array_t *array)
{
    int i;
    
    for (i = 0; i < array->num_of_event; i ++) {
        vertex_set_t *current_set = array->sets[i];
        
        if (current_set->vertex->color == COLOR_UNKNOWN) {
            int j;
            
            // Color current vertex
            coloring_one_vertex(array, current_set);
            // Color all of its sorted neighbors
            for (j = 0; j < current_set->num_of_neighbor; j ++) {
                int current_neighbor_idx = current_set->sorted_neighbors->keys[j].event_idx;
                vertex_set_t *current_neighbor = array->sets[current_neighbor_idx];
                
                if (current_neighbor->vertex->color == COLOR_UNKNOWN)
                    coloring_one_vertex(array, current_neighbor);
            }
        }
    }
}

/*
 * v1 < v2: return 1;
 * v1 == v2: return 0;
 * v1 > v2: return -1;
 */
int vetex_compare(void *v1, void *v2)
{
    vertex_t *vetex1 = (vertex_t *)v1;
    vertex_t *vetex2 = (vertex_t *)v2;
    
    return ((vetex1->key < vetex2->key) - (vetex1->key > vetex2->key));
}

static void load_algo_lib(void)
{
    lib_handle = dlopen(ALGO_LIB, RTLD_LAZY);
    if (!lib_handle){
        printf("dlopen: %s\n", dlerror());
        exit(-1);
    }
    
    quick_sort_general = dlsym(lib_handle, "quick_sort_general");
    if (!quick_sort_general){
        printf("quick_sort_general: %s\n", dlerror());
        exit(-1);
    }
    
    bit_vector_init = dlsym(lib_handle, "bit_vector_init");
    if (!bit_vector_init){
        printf("bit_vector_init: %s\n", dlerror());
        exit(-1);
    }
    
    bit_vector_deinit = dlsym(lib_handle, "bit_vector_deinit");
    if (!bit_vector_deinit){
        printf("bit_vector_deinit: %s\n", dlerror());
        exit(-1);
    }
    
    bit_vector_insert = dlsym(lib_handle, "bit_vector_insert");
    if (!bit_vector_insert){
        printf("bit_vector_insert: %s\n", dlerror());
        exit(-1);
    }
    
    bit_vector_get = dlsym(lib_handle, "bit_vector_get");
    if (!bit_vector_get){
        printf("bit_vector_get: %s\n", dlerror());
        exit(-1);
    }
    
    bit_vector_search = dlsym(lib_handle, "bit_vector_search");
    if (!bit_vector_search){
        printf("bit_vector_search: %s\n", dlerror());
        exit(-1);
    }
    
    bit_vector_get_num_of_set_bit = dlsym(lib_handle, "bit_vector_get_num_of_set_bit");
    if (!bit_vector_get_num_of_set_bit){
        printf("bit_vector_get_num_of_set_bit: %s\n", dlerror());
        exit(-1);
    }
    
    bit_vector_get_num_of_common_bit = dlsym(lib_handle, "bit_vector_get_num_of_common_bit");
    if (!bit_vector_get_num_of_common_bit){
        printf("bit_vector_get_num_of_common_bit: %s\n", dlerror());
        exit(-1);
    }
}

static void unload_algo_lib(void)
{
    dlclose(lib_handle);
}