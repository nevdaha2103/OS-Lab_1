#pragma once
#include "config.h"

void* kernel_mem_alloc(size_t);
void kernel_mem_free(void*, size_t);