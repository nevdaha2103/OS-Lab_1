#ifndef BLOCK_H
#define BLOCK_H

#include <stddef.h>
#include <stdbool.h>

/* Теги у молодших бітах */
enum {
    BLOCK_FLAG_BUSY  = 1 << 0,
    BLOCK_FLAG_FIRST = 1 << 1,
    BLOCK_FLAG_LAST  = 1 << 2
};

typedef struct Block {
    /* Розмір поточного блока з тегами; включає заголовок */
    size_t size_flags;
    /* Розмір попереднього блока з тегами */
    size_t prev_size_flags;
    /* Далі йде payload або службові поля для вільного блока */
} Block;

/* Розмір заголовка блока */
size_t block_header_size(void);

/* Отримати чистий розмір (без флагів) */
size_t block_get_size(Block*);

/* Встановити розмір (зберігаючи/оновлюючи флаги) */
void block_set_size(Block*, size_t);

/* Розміри prev */
size_t block_get_size_prev(Block*);
void block_set_size_prev(Block*, size_t);

/* Флаги */
bool block_get_flag_busy(Block*);
bool block_get_flag_first(Block*);
bool block_get_flag_last(Block*);

void block_set_flag_busy(Block*, bool);
void block_set_flag_first(Block*, bool);
void block_set_flag_last(Block*, bool);

/* Перехід до наступного/попереднього блока за офсетами */
Block* block_next(Block* base, Block* cur, size_t arena_size);
Block* block_prev(Block* base, Block* cur);

/* Доступ до payload */
void* block_payload(Block*);

/* Допоміжне вирівнювання */
size_t align_up(size_t x, size_t a);

#endif
void block_initialize(Block* b, size_t size, bool busy, bool first, bool last);