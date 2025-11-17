#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "allocator.h"

int main() {
    printf("Memory Allocator Demo\n");

    // Ініціалізація алокатора
    mem_init(4096, 16384);

    printf("\n=== Initial State ===\n");
    mem_show();

    printf("\n=== Allocating memory ===\n");
    void* ptr1 = mem_alloc(100);
    printf("Allocated 100 bytes at %p\n", ptr1);

    void* ptr2 = mem_alloc(200);
    printf("Allocated 200 bytes at %p\n", ptr2);

    void* ptr3 = mem_alloc(500);
    printf("Allocated 500 bytes at %p\n", ptr3);

    mem_show();

    printf("\n=== Freeing memory ===\n");
    mem_free(ptr2);
    printf("Freed 200-byte block\n");

    mem_show();

    printf("\n=== Reallocating memory ===\n");
    ptr1 = mem_realloc(ptr1, 300);
    printf("Reallocated 100 bytes to 300 bytes at %p\n", ptr1);

    mem_show();

    printf("\n=== Large allocation ===\n");
    void* large_ptr = mem_alloc(10000);
    printf("Allocated 10000 bytes at %p\n", large_ptr);

    mem_show();

    printf("\n=== Final cleanup ===\n");
    mem_free(ptr1);
    mem_free(ptr3);
    mem_free(large_ptr);

    mem_show();

    printf("Demo completed successfully!\n");
    return 0;
}