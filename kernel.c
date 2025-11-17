#include "kernel.h"

#if defined(_WIN32) ||  defined(_WIN64)
#include <windows.h> // #include <memoryapi.h> // included in windows.h

void* kernel_mem_alloc(size_t size)
{
    return VirtualAlloc(NULL, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
}

void kernel_mem_free(void* ptr, size_t size)
{
    (void*)size;
    VirtualFree(ptr, 0, MEM_RELEASE);
}

#else
#include <sys/mman.h>
#if defined(MAP_ANONYMOUS) 
#define FLAG_ANON MAP_ANONYMOUS 
#elif defined(MAP_ANON) 
#define FLAG_ANON MAP_ANON 
#else 
#error "Do not know how to specify anonymous memory in mmap()"
#endif 

void* kernel_mem_alloc(size_t size)
{
    void* ptr;
    ptr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|FLAG_ANON, -1, 0);
    return ptr != MAP_FAILED ? ptr : NULL;
}

void kernel_mem_free(void* ptr, size_t size)
{
    if (munmap(ptr, size) < 0)
        failed();
}
#endif // (_WIN32) ||  defined(_WIN64)