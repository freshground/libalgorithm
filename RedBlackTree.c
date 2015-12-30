//
//  RedBlackTree.c
//  libalgorithm
//
//  Created by sshe on 13-10-15.
//  Copyright (c) 2013年 sshe. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct rb_tree_node_s {
    struct rb_tree_node_s *parent, *left, *right;
    unsigned int color;
    int key;
} rb_tree_node_t;

typedef struct rb_binary_tree_s {
    rb_tree_node_t *root;
    unsigned long bh; // black height
} rb_binary_tree_t;

#define RB_COLOR_BLACK  0
#define RB_COLOR_RED    1

static rb_tree_node_t node_nil = {
    .color = RB_COLOR_BLACK,
    .key = 0,
    .parent = &node_nil,
    .left = &node_nil,
    .right = &node_nil
};

#define RB_SET_BLACK(node)  do{(node)->color = RB_COLOR_BLACK;}while(0)
#define RB_SET_RED(node)  do{(node)->color = RB_COLOR_RED;}while(0)
#define RB_IS_BLACK(node) ((node)->color == RB_COLOR_BLACK)
#define RB_IS_RED(node) ((node)->color == RB_COLOR_RED)
#define RB_SET_NIL_NODE(node) ((node) = &node_nil)
#define RB_IS_NIL_NODE(node) (&node_nil == (node))
#define RB_GET_NIL_NODE() (&node_nil)
#define RB_IS_EMPTY(tree) (tree->root == &node_nil)

#define RB_IS_LEFT_CHILD(node) ((node) == (node)->parent->left)
#define RB_IS_RIGHT_CHILD(node) ((node) == (node)->parent->right)

#define RB_SIBLING(node) ((node) == (node)->parent->left? (node)->parent->right: (node)->parent->left)
#define RB_RIGHT_SIBING(node) ((node)->parent->right)
#define RB_LEFT_SIBING(node) ((node)->parent->left)

#define RB_COMPARE(a, b) ((a)->key - (b)->key)
#define RB_KEY(node) ((node)->key)

static int rb_insert(rb_binary_tree_t *tree, rb_tree_node_t *new_node);
static void rb_delete(rb_binary_tree_t *tree, rb_tree_node_t *del_node);
static rb_tree_node_t *rb_minimum(rb_binary_tree_t *tree, rb_tree_node_t *x);
static rb_tree_node_t *rb_maximum(rb_binary_tree_t *tree, rb_tree_node_t *x);
static rb_tree_node_t *rb_successor(rb_binary_tree_t *tree, rb_tree_node_t *x);
static rb_tree_node_t *rb_predecessor(rb_binary_tree_t *tree, rb_tree_node_t *x);
static void rb_inorder_walk(rb_binary_tree_t *tree, rb_tree_node_t *x);
static void rb_preorder_walk(rb_binary_tree_t *tree, rb_tree_node_t *x);
static void rb_postorder_walk(rb_binary_tree_t *tree, rb_tree_node_t *x);
static void rb_delete_all_nodes(rb_binary_tree_t *tree);
static rb_binary_tree_t * rb_join_trees(rb_binary_tree_t *left_tree, rb_binary_tree_t *right_tree);

unsigned long rb_tree_init(void)
{
    rb_binary_tree_t *tree = malloc(sizeof(rb_binary_tree_t));
    
    if (!tree){
        perror("Cannot allocate RB tree");
        return 0;
    }
    
    memset(tree, 0, sizeof(rb_binary_tree_t));
    RB_SET_NIL_NODE(tree->root);
    RB_SET_NIL_NODE(tree->root->parent);
    RB_SET_NIL_NODE(tree->root->left);
    RB_SET_NIL_NODE(tree->root->right);
    
    return (unsigned long)tree;
}

void rb_tree_deinit(unsigned long tr)
{
    rb_binary_tree_t *tree = (rb_binary_tree_t *)tr;
    
    rb_delete_all_nodes(tree);
    
    free(tree);
}

