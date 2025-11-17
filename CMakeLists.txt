cmake_minimum_required(VERSION 3.20)
project(Lab1 C)

set(CMAKE_C_STANDARD 99)

# Основні файли проекту
set(SOURCES
        allocator.c
        main_test.c
        main_test.c
        tree.c
        main.c
        block.c
        simple_test.c
        kernel.c
        mem_alloc.c
        demo_final.c
)

# Головний виконуваний файл
add_executable(Lab1 ${SOURCES})

# Налаштування компіляції
target_compile_options(Lab1 PRIVATE -Wall -Wextra -g)