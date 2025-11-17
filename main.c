#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "allocator.h"

void test_basic_functionality() {
    printf("=== TEST 1: BASIC FUNCTIONALITY ===\n");
    
    void* ptr1 = mem_alloc(100);
    assert(ptr1 != NULL);
    printf("✓ Allocated 100 bytes at %p\n", ptr1);
    
    void* ptr2 = mem_alloc(200);
    assert(ptr2 != NULL);
    printf("✓ Allocated 200 bytes at %p\n", ptr2);
    
    void* ptr3 = mem_alloc(300);
    assert(ptr3 != NULL);
    printf("✓ Allocated 300 bytes at %p\n", ptr3);
    
    // Заповнення даними
    strcpy((char*)ptr1, "Hello");
    strcpy((char*)ptr2, "World");
    strcpy((char*)ptr3, "Memory Allocator");
    
    printf("✓ Data written successfully\n");
    
    mem_show();
    
    // Звільнення
    mem_free(ptr2);
    printf("✓ Freed 200-byte block\n");
    
    mem_show();
    
    // Realloc
    ptr1 = mem_realloc(ptr1, 150);
    assert(ptr1 != NULL);
    printf("✓ Reallocated 100 bytes to 150 bytes\n");
    printf("  Data preserved: '%s'\n", (char*)ptr1);
    
    mem_show();
    
    // Очищення
    mem_free(ptr1);
    mem_free(ptr3);
    printf("✓ Freed all remaining blocks\n");
    
    mem_show();
    printf("=== TEST 1 PASSED ===\n\n");
}

void test_edge_cases() {
    printf("=== TEST 2: EDGE CASES ===\n");
    
    // Нульовий розмір
    void* ptr = mem_alloc(0);
    assert(ptr == NULL);
    printf("✓ Zero-size allocation handled correctly\n");
    
    // NULL pointer free
    mem_free(NULL);
    printf("✓ NULL pointer free handled correctly\n");
    
    // Великий блок
    void* large = mem_alloc(5000);
    assert(large != NULL);
    printf("✓ Large allocation (5000 bytes) successful\n");
    
    // Багато маленьких блоків
    void* small_blocks[10];
    for (int i = 0; i < 10; i++) {
        small_blocks[i] = mem_alloc(32);
        assert(small_blocks[i] != NULL);
        sprintf((char*)small_blocks[i], "Block%d", i);
    }
    printf("✓ Multiple small allocations successful\n");
    
    // Перевірка даних
    for (int i = 0; i < 10; i++) {
        char expected[10];
        sprintf(expected, "Block%d", i);
        assert(strcmp((char*)small_blocks[i], expected) == 0);
    }
    printf("✓ Data integrity verified\n");
    
    // Звільнення в змішаному порядку
    for (int i = 0; i < 10; i += 2) {
        mem_free(small_blocks[i]);
    }
    printf("✓ Partial free successful\n");
    
    mem_show();
    
    // Очищення
    for (int i = 1; i < 10; i += 2) {
        mem_free(small_blocks[i]);
    }
    mem_free(large);
    
    printf("✓ All blocks freed\n");
    printf("=== TEST 2 PASSED ===\n\n");
}

void test_fragmentation() {
    printf("=== TEST 3: FRAGMENTATION HANDLING ===\n");
    
    void* blocks[5];
    
    // Виділяємо кілька блоків
    for (int i = 0; i < 5; i++) {
        blocks[i] = mem_alloc(100);
        assert(blocks[i] != NULL);
        sprintf((char*)blocks[i], "Data%d", i);
    }
    printf("✓ Allocated 5 blocks of 100 bytes\n");
    
    // Звільняємо блоки 1 та 3
    mem_free(blocks[1]);
    mem_free(blocks[3]);
    printf("✓ Freed blocks 1 and 3\n");
    
    // Виділяємо блок більшого розміру (має використати об'єднані вільні блоки)
    void* new_block = mem_alloc(180);
    assert(new_block != NULL);
    printf("✓ Allocated 180 bytes (should reuse freed space)\n");
    
    strcpy((char*)new_block, "Reused memory");
    printf("✓ New block data: '%s'\n", (char*)new_block);
    
    mem_show();
    
    // Очищення
    mem_free(blocks[0]);
    mem_free(blocks[2]);
    mem_free(blocks[4]);
    mem_free(new_block);
    
    printf("✓ All blocks freed\n");
    printf("=== TEST 3 PASSED ===\n\n");
}

