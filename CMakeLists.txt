cmake_minimum_required(VERSION 3.17)
project(CoRoutines C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
add_executable(CoSort
        coSort.h
        coSort.c
        stack.c
        stack.h)