unsigned long rb_tree_search(unsigned long tr, int key)
{
    rb_binary_tree_t *tree = (rb_binary_tree_t *)tr;
    rb_tree_node_t *x = tree->root;
    
    while (!RB_IS_NIL_NODE(x) && x->key != key) {
        if (x->key < key)
            x = x->right;
        else
            x = x->left;
    }
    
    if (RB_IS_NIL_NODE(x))
        return 0;
    else
        return (unsigned long)x;
}

int rb_tree_get_value(unsigned long item)
{
    rb_tree_node_t *x = (rb_tree_node_t *)item;
    
    return x->key;
}

int rb_tree_insert(unsigned long tr, int value)
{
    rb_binary_tree_t *tree = (rb_binary_tree_t *)tr;
    rb_tree_node_t *new_node;
    
    new_node = malloc(sizeof(rb_tree_node_t));
    if (!new_node) {
        printf("Cannot allocate new node\n");
        return -1;
    }
    
    new_node->key = value;
    RB_SET_RED(new_node);
    RB_SET_NIL_NODE(new_node->parent);
    RB_SET_NIL_NODE(new_node->left);
    RB_SET_NIL_NODE(new_node->right);
    
    if (rb_insert(tree, new_node) < 0) {
        free(new_node);
        return -1;
    } else {
        return 0;
    }
}

void rb_tree_delete(unsigned long tr, unsigned long item)
{
    rb_binary_tree_t *tree = (rb_binary_tree_t *)tr;
    rb_tree_node_t *del_node = (rb_tree_node_t *)item;
    
    if (!RB_IS_NIL_NODE(del_node)) {
        rb_delete(tree, del_node);
        free(del_node);
    }
}

unsigned long rb_tree_next_item(unsigned long tr, unsigned long item)
{
    rb_binary_tree_t *tree = (rb_binary_tree_t *)tr;
    rb_tree_node_t *curr_node = (rb_tree_node_t *)item, *next_node;
    
    next_node = rb_successor(tree, curr_node);
    
    if (RB_IS_NIL_NODE(next_node))
        return 0;
    else
        return (unsigned long)next_node;
}

unsigned long rb_tree_prev_item(unsigned long tr, unsigned long item)
{
    rb_binary_tree_t *tree = (rb_binary_tree_t *)tr;
    rb_tree_node_t *curr_node = (rb_tree_node_t *)item, *prev_node;
    
    prev_node = rb_predecessor(tree, curr_node);
    
    if (RB_IS_NIL_NODE(prev_node))
        return 0;
    else
        return (unsigned long)prev_node;
}

int rb_tree_min_value(unsigned long tr)
{
    rb_binary_tree_t *tree = (rb_binary_tree_t *)tr;
    
    return rb_minimum(tree, tree->root)->key;
}

int rb_tree_max_value(unsigned long tr)
{
    rb_binary_tree_t *tree = (rb_binary_tree_t *)tr;
    
    return rb_maximum(tree, tree->root)->key;
}

unsigned long rb_tree_min_item(unsigned long tr)
{
    rb_binary_tree_t *tree = (rb_binary_tree_t *)tr;
    rb_tree_node_t *min_node;
    
    min_node = rb_minimum(tree, tree->root);
    
    if (RB_IS_NIL_NODE(min_node))
        return 0;
    else
        return (unsigned long)min_node;
}

unsigned long rb_tree_max_item(unsigned long tr)
{
    rb_binary_tree_t *tree = (rb_binary_tree_t *)tr;
    rb_tree_node_t *max_node;
    
    max_node = rb_maximum(tree, tree->root);
    
    if (RB_IS_NIL_NODE(max_node))
        return 0;
    else
        return (unsigned long)max_node;
}

void rb_tree_inorder_walk(unsigned long tr)
{
    rb_binary_tree_t *tree = (rb_binary_tree_t *)tr;
    
    rb_inorder_walk(tree, tree->root);
    printf("\n");
}

void rb_tree_preorder_walk(unsigned long tr)
{
    rb_binary_tree_t *tree = (rb_binary_tree_t *)tr;
    
    rb_preorder_walk(tree, tree->root);
    printf("\n");
}

