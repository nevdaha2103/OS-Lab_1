#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

void* mem_alloc(size_t size);
void mem_free(void* ptr);
void* mem_realloc(void* ptr, size_t size);
void mem_show(void);
void mem_init(size_t custom_page_size, size_t custom_arena_size);

extern size_t page_size;
extern size_t default_arena_size;

#endif