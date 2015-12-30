//
//  CutRod.c
//  libalgorithm
//
//  Created by sshe on 7/16/14.
//  Copyright (c) 2014 sshe. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(a, b) ((a) >= (b)? (a) : (b))

int cut_rod(int *price, int n, int cut_cost)
{
    int q = INT32_MIN;
    int i;
    
    if (n <= 0)
        return 0;
    
    for (i = 1; i <= n; i ++) {
        int new_q = price[i - 1] + cut_rod(price, n - i, cut_cost);
        if (i < n)
            new_q -= cut_cost;
        
        q = max(q, new_q);
    }
    
    return q;
}

static int memoized_cut_rod_aux(int *price, int n, int *r, int *s, int cut_cost)
{
    int i, q;
    
    if (n <= 0)
        return 0;
    
    if (r[n] > INT32_MIN)
        return r[n];
    
    q = INT32_MIN;
    for (i = 1; i <= n; i ++) {
        int new_q = price[i - 1] + memoized_cut_rod_aux(price, n - i, r, s, cut_cost);
        if (i < n)
            new_q -= cut_cost;
        
        if (q < new_q) {
            q = new_q;
            s[n] = i;
        }
        
    }
    
    r[n] = q;
    
    return q;
}

int memoized_cut_rod(int *price, int n, int cut_cost)
{
    int *r, *s, revenue, i;
    
    revenue = 0;
    
    s = malloc(sizeof(int) * (n + 1));
    if (!s) {
        perror("memoized_cut_rod failed to malloc s array");
        goto failed_alloc_s;
    }
    
    memset(s, 0, sizeof(int) * (n + 1));
    
    r = malloc(sizeof(int) * (n + 1));
    if (!r) {
        perror("memoized_cut_rod failed to malloc r array");
        goto failed_alloc_r;
    }
    
    for (i = 0; i <= n; i ++)
        r[i] = INT32_MIN;
    
    revenue = memoized_cut_rod_aux(price, n, r, s, cut_cost);
    
    while (n > 0) {
        printf("%4d", s[n]);
        n -= s[n];
    }
    
    printf("\n");
    
    free(r);
failed_alloc_r:
    free(s);
failed_alloc_s:
    return revenue;
}

int bottom_up_cut_rod(int *price, int n, int cut_cost)
{
    int *r, revenue, i, j;
    
    r = malloc(sizeof(int) * (n + 1));
    if (!r){
        perror("bottom_up_cut_rod failed to malloc r array");
        return 0;
    }
    
    r[0] = 0;
    for (j = 1; j <= n; j ++) {
        int q = INT32_MIN;
        
        for (i = 1; i <= j; i ++) {
            int new_q = price[i - 1] + r[j - i];
            if (i < j)
                new_q -= cut_cost;
            q = max(q, new_q);
        }
        
        r[j] = q;
    }
    
    revenue = r[n];
    free(r);
    
    return revenue;
}

static int extended_bottom_up_cut_rod(int *price, int n, int *s, int cut_cost)
{
    int *r, i, j;
    
    r = malloc(sizeof(int) * (n + 1));
    if (!r){
        perror("extended_bottom_up_cut_rod failed to malloc r array");
        return -1;
    }
    
    r[0] = 0;
    for (j = 1; j <= n; j ++) {
        int q = INT32_MIN;
        
        for (i = 1; i <= j; i ++) {
            int new_q = price[i - 1] + r[j - i];
            if (i < j)
                new_q -= cut_cost;
            if (q < new_q) {
                q = new_q;
                s[j] = i;
            }
        }
        
        r[j] = q;
    }
    
    free(r);
    
    return 0;
}

void print_cut_rod_solution(int *price, int n, int cut_cost)
{
    int *s;
    
    s = malloc(sizeof(int) * (n + 1));
    if (!s) {
        perror("print_cut_rod_solution failed to malloc s array");
        return;
    }
    
    if (extended_bottom_up_cut_rod(price, n, s, cut_cost) < 0)
        return;
    
    while (n > 0) {
        printf("%4d", s[n]);
        n -= s[n];
    }
    
    printf("\n");
    free(s);
}