void rb_tree_postorder_walk(unsigned long tr)
{
    rb_binary_tree_t *tree = (rb_binary_tree_t *)tr;
    
    rb_postorder_walk(tree, tree->root);
    printf("\n");
}

unsigned long rb_tree_join(unsigned long left_tr, unsigned long right_tr)
{
    rb_binary_tree_t *left_tree = (rb_binary_tree_t *)left_tr;
    rb_binary_tree_t *right_tree = (rb_binary_tree_t *)right_tr;
    rb_binary_tree_t *return_tree;
    
    return_tree = rb_join_trees(left_tree, right_tree);
    
    return (unsigned long)return_tree;
}

static void rb_left_rotate(rb_binary_tree_t *tree, rb_tree_node_t *sub_tree_root)
{
    // Set new_sub_tree_root to sub_tree_root's right child
    rb_tree_node_t *new_sub_tree_root = sub_tree_root->right;
    
    // Move new_sub_tree_root's left child as sub_tree_root's right child
    sub_tree_root->right = new_sub_tree_root->left;
    if (!RB_IS_NIL_NODE(sub_tree_root->right))
        sub_tree_root->right->parent = sub_tree_root;
    
    // Set new_sub_tree_root's parent to sub_tree_root's parent
    new_sub_tree_root->parent = sub_tree_root->parent;
    
    // Change child of sub_tree_root's parent
    if (RB_IS_NIL_NODE(new_sub_tree_root->parent))
        tree->root = new_sub_tree_root;
    else if (sub_tree_root == sub_tree_root->parent->left)
        sub_tree_root->parent->left = new_sub_tree_root;
    else
        sub_tree_root->parent->right = new_sub_tree_root;
    
    // Set sub_tree_root to new_sub_tree_root's left child
    new_sub_tree_root->left = sub_tree_root;
    sub_tree_root->parent = new_sub_tree_root;
}

static void rb_right_rotate(rb_binary_tree_t *tree, rb_tree_node_t *sub_tree_root)
{
    // Set new_sub_tree_root to sub_tree_root's left child
    rb_tree_node_t *new_sub_tree_root = sub_tree_root->left;
    
    // Move new_sub_tree_root's right child as sub_tree_root's left child
    sub_tree_root->left = new_sub_tree_root->right;
    if (!RB_IS_NIL_NODE(sub_tree_root->left))
        sub_tree_root->left->parent = sub_tree_root;
    
    // Set new_sub_tree_root's parent to sub_tree_root's parent
    new_sub_tree_root->parent = sub_tree_root->parent;
    
    // Change child of sub_tree_root's parent
    if (RB_IS_NIL_NODE(new_sub_tree_root->parent))
        tree->root = new_sub_tree_root;
    else if (sub_tree_root == sub_tree_root->parent->left)
        sub_tree_root->parent->left = new_sub_tree_root;
    else
        sub_tree_root->parent->right = new_sub_tree_root;
    
    // Set sub_tree_root to new_sub_tree_root's left child
    new_sub_tree_root->right = sub_tree_root;
    sub_tree_root->parent = new_sub_tree_root;
}

static void rb_insert_fixup(rb_binary_tree_t *tree, rb_tree_node_t *node)
{
    rb_tree_node_t *sibling_of_parent;
    
    while (RB_IS_RED(node->parent)) {
        if (RB_IS_LEFT_CHILD(node->parent)) {
            sibling_of_parent = RB_RIGHT_SIBING(node->parent);
            // case 1
            if (RB_IS_RED(sibling_of_parent)) {
                RB_SET_BLACK(sibling_of_parent);
                RB_SET_BLACK(node->parent);
                RB_SET_RED(node->parent->parent);
                node = node->parent->parent;
            } else {
                // case 2
                if (RB_IS_RIGHT_CHILD(node)) {
                    node = node->parent;
                    rb_left_rotate(tree, node);
                }
                
                // case 3
                RB_SET_BLACK(node->parent);
                RB_SET_RED(node->parent->parent);
                rb_right_rotate(tree, node->parent->parent);
            }
        } else { // if (RB_IS_RIGHT_CHILD(node->parent))
            sibling_of_parent = RB_LEFT_SIBING(node->parent);
            // case 1
            if (RB_IS_RED(sibling_of_parent)) {
                RB_SET_BLACK(sibling_of_parent);
                RB_SET_BLACK(node->parent);
                RB_SET_RED(node->parent->parent);
                node = node->parent->parent;
            } else {
                // case 2
                if (RB_IS_LEFT_CHILD(node)) {
                    node = node->parent;
                    rb_right_rotate(tree, node);
                }
                
                // case 3
                RB_SET_BLACK(node->parent);
                RB_SET_RED(node->parent->parent);
                rb_left_rotate(tree, node->parent->parent);
            }
        }
    }
    if (RB_IS_RED(tree->root)){
        RB_SET_BLACK(tree->root);
        tree->bh ++;
    }
}

