#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "allocator.h"

void simple_test() {
    printf("=== SIMPLE TEST ===\n");
    
    mem_init(4096, 8192);
    
    // Просте виділення
    void* ptr1 = mem_alloc(100);
    assert(ptr1 != NULL);
    printf("Allocated 100 bytes at %p\n", ptr1);
    
    // Запис даних
    strcpy((char*)ptr1, "Hello World");
    printf("Data written: '%s'\n", (char*)ptr1);
    
    // Перевірка даних
    assert(strcmp((char*)ptr1, "Hello World") == 0);
    printf("Data verification: OK\n");
    
    // Ще одне виділення
    void* ptr2 = mem_alloc(50);
    assert(ptr2 != NULL);
    strcpy((char*)ptr2, "Test");
    printf("Allocated 50 bytes at %p: '%s'\n", ptr2, (char*)ptr2);
    
    mem_show();
    
    // Звільнення
    mem_free(ptr1);
    printf("Freed first block\n");
    
    mem_free(ptr2);
    printf("Freed second block\n");
    
    mem_show();
    printf("=== SIMPLE TEST PASSED ===\n");
}

int main() {
    simple_test();
    return 0;
}