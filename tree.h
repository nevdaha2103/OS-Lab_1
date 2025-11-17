#ifndef TREE_H
#define TREE_H

#include <stddef.h>
#include <stdbool.h>

struct Node {
    size_t key;
    void* data;  // Вказівник на блок
    int height;
    struct Node* left;
    struct Node* right;
};

/* Створення нового вузла з ключем та даними */
struct Node* node_new(size_t key, void* data);

/* Обчислення висоти вузла */
int node_height(struct Node* node);

/* Фіксація висоти за дітьми */
void node_fix_height(struct Node* node);

/* Повороти AVL */
struct Node* node_rotate_right(struct Node* p);
struct Node* node_rotate_left(struct Node* q);

/* Балансування вузла */
struct Node* node_balance(struct Node* node);

/* Вставка ключа з даними */
struct Node* node_insert(struct Node* root, size_t key, void* data);

/* Пошук найменшого ключа */
struct Node* node_find_min(struct Node* root);

/* Видалення мінімального */
struct Node* node_remove_min(struct Node* root);

/* Видалення ключа */
struct Node* node_remove(struct Node* root, size_t key);

/* Діагностика дерева */
void node_show(struct Node* node);

#endif