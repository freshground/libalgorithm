//
//  BinarySearchTree.c
//  libalgorithm
//
//  Created by sshe on 13-7-23.
//  Copyright (c) 2013年 sshe. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define ENABLE_RECURSION

typedef struct binary_tree_item_s{
    struct binary_tree_item_s *p, *left, *right;
    int key;
}binary_tree_item_t;

typedef struct binary_tree_s{
    binary_tree_item_t *root;
}binary_tree_t;

unsigned long binary_tree_init(void)
{
    binary_tree_t *tr;
    
    tr = malloc(sizeof(binary_tree_t));
    if (!tr){
        perror("Failed to allocate a tree");
        return 0;
    }
    
    memset(tr, 0, sizeof(binary_tree_t));
    
    return (unsigned long)tr;
}

void binary_tree_deinit(unsigned long tree)
{
    binary_tree_t *tr = (binary_tree_t *)tree;
    
    if (tr)
        free(tr);
}

unsigned long binary_tree_search(unsigned long tree, int key)
{
    binary_tree_t *tr = (binary_tree_t *)tree;
    binary_tree_item_t *x = tr->root;
    
    while (x && x->key != key) {
        if (x->key < key)
            x = x->right;
        else
            x = x->left;
    }
    
    return (unsigned long)x;
}

int binary_tree_get_value(unsigned long item)
{
    binary_tree_item_t *x = (binary_tree_item_t *)item;
    
    return x->key;
}

static binary_tree_item_t *binary_tree_minimum(binary_tree_item_t *x)
{
    while (x->left)
        x = x->left;
    
    return x;
}

static binary_tree_item_t *binary_tree_maximum(binary_tree_item_t *x)
{
    while (x->right)
        x = x->right;
    
    return x;
}


static binary_tree_item_t *binary_tree_successor(binary_tree_item_t *x)
{
    binary_tree_item_t *y;
    
    if (x->right)
        return binary_tree_minimum(x->right);
    
    y = x->p;
    while(y && x == y->right){
        x = y;
        y = y->p;
    }
    
    return y;
}

static binary_tree_item_t *binary_tree_predecessor(binary_tree_item_t *x)
{
    binary_tree_item_t *y;
    
    
    if (x->left)
        return binary_tree_maximum(x->left);
    
    y = x->p;
    while(y && x == y->left){
        x = y;
        y = y->p;
    }
    
    return y;
}

unsigned long binary_tree_next_item(unsigned long item)
{
    binary_tree_item_t *x = (binary_tree_item_t *)item, *y;
    
    y = binary_tree_successor(x);
    
    return (unsigned long)y;
}

unsigned long binary_tree_prev_item(unsigned long item)
{
    binary_tree_item_t *x = (binary_tree_item_t *)item, *y;
    
    y = binary_tree_predecessor(x);
    
    return (unsigned long)y;
}

int binary_tree_min_value(unsigned long tree)
{
    return binary_tree_minimum(((binary_tree_t *)tree)->root)->key;
}

unsigned long binary_tree_min_item(unsigned long tree)
{
    return (unsigned long)(binary_tree_minimum(((binary_tree_t *)tree)->root));
}

int binary_tree_max_value(unsigned long tree)
{
    return binary_tree_maximum(((binary_tree_t *)tree)->root)->key;
}

unsigned long binary_tree_max_item(unsigned long tree)
{
    return (unsigned long)(binary_tree_maximum(((binary_tree_t *)tree)->root));
}

unsigned long binary_tree_insert(unsigned long tree, int key)
{
    binary_tree_t *tr = (binary_tree_t *)tree;
    binary_tree_item_t *x = tr->root;
    binary_tree_item_t *y, *z;
    
    z = malloc(sizeof(binary_tree_item_t));
    if (!z){
        perror("Cannot insert new key");
        return 0;
    }
    
    z->key = key;
    z->left = z->right = NULL;
    
    y = NULL;
    while (x) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else if (z->key > x->key)
            x = x->right;
        else {
            printf("Insert: key %d already exist.\n", key);
            free(z);
            return 0;
        }
    }
    
    z->p = y;
    if (!y)
        tr->root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;
    
    return (unsigned long)z;
}

static void transplant(binary_tree_t *tree, binary_tree_item_t *u, binary_tree_item_t *v)
{
    if (!u->p)
        tree->root = v;
    else if (u == u->p->left)
        u->p->left = v;
    else
        u->p->right = v;
    
    if (v)
        v->p = u->p;
}

