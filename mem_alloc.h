#ifndef LAB1_MEM_ALLOC_H
#define LAB1_MEM_ALLOC_H

#endif //LAB1_MEM_ALLOC_H

#ifndef MEM_ALLOC_H
#define MEM_ALLOC_H

#include <stddef.h>
#include <stdint.h>

// API алокатора
void *mem_alloc(size_t size);
void mem_free(void *ptr);
void *mem_realloc(void *ptr, size_t size);
void mem_show(void);

// Системно-залежні функції
void *sys_alloc(size_t size);
void sys_free(void *ptr, size_t size);
void sys_advise(void *ptr, size_t size);

// Конфігурація
extern size_t page_size;
extern size_t default_arena_size;

#endif