//
//  GreedyColoring.h
//  ActivityScheduler
//
//  Created by sshe on 16/1/26.
//  Copyright © 2016年 sshe. All rights reserved.
//

#ifndef GreedyColoring_h
#define GreedyColoring_h


#define COLOR_UNKNOWN -1

typedef struct event_interval_s {
    int a, b;
} event_interval_t;

typedef struct vertex_s {
    int event_idx;
    int color;
    int key; // used in sorted key array
} vertex_t;

typedef struct sorted_vertex_set_s {
    vertex_t *keys;
    vertex_t **key_ptrs;
    int num_of_vertex_set;
} sorted_vertex_set_t;

typedef struct vertex_set_s {
    vertex_t *vertex;
    sorted_vertex_set_t *sorted_neighbors;
    unsigned long neighbors; // bitmap of neighbors
    unsigned long colors_in_neighbors; // bitmap of the neighbors' color
    int num_of_neighbor;
} vertex_set_t;

typedef struct vertex_set_array_s {
    event_interval_t *event_intervals;
    vertex_set_t **sets;
    sorted_vertex_set_t *sorted_vertex_sets;
    int num_of_event;
} vertex_set_array_t;

// Called by main()
static vertex_t *vertex_init(int event_idx, int color);
static void vertex_deinit(vertex_t *v);
static void events_coloring(unsigned long events);
static void events_show_colors(unsigned long events);

// Internal functions
static sorted_vertex_set_t *vertex_sorted_key_init(int count);
static void vertex_sorted_key_deinit(sorted_vertex_set_t *k);
static vertex_set_t *vertex_set_init(vertex_t *v, int max_num_of_neighbors);
static void vertex_set_deinit(vertex_set_t *set);
static int events_sets_init(vertex_set_array_t *array);
static void events_sets_deinit(vertex_set_array_t *array);
static int build_neighbors_one_set(vertex_set_array_t *array, int current_idx);
static int build_neighbors_all_sets(vertex_set_array_t *array);
static unsigned long events_init(event_interval_t *e, int count);
static void events_deinit(unsigned long events);
static void events_setup_keys(vertex_set_array_t *array);
int vetex_compare(void *v1, void *v2);
static void events_sort_keys(vertex_set_array_t *array);
static void coloring_one_vertex(vertex_set_array_t *array, vertex_set_t *current_set);
static void coloring_vertice(vertex_set_array_t *array);

#endif /* GreedyColoring_h */
