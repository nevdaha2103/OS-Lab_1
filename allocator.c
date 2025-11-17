#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "allocator.h"
#include "block.h"
#include "tree.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#endif

// Глобальні змінні
size_t page_size = 4096;
size_t default_arena_size = 4 * 4096;

// Структури
typedef struct Arena {
    size_t size;
    struct Arena* next;
    int is_large;
} Arena;

// Статичні змінні
static Arena* arena_list = NULL;
static struct Node* free_tree = NULL;

// Макрос для вирівнювання
#define ALIGN(size) align_up(size, sizeof(long double))

// Системні функції
#ifdef _WIN32
static void* sys_alloc(size_t size) {
    return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

static void sys_free(void* ptr, size_t size) {
    (void)size;
    VirtualFree(ptr, 0, MEM_RELEASE);
}

static size_t get_page_size() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwPageSize;
}
#else
static void* sys_alloc(size_t size) {
    void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    return (ptr == MAP_FAILED) ? NULL : ptr;
}

static void sys_free(void* ptr, size_t size) {
    munmap(ptr, size);
}

static size_t get_page_size() {
    return sysconf(_SC_PAGESIZE);
}
#endif

// Допоміжні функції
static Block* get_first_block(Arena* arena) {
    return (Block*)((char*)arena + sizeof(Arena));
}

static Arena* find_arena_for_block(Block* block) {
    Arena* arena = arena_list;
    while (arena != NULL) {
        char* arena_start = (char*)arena;
        char* arena_end = arena_start + arena->size;
        if ((char*)block >= arena_start && (char*)block < arena_end) {
            return arena;
        }
        arena = arena->next;
    }
    return NULL;
}

static void add_to_free_tree(size_t size, Block* block) {
    if (block == NULL || size == 0) return;
    free_tree = node_insert(free_tree, size, block);
}

static void remove_from_free_tree(size_t size) {
    free_tree = node_remove(free_tree, size);
}

static Block* find_free_block(size_t size) {
    if (free_tree == NULL) return NULL;

    struct Node* current = free_tree;
    struct Node* best = NULL;

    while (current != NULL) {
        if (current->key >= size) {
            if (best == NULL || current->key < best->key) {
                best = current;
            }
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return (best != NULL) ? (Block*)best->data : NULL;
}

// Основні функції алокатора
void* mem_alloc(size_t size) {
    if (size == 0) return NULL;

    size_t total_size = ALIGN(size + block_header_size());
    if (total_size < block_header_size() * 2) {
        total_size = block_header_size() * 2;
    }

    Block* block = find_free_block(total_size);

    if (block != NULL) {
        remove_from_free_tree(block_get_size(block));

        size_t block_size = block_get_size(block);

        if (block_size >= total_size + block_header_size() + 16) {
            size_t remaining_size = block_size - total_size;

            Block* new_block = (Block*)((char*)block + total_size);
            block_initialize(new_block, remaining_size, false, false, block_get_flag_last(block));
            block_set_size_prev(new_block, total_size);

            block_set_size(block, total_size);
            block_set_flag_last(block, false);

            add_to_free_tree(remaining_size, new_block);
        }

        block_set_flag_busy(block, true);
        return block_payload(block);

    } else {
        size_t arena_size = (total_size > default_arena_size) ?
                           ALIGN(total_size + sizeof(Arena)) : default_arena_size;

        Arena* arena = (Arena*)sys_alloc(arena_size);
        if (arena == NULL) return NULL;

        arena->size = arena_size;
        arena->next = arena_list;
        arena->is_large = (total_size > default_arena_size);
        arena_list = arena;

        block = get_first_block(arena);
        size_t block_size = arena_size - sizeof(Arena);
        block_initialize(block, block_size, true, true, true);

        if (!arena->is_large && block_size >= total_size + block_header_size() + 16) {
            size_t remaining_size = block_size - total_size;

            Block* new_block = (Block*)((char*)block + total_size);
            block_initialize(new_block, remaining_size, false, false, true);
            block_set_size_prev(new_block, total_size);

            block_set_size(block, total_size);
            block_set_flag_last(block, false);

            add_to_free_tree(remaining_size, new_block);
        }

        return block_payload(block);
    }
}

void mem_free(void* ptr) {
    if (ptr == NULL) return;

    Block* block = (Block*)((char*)ptr - block_header_size());
    Arena* arena = find_arena_for_block(block);

    if (arena == NULL) return;

    if (arena->is_large) {
        Arena* prev = NULL;
        Arena* curr = arena_list;
        while (curr != NULL && curr != arena) {
            prev = curr;
            curr = curr->next;
        }

        if (curr == arena) {
            if (prev == NULL) {
                arena_list = arena->next;
            } else {
                prev->next = arena->next;
            }
            sys_free(arena, arena->size);
        }
    } else {
        size_t block_size = block_get_size(block);
        if (block_size > 0) {
            block_set_flag_busy(block, false);
            add_to_free_tree(block_size, block);
        }
    }
}

void* mem_realloc(void* ptr, size_t size) {
    if (ptr == NULL) return mem_alloc(size);
    if (size == 0) {
        mem_free(ptr);
        return NULL;
    }

    Block* block = (Block*)((char*)ptr - block_header_size());
    size_t old_data_size = block_get_size(block) - block_header_size();

    if (size <= old_data_size) {
        return ptr;
    }

    void* new_ptr = mem_alloc(size);
    if (new_ptr != NULL) {
        memcpy(new_ptr, ptr, old_data_size);
        mem_free(ptr);
        return new_ptr;
    }

    return NULL;
}

void mem_show(void) {
    printf("=== Memory Allocator State ===\n");
    printf("Page size: %lu, Default arena size: %lu\n",
           (unsigned long)page_size, (unsigned long)default_arena_size);

    Arena* arena = arena_list;
    int arena_count = 0;
    printf("Arenas:\n");
    while (arena != NULL) {
        printf("  Arena %d: %p, size: %lu, %s\n",
               arena_count++, (void*)arena, (unsigned long)arena->size,
               arena->is_large ? "large" : "normal");
        arena = arena->next;
    }

    printf("Free blocks in tree:\n");
    if (free_tree == NULL) {
        printf("  (empty)\n");
    } else {
        node_show(free_tree);
    }

    arena = arena_list;
    int block_count = 0;
    printf("All blocks:\n");
    while (arena != NULL) {
        Block* block = get_first_block(arena);
        size_t arena_data_size = arena->size - sizeof(Arena);

        while (block != NULL && block_count < 50) {
            printf("  Block %d: %p, size: %lu, busy: %d, first: %d, last: %d\n",
                   block_count++, (void*)block,
                   (unsigned long)block_get_size(block),
                   block_get_flag_busy(block),
                   block_get_flag_first(block),
                   block_get_flag_last(block));

            Block* next = block_next(block, block, arena_data_size);
            if (next == NULL || next == block || (char*)next >= (char*)arena + arena->size) {
                break;
            }
            block = next;
        }
        arena = arena->next;
    }
    printf("=== End of State ===\n");
}

void mem_init(size_t custom_page_size, size_t custom_arena_size) {
    if (custom_page_size > 0) {
        page_size = custom_page_size;
    } else {
        page_size = get_page_size();
    }

    if (custom_arena_size > 0) {
        default_arena_size = custom_arena_size;
    } else {
        default_arena_size = 4 * page_size;
    }

    arena_list = NULL;
    free_tree = NULL;
}