void test_realloc_scenarios() {
    printf("=== TEST 4: REALLOC SCENARIOS ===\n");
    
    void* ptr = mem_alloc(100);
    assert(ptr != NULL);
    
    // Заповнюємо даними
    strcpy((char*)ptr, "Original data");
    printf("✓ Original: '%s' (100 bytes)\n", (char*)ptr);
    
    // Зменшення розміру (in-place)
    ptr = mem_realloc(ptr, 50);
    assert(ptr != NULL);
    printf("✓ After shrink to 50 bytes: '%s'\n", (char*)ptr);
    
    // Збільшення розміру (може бути in-place або з переміщенням)
    ptr = mem_realloc(ptr, 200);
    assert(ptr != NULL);
    printf("✓ After expand to 200 bytes: '%s'\n", (char*)ptr);
    
    // Значне збільшення
    ptr = mem_realloc(ptr, 1000);
    assert(ptr != NULL);
    printf("✓ After expand to 1000 bytes: '%s'\n", (char*)ptr);
    
    mem_show();
    
    mem_free(ptr);
    printf("✓ Block freed\n");
    printf("=== TEST 4 PASSED ===\n\n");
}

void comprehensive_demo() {
    printf("=== COMPREHENSIVE DEMONSTRATION ===\n\n");
    
    printf("1. INITIALIZATION\n");
    mem_init(4096, 8192);
    printf("✓ Allocator initialized with page size 4096, arena size 8192\n\n");
    
    printf("2. MEMORY ALLOCATION\n");
    void* arr[6];
    
    arr[0] = mem_alloc(128);
    arr[1] = mem_alloc(256);
    arr[2] = mem_alloc(512);
    arr[3] = mem_alloc(1024);
    arr[4] = mem_alloc(64);
    arr[5] = mem_alloc(2048);
    
    for (int i = 0; i < 6; i++) {
        assert(arr[i] != NULL);
        sprintf((char*)arr[i], "Block%d_data", i);
    }
    printf("✓ Allocated 6 blocks of different sizes\n");
    
    printf("3. DATA VERIFICATION\n");
    for (int i = 0; i < 6; i++) {
        char expected[20];
        sprintf(expected, "Block%d_data", i);
        assert(strcmp((char*)arr[i], expected) == 0);
    }
    printf("✓ All data correctly stored and retrieved\n\n");
    
    printf("4. MEMORY STATE AFTER ALLOCATION\n");
    mem_show();
    printf("\n");
    
    printf("5. MEMORY REUSE\n");
    mem_free(arr[1]);
    mem_free(arr[3]);
    printf("✓ Freed blocks 1 and 3\n");
    
    void* reused1 = mem_alloc(200);
    void* reused2 = mem_alloc(800);
    assert(reused1 != NULL && reused2 != NULL);
    printf("✓ Reallocated 200 and 800 bytes (should reuse freed space)\n\n");
    
    printf("6. MEMORY STATE AFTER REUSE\n");
    mem_show();
    printf("\n");
    
    printf("7. LARGE ALLOCATION\n");
    void* large = mem_alloc(5000);
    assert(large != NULL);
    strcpy((char*)large, "Large block data");
    printf("✓ Large block (5000 bytes) allocated: '%s'\n\n", (char*)large);
    
    printf("8. FINAL CLEANUP\n");
    mem_free(arr[0]);
    mem_free(arr[2]);
    mem_free(arr[4]);
    mem_free(arr[5]);
    mem_free(reused1);
    mem_free(reused2);
    mem_free(large);
    printf("✓ All memory freed\n\n");
    
    printf("9. FINAL STATE (should be empty)\n");
    mem_show();
    
    printf("\n=== DEMONSTRATION COMPLETED SUCCESSFULLY ===\n");
}

int main() {
    printf("🚀 MEMORY ALLOCATOR TEST SUITE\n");
    printf("===============================\n\n");
    
    // Ініціалізація
    mem_init(4096, 8192);

    // Запуск тестів
    test_basic_functionality();
    test_edge_cases();
    test_fragmentation();
    test_realloc_scenarios();
    
    // Комплексна демонстрація
    comprehensive_demo();
    
    printf("\n🎉 ALL TESTS PASSED! MEMORY ALLOCATOR IS WORKING CORRECTLY! 🎉\n");
    printf("=============================================================\n");
    
    return 0;
}