static int rb_insert(rb_binary_tree_t *tree, rb_tree_node_t *new_node)
{
    rb_tree_node_t *parent, *child;
    
    RB_SET_NIL_NODE(parent);
    child = tree->root;
    while (!(RB_IS_NIL_NODE(child))) {
        parent = child;
        if (RB_COMPARE(new_node, child) > 0)
            child = child->right;
        else if (RB_COMPARE(new_node, child) < 0)
            child = child->left;
        else {
            printf("node with key %d already exists\n", RB_KEY(new_node));
            return -1;
        }
    }
    
    new_node->parent = parent;
    if (RB_IS_NIL_NODE(parent))
        tree->root = new_node;
    else if (RB_COMPARE(new_node, parent) < 0)
        parent->left = new_node;
    else
        parent->right = new_node;
    
    RB_SET_NIL_NODE(new_node->left);
    RB_SET_NIL_NODE(new_node->right);
    RB_SET_RED(new_node);
    rb_insert_fixup(tree, new_node);
    
    return 0;
}

static void rb_transplant(rb_binary_tree_t *tree, rb_tree_node_t *old_node, rb_tree_node_t *new_node)
{
    if (RB_IS_NIL_NODE(old_node->parent))
        tree->root = new_node;
    else if (RB_IS_LEFT_CHILD(old_node))
        old_node->parent->left = new_node;
    else
        old_node->parent->right = new_node;
    
    new_node->parent = old_node->parent;
}

static rb_tree_node_t *rb_minimum(rb_binary_tree_t *tree, rb_tree_node_t *x)
{
    while (!RB_IS_NIL_NODE(x->left))
        x = x->left;
    
    return x;
}

static rb_tree_node_t *rb_maximum(rb_binary_tree_t *tree, rb_tree_node_t *x)
{
    while (!RB_IS_NIL_NODE(x->right))
        x = x->right;
    
    return x;
}

