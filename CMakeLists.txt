cmake_minimum_required(VERSION 3.17)
project(CoRoutines C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(.)
add_executable(coSort
        src/coSort.h
        src/coSort.c
        src/stack.c
        src/stack.h)
