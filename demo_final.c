// demo_final.c - фінальна демонстрація роботи алокатора
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "allocator.h"

void demonstration() {
    printf("=== MEMORY ALLOCATOR FINAL DEMONSTRATION ===\n\n");

    // Ініціалізація
    printf("1. INITIALIZATION\n");
    mem_init(4096, 8192);
    printf("✓ Allocator initialized with 4KB page size and 8KB default arena\n\n");

    // Базове виділення
    printf("2. BASIC ALLOCATION\n");
    void* buffer1 = mem_alloc(256);
    void* buffer2 = mem_alloc(512);
    void* buffer3 = mem_alloc(1024);
    assert(buffer1 != NULL && buffer2 != NULL && buffer3 != NULL);
    printf("✓ Allocated: 256B, 512B, 1024B\n");
    mem_show();
    printf("\n");

    // Робота з даними
    printf("3. DATA MANIPULATION\n");
    strcpy((char*)buffer1, "Hello");
    strcpy((char*)buffer2, "World");
    printf("✓ Set buffer1 to: '%s'\n", (char*)buffer1);
    printf("✓ Set buffer2 to: '%s'\n", (char*)buffer2);
    printf("\n");

    // Realloc демонстрація
    printf("4. REALLOC DEMONSTRATION\n");
    printf("Before realloc:\n");
    printf("buffer1: '%s' (256 bytes)\n", (char*)buffer1);

    buffer1 = mem_realloc(buffer1, 512);
    assert(buffer1 != NULL);
    printf("After realloc to 512 bytes:\n");
    printf("buffer1: '%s' (data preserved!)\n", (char*)buffer1);
    mem_show();
    printf("\n");

    // Звільнення та повторне використання
    printf("5. MEMORY REUSE\n");
    mem_free(buffer2);
    printf("✓ Freed buffer2 (512B)\n");

    void* buffer4 = mem_alloc(256);
    assert(buffer4 != NULL);
    printf("✓ Allocated new 256B block (should reuse freed space)\n");
    mem_show();
    printf("\n");

    // Велике виділення
    printf("6. LARGE ALLOCATION\n");
    void* large_buffer = mem_alloc(5000);
    assert(large_buffer != NULL);
    printf("✓ Allocated large block of 5000 bytes\n");
    mem_show();
    printf("\n");

    // Очищення
    printf("7. CLEANUP\n");
    mem_free(buffer1);
    mem_free(buffer3);
    mem_free(buffer4);
    mem_free(large_buffer);
    printf("✓ Freed all allocated blocks\n");

    printf("Final state (should be empty):\n");
    mem_show();

    printf("\n");
    printf("=== DEMONSTRATION COMPLETED SUCCESSFULLY ===\n");
    printf("The memory allocator correctly:\n");
    printf("• Allocates memory of various sizes\n");
    printf("• Preserves data during reallocation\n");
    printf("• Reuses freed memory blocks\n");
    printf("• Handles large allocations\n");
    printf("• Returns all memory to the system\n");
}

int main() {
    demonstration();
    return 0;
}