static void rb_delete_fixup(rb_binary_tree_t *tree, rb_tree_node_t *fix_node)
{
    rb_tree_node_t *sibling;
    
    while (fix_node != tree->root && RB_IS_BLACK(fix_node)) {
        if (RB_IS_LEFT_CHILD(fix_node)) {
            sibling = RB_RIGHT_SIBING(fix_node);
            // case 1
            if (RB_IS_RED(sibling)) {
                RB_SET_BLACK(sibling);
                RB_SET_RED(fix_node->parent);
                rb_left_rotate(tree, fix_node->parent);
                sibling = RB_RIGHT_SIBING(fix_node);
            }
            
            // case 2
            if (RB_IS_BLACK(sibling->left) && RB_IS_BLACK(sibling->right)) {
                RB_SET_RED(sibling);
                fix_node = fix_node->parent;
                fix_node = fix_node->parent;
                if (fix_node == tree->root)
                    tree->bh --;
            } else {
                // case 3
                if (RB_IS_BLACK(sibling->right)) {
                    RB_SET_BLACK(sibling->left);
                    RB_SET_RED(sibling);
                    rb_right_rotate(tree, sibling);
                    sibling = RB_RIGHT_SIBING(fix_node);
                }
                
                // case 4
                sibling->color = fix_node->parent->color;
                RB_SET_BLACK(fix_node->parent);
                RB_SET_BLACK(sibling->right);
                rb_left_rotate(tree, fix_node->parent);
                fix_node = tree->root;
            }
        } else {
            sibling = RB_LEFT_SIBING(fix_node);
            // case 1
            if (RB_IS_RED(sibling)) {
                RB_SET_BLACK(sibling);
                RB_SET_RED(fix_node->parent);
                rb_right_rotate(tree, fix_node->parent);
                sibling = RB_LEFT_SIBING(fix_node);
            }
            
            // case 2
            if (RB_IS_BLACK(sibling->left) && RB_IS_BLACK(sibling->right)) {
                RB_SET_RED(sibling);
                fix_node = fix_node->parent;
                fix_node = fix_node->parent;
                if (fix_node == tree->root)
                    tree->bh --;
            } else {
                // case 3
                if (RB_IS_BLACK(sibling->left)) {
                    RB_SET_BLACK(sibling->right);
                    RB_SET_RED(sibling);
                    rb_left_rotate(tree, sibling);
                    sibling = RB_LEFT_SIBING(fix_node);
                }
                
                // case 4
                sibling->color = fix_node->parent->color;
                RB_SET_BLACK(fix_node->parent);
                RB_SET_BLACK(sibling->left);
                rb_right_rotate(tree, fix_node->parent);
                fix_node = tree->root;
            }
        }
    }
    
    RB_SET_BLACK(fix_node);
}

static void rb_delete(rb_binary_tree_t *tree, rb_tree_node_t *del_node)
{
    rb_tree_node_t *replace_node, *fix_node;
    unsigned int replace_node_original_color;
    
    replace_node = del_node;
    replace_node_original_color = replace_node->color;
    if (RB_IS_NIL_NODE(del_node->left)) {
        fix_node = del_node->right;
        rb_transplant(tree, del_node, fix_node);
    } else if (RB_IS_NIL_NODE(del_node->right)) {
        fix_node = del_node->left;
        rb_transplant(tree, del_node, fix_node);
    } else {
        replace_node = rb_minimum(tree, del_node->right);
        replace_node_original_color = replace_node->color;
        fix_node = replace_node->right;
        if (replace_node->parent == del_node)
            fix_node->parent = replace_node;
        else {
            rb_transplant(tree, replace_node, replace_node->right);
            replace_node->right = del_node->right;
            replace_node->right->parent = replace_node;
        }
        
        rb_transplant(tree, del_node, replace_node);
        replace_node->left = del_node->left;
        replace_node->left->parent = replace_node;
        replace_node->color = del_node->color;
    }
    
    if (replace_node_original_color == RB_COLOR_BLACK)
        rb_delete_fixup(tree, fix_node);
}

static rb_tree_node_t *rb_successor(rb_binary_tree_t *tree, rb_tree_node_t *x)
{
    rb_tree_node_t *y;
    
    if (!RB_IS_NIL_NODE(x->right))
        return rb_minimum(tree, x->right);
    
    y = x->parent;
    while(!RB_IS_NIL_NODE(y) && x == y->right){
        x = y;
        y = y->parent;
    }
    
    return y;
}

static rb_tree_node_t *rb_predecessor(rb_binary_tree_t *tree, rb_tree_node_t *x)
{
    rb_tree_node_t *y;
    
    
    if (!RB_IS_NIL_NODE(x->left))
        return rb_maximum(tree, x->left);
    
    y = x->parent;
    while(!RB_IS_NIL_NODE(y) && x == y->left){
        x = y;
        y = y->parent;
    }
    
    return y;
}

static void rb_inorder_walk(rb_binary_tree_t *tree, rb_tree_node_t *x)
{
    rb_tree_node_t *prev, *curr, *root;
    
    curr = x;
    prev = root = x->parent;
    while (curr != root) {
        if (prev == curr->parent) {
            prev = curr;
            if (!RB_IS_NIL_NODE(curr->left)) {
                curr = curr->left;
            } else {
                printf("%4d", curr->key);
                if (!RB_IS_NIL_NODE(curr->right))
                    curr = curr->right;
                else
                    curr = curr->parent;
            }
        }
        
        if (prev == curr->left) {
            printf("%4d", curr->key);
            prev = curr;
            if (!RB_IS_NIL_NODE(curr->right))
                curr = curr->right;
            else
                curr = curr->parent;
        }
        
        if (prev == curr->right) {
            prev = curr;
            curr = curr->parent;
        }
    }
}

