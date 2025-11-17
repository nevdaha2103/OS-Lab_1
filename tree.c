#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

static int max2(int a, int b) { return a > b ? a : b; }

struct Node* node_new(size_t key, void* data) {
    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    if (!n) return NULL;
    n->key = key;
    n->data = data;
    n->height = 1;
    n->left = NULL;
    n->right = NULL;
    return n;
}

int node_height(struct Node* node) {
    return node ? node->height : 0;
}

void node_fix_height(struct Node* node) {
    int hl = node_height(node->left);
    int hr = node_height(node->right);
    node->height = max2(hl, hr) + 1;
}

int node_bfactor(struct Node* node) {
    return node_height(node->right) - node_height(node->left);
}

struct Node* node_rotate_right(struct Node* p) {
    struct Node* q = p->left;
    p->left = q->right;
    q->right = p;
    node_fix_height(p);
    node_fix_height(q);
    return q;
}

struct Node* node_rotate_left(struct Node* q) {
    struct Node* p = q->right;
    q->right = p->left;
    p->left = q;
    node_fix_height(q);
    node_fix_height(p);
    return p;
}

struct Node* node_balance(struct Node* node) {
    node_fix_height(node);
    if (node_bfactor(node) == 2) {
        if (node_bfactor(node->right) < 0)
            node->right = node_rotate_right(node->right);
        return node_rotate_left(node);
    }
    if (node_bfactor(node) == -2) {
        if (node_bfactor(node->left) > 0)
            node->left = node_rotate_left(node->left);
        return node_rotate_right(node);
    }
    return node;
}

struct Node* node_insert(struct Node* root, size_t key, void* data) {
    if (!root) return node_new(key, data);
    if (key < root->key)
        root->left = node_insert(root->left, key, data);
    else
        root->right = node_insert(root->right, key, data);
    return node_balance(root);
}

struct Node* node_find_min(struct Node* root) {
    return root->left ? node_find_min(root->left) : root;
}

struct Node* node_remove_min(struct Node* root) {
    if (!root->left) return root->right;
    root->left = node_remove_min(root->left);
    return node_balance(root);
}

struct Node* node_remove(struct Node* root, size_t key) {
    if (!root) return NULL;
    if (key < root->key) {
        root->left = node_remove(root->left, key);
    } else if (key > root->key) {
        root->right = node_remove(root->right, key);
    } else {
        struct Node* l = root->left;
        struct Node* r = root->right;
        free(root);
        if (!r) return l;
        struct Node* min = node_find_min(r);
        min->right = node_remove_min(r);
        min->left = l;
        return node_balance(min);
    }
    return node_balance(root);
}

void node_show(struct Node* node) {
    if (!node) {
        printf("  (empty tree)\n");
        return;
    }
    node_show(node->left);
    printf("  Key: %zu, Data: %p\n", node->key, node->data);
    node_show(node->right);
}