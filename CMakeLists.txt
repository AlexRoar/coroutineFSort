cmake_minimum_required(VERSION 3.17)
project(CoRoutines C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(CoRoutinesJmp
        coro_jmp.h
        example_jmp.c)

add_executable(CoRoutinesSwap
        coro_jmp.h
        example_swap.c)

add_executable(CoSort
        my/coSort.h
        my/coSort.c my/stack.c my/stack.h)