static void rb_preorder_walk(rb_binary_tree_t *tree, rb_tree_node_t *x)
{
    rb_tree_node_t *prev, *curr, *root;
    
    curr = x;
    prev = root = x->parent;
    while (curr != root) {
        if (prev == curr->parent) {
            printf("%4d", curr->key);
            prev = curr;
            if (!RB_IS_NIL_NODE(curr->left)) {
                curr = curr->left;
            } else {
                if (!RB_IS_NIL_NODE(curr->right))
                    curr = curr->right;
                else
                    curr = curr->parent;
            }
        }
        
        if (prev == curr->left) {
            prev = curr;
            if (!RB_IS_NIL_NODE(curr->right))
                curr = curr->right;
            else
                curr = curr->parent;
        }
        
        if (prev == curr->right) {
            prev = curr;
            curr = curr->parent;
        }
    }
}

static void rb_postorder_walk(rb_binary_tree_t *tree, rb_tree_node_t *x)
{
    rb_tree_node_t *prev, *curr, *root;
    
    curr = x;
    prev = root = x->parent;
    while (curr != root) {
        if (prev == curr->parent) {
            prev = curr;
            if (!RB_IS_NIL_NODE(curr->left)) {
                curr = curr->left;
            } else {
                if (!RB_IS_NIL_NODE(curr->right)) {
                    curr = curr->right;
                } else {
                    printf("%4d", curr->key);
                    curr = curr->parent;
                }
            }
        }
        
        if (prev == curr->left) {
            prev = curr;
            if (!RB_IS_NIL_NODE(curr->right)) {
                curr = curr->right;
            } else {
                printf("%4d", curr->key);
                curr = curr->parent;
            }
        }
        
        if (prev == curr->right) {
            prev = curr;
            printf("%4d", curr->key);
            curr = curr->parent;
        }
    }
}

static void rb_delete_one_node(rb_binary_tree_t *tree, rb_tree_node_t *del_node)
{
    
    
    if (RB_IS_NIL_NODE(del_node))
        return;
    
    if (RB_IS_NIL_NODE(del_node->parent)){
        RB_SET_NIL_NODE(tree->root);
    } else {
        if (RB_IS_LEFT_CHILD(del_node))
            RB_SET_NIL_NODE(del_node->parent->left);
        else
            RB_SET_NIL_NODE(del_node->parent->right);
    }
    
    free(del_node);
}

static void rb_delete_all_nodes(rb_binary_tree_t *tree)
{
    rb_tree_node_t *curr, *del;
    int i = 0;
    
    curr = tree->root;
    while (!RB_IS_NIL_NODE(curr)) {
        if (!RB_IS_NIL_NODE(curr->left))
            curr = curr->left;
        else if (!RB_IS_NIL_NODE(curr->right))
            curr = curr->right;
        else{
            del = curr;
            curr = curr->parent;
            rb_delete_one_node(tree, del);
            i ++;
        }
    }
    
    printf("Destroyed %d items\n", i);
}

static rb_tree_node_t *rb_extract_mid_node(rb_binary_tree_t *small_tree, rb_binary_tree_t *big_tree)
{
    rb_tree_node_t *small_node, *big_node;
    
    if (RB_IS_EMPTY(small_tree) || RB_IS_EMPTY(big_tree)){
        printf("rb_extract_mid_node: Empty tree\n");
        return RB_GET_NIL_NODE();
    }
    
    small_node = rb_maximum(small_tree, small_tree->root);
    big_node = rb_minimum(big_tree, big_tree->root);
    if (RB_IS_NIL_NODE(small_node) || RB_IS_NIL_NODE(big_node)){
        printf("rb_extract_mid_node: failed to get big/small node\n");
        return RB_GET_NIL_NODE();
    }
    
    if (small_node->key >= big_node->key) {
        printf("rb_extract_mid_node: overlap between trees\n");
        return RB_GET_NIL_NODE();
    }
    
    if (small_tree->bh > big_tree->bh) {
        rb_delete(small_tree, small_node);
        return small_node;
    }else{
        rb_delete(big_tree, big_node);
        return big_node;
    }
}