void binary_tree_delete_key(unsigned long tree, unsigned long item)
{
    binary_tree_t *tr = (binary_tree_t *)tree;
    binary_tree_item_t *z = (binary_tree_item_t *)item, *y;
    
    if (!z->left)
        transplant(tr, z, z->right);
    else if (!z->right)
        transplant(tr, z, z->left);
    else {
        y = binary_tree_minimum(z->right);
        if (y->p != z){
            transplant(tr, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        
        transplant(tr, z, y);
        y->left = z->left;
        y->left->p = y;
    }
    
    free(z);
}

#ifdef ENABLE_RECURSION
static void inorder_walk(binary_tree_item_t *x)
{
    if (x){
        inorder_walk(x->left);
        printf("%4d", x->key);
        inorder_walk(x->right);
    }
}
#else
static void inorder_walk(binary_tree_item_t *x)
{
    binary_tree_item_t *prev, *curr, *root;
    
    curr = x;
    prev = root = x->p;
    while (curr != root) {
        if (prev == curr->p){
            prev = curr;
            if (curr->left){
                curr = curr->left;
            }else{
                printf("%4d", curr->key);
                if (curr->right)
                    curr = curr->right;
                else
                    curr = curr->p;
            }
        }
        
        if (prev == curr->left){
            printf("%4d", curr->key);
            prev = curr;
            if (curr->right)
                curr = curr->right;
            else
                curr = curr->p;
        }
        
        if (prev == curr->right){
            prev = curr;
            curr = curr->p;
        }
    }
}
#endif

void binary_tree_inorder_walk(unsigned long tree)
{
    binary_tree_t *tr = (binary_tree_t *)tree;
    binary_tree_item_t *x = tr->root;
    
    inorder_walk(x);
    printf("\n");
}

#ifdef ENABLE_RECURSION
static void preorder_walk(binary_tree_item_t *x)
{
    if (x){
        printf("%4d", x->key);
        preorder_walk(x->left);
        preorder_walk(x->right);
    }
}
#else
static void preorder_walk(binary_tree_item_t *x)
{
    binary_tree_item_t *prev, *curr, *root;
    
    curr = x;
    prev = root = x->p;
    while (curr != root) {
        if (prev == curr->p){
            printf("%4d", curr->key);
            prev = curr;
            if (curr->left){
                curr = curr->left;
            }else{
                if (curr->right)
                    curr = curr->right;
                else
                    curr = curr->p;
            }
        }
        
        if (prev == curr->left){
            prev = curr;
            if (curr->right)
                curr = curr->right;
            else
                curr = curr->p;
        }
        
        if (prev == curr->right){
            prev = curr;
            curr = curr->p;
        }
    }
}
#endif

void binary_tree_preorder_walk(unsigned long tree)
{
    binary_tree_t *tr = (binary_tree_t *)tree;
    binary_tree_item_t *x = tr->root;
    
    preorder_walk(x);
    printf("\n");
}


#ifdef ENABLE_RECURSION
static void postorder_walk(binary_tree_item_t *x)
{
    if (x){
        postorder_walk(x->left);
        postorder_walk(x->right);
        printf("%4d", x->key);
    }
}
#else
static void postorder_walk(binary_tree_item_t *x)
{
    binary_tree_item_t *prev, *curr, *root;
    
    curr = x;
    prev = root = x->p;
    while (curr != root) {
        if (prev == curr->p){
            prev = curr;
            if (curr->left){
                curr = curr->left;
            }else{
                if (curr->right){
                    curr = curr->right;
                }else{
                    printf("%4d", curr->key);
                    curr = curr->p;
                }
            }
        }
        
        if (prev == curr->left){
            prev = curr;
            if (curr->right){
                curr = curr->right;
            }else{
                printf("%4d", curr->key);
                curr = curr->p;
            }
        }
        
        if (prev == curr->right){
            printf("%4d", curr->key);
            prev = curr;
            curr = curr->p;
        }
    }
}
#endif

void binary_tree_postorder_walk(unsigned long tree)
{
    binary_tree_t *tr = (binary_tree_t *)tree;
    binary_tree_item_t *x = tr->root;
    
    postorder_walk(x);
    printf("\n");
}
