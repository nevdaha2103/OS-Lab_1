#include "block.h"
#include <stddef.h>
#include <stdint.h>

#ifndef MAX_ALIGN
#define MAX_ALIGN (sizeof(long double))
#endif

size_t align_up(size_t x, size_t a) {
    return (x + (a - 1)) & ~(a - 1);
}

static inline size_t mask_flags(size_t v) {
    return v & ~(size_t)(BLOCK_FLAG_BUSY | BLOCK_FLAG_FIRST | BLOCK_FLAG_LAST);
}

static inline size_t take_flags(size_t v) {
    return v & (size_t)(BLOCK_FLAG_BUSY | BLOCK_FLAG_FIRST | BLOCK_FLAG_LAST);
}

size_t block_header_size(void) {
    return align_up(sizeof(Block), MAX_ALIGN);
}

size_t block_get_size(Block* b) {
    if (b == NULL) return 0;
    return mask_flags(b->size_flags);
}

void block_set_size(Block* b, size_t size) {
    if (b == NULL) return;
    size_t flags = take_flags(b->size_flags);
    b->size_flags = size | flags;
}

size_t block_get_size_prev(Block* b) {
    if (b == NULL) return 0;
    return mask_flags(b->prev_size_flags);
}

void block_set_size_prev(Block* b, size_t size) {
    if (b == NULL) return;
    size_t flags = take_flags(b->prev_size_flags);
    b->prev_size_flags = size | flags;
}

bool block_get_flag_busy(Block* b) {
    if (b == NULL) return false;
    return (b->size_flags & BLOCK_FLAG_BUSY) != 0;
}

bool block_get_flag_first(Block* b) {
    if (b == NULL) return false;
    return (b->size_flags & BLOCK_FLAG_FIRST) != 0;
}

bool block_get_flag_last(Block* b) {
    if (b == NULL) return false;
    return (b->size_flags & BLOCK_FLAG_LAST) != 0;
}

void block_set_flag_busy(Block* b, bool v) {
    if (b == NULL) return;
    if (v)
        b->size_flags |= BLOCK_FLAG_BUSY;
    else
        b->size_flags &= ~BLOCK_FLAG_BUSY;
}

void block_set_flag_first(Block* b, bool v) {
    if (b == NULL) return;
    if (v)
        b->size_flags |= BLOCK_FLAG_FIRST;
    else
        b->size_flags &= ~BLOCK_FLAG_FIRST;
}

void block_set_flag_last(Block* b, bool v) {
    if (b == NULL) return;
    if (v)
        b->size_flags |= BLOCK_FLAG_LAST;
    else
        b->size_flags &= ~BLOCK_FLAG_LAST;
}

void* block_payload(Block* b) {
    if (b == NULL) return NULL;
    return (void*)((unsigned char*)b + block_header_size());
}

Block* block_next(Block* base, Block* cur, size_t arena_size) {
    if (cur == NULL || base == NULL) return NULL;

    size_t sz = block_get_size(cur);
    if (sz == 0) return NULL;

    unsigned char* p = (unsigned char*)cur;
    unsigned char* n = p + sz;
    unsigned char* arena_end = (unsigned char*)base + arena_size;

    if (n >= arena_end) return NULL;
    return (Block*)n;
}

Block* block_prev(Block* base, Block* cur) {
    (void)base; // Не використовується, але залишаємо для консистентності

    if (cur == NULL) return NULL;

    size_t psz = block_get_size_prev(cur);
    if (psz == 0) return NULL;

    unsigned char* p = (unsigned char*)cur;
    return (Block*)(p - psz);
}

// Допоміжна функція для ініціалізації нового блоку
void block_initialize(Block* b, size_t size, bool busy, bool first, bool last) {
    if (b == NULL) return;

    b->size_flags = size;
    b->prev_size_flags = 0;

    block_set_flag_busy(b, busy);
    block_set_flag_first(b, first);
    block_set_flag_last(b, last);
}