static void rb_join_nodes(rb_tree_node_t *mid_node, rb_tree_node_t *left_node,
                          rb_tree_node_t *right_node, rb_tree_node_t *replaced_node,
                          rb_binary_tree_t *replaced_tree)
{
    rb_tree_node_t *parent;
    
    parent = replaced_node->parent;
    if (RB_IS_NIL_NODE(parent)) {
        replaced_tree->root = mid_node;
        RB_SET_NIL_NODE(mid_node->parent);
    } else {
        if (RB_IS_LEFT_CHILD(replaced_node))
            parent->left = mid_node;
        else
            parent->right = mid_node;
        
        mid_node->parent = parent;
    }
    
    mid_node->left = left_node;
    mid_node->right = right_node;
    left_node->parent = mid_node;
    right_node->parent = mid_node;
}

static rb_tree_node_t *rb_find_max_black_node_by_bh(rb_binary_tree_t *tree, unsigned long bh)
{
    rb_tree_node_t *found_node;
    unsigned long height = tree->bh;
    
    if (height < bh) {
        printf("%s: tree is too short %lu/%lu\n", __FUNCTION__, height, bh);
        return NULL;
    }
    
    found_node = tree->root;
    while (height > bh && RB_IS_BLACK(found_node)) {
        if (RB_IS_BLACK(found_node))
            height --;
        
        found_node = found_node->right;
    }
    
    if (RB_IS_NIL_NODE(found_node)) {
        printf("%s: failed to find black node with bh %lu\n", __FUNCTION__, bh);
        return NULL;
    }
    
    return found_node;
}

static rb_tree_node_t *rb_find_min_black_node_by_bh(rb_binary_tree_t *tree, unsigned long bh)
{
    rb_tree_node_t *found_node;
    unsigned long height = tree->bh;
    
    if (height < bh) {
        printf("%s: tree is too short %lu/%lu\n", __FUNCTION__, height, bh);
        return NULL;
    }
    
    found_node = tree->root;
    while (height > bh && RB_IS_BLACK(found_node)) {
        if (RB_IS_BLACK(found_node))
            height --;
        
        found_node = found_node->left;
    }
    
    if (RB_IS_NIL_NODE(found_node)) {
        printf("%s: failed to find black node with bh %lu\n", __FUNCTION__, bh);
        return NULL;
    }
    
    return found_node;
}

static rb_binary_tree_t * rb_join_trees(rb_binary_tree_t *left_tree, rb_binary_tree_t *right_tree)
{
    rb_tree_node_t *mid_node, *left_node, *right_node, *replace_node;
    rb_binary_tree_t *free_tree, *return_tree, *replaed_tree;
    
    mid_node = rb_extract_mid_node(left_tree, right_tree);
    if (!mid_node)
        return NULL;
    
    RB_SET_RED(mid_node);
    
    if (left_tree->bh > right_tree->bh) {
        left_node = rb_find_max_black_node_by_bh(left_tree, right_tree->bh);
        right_node = right_tree->root;
        replace_node = left_node;
        replaed_tree = left_tree;
        free_tree = right_tree;
        return_tree = left_tree;
    } else {
        left_node = left_tree->root;
        right_node = rb_find_min_black_node_by_bh(right_tree, left_tree->bh);
        replace_node = right_node;
        replaed_tree = right_tree;
        free_tree = left_tree;
        return_tree = right_tree;
    }
    
    rb_join_nodes(mid_node, left_node, right_node, replace_node, replaed_tree);
    rb_insert_fixup(return_tree, mid_node);
    free(free_tree);
    
    return return_tree;
}
