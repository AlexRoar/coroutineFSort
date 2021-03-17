// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "stackArrays.h"

#define handleError(msg) \
   do { perror(msg); exit(EXIT_FAILURE); } while (0)

stackArray* newStackArray(int capacity) {
    stackArray *pt = (stackArray*)malloc(sizeof(stackArray));
    if (!pt){
        handleError("malloc");
    }
    StackArray_init(pt, capacity);
    return pt;
}

void StackArray_init(stackArray *pt, int capacity){
    pt->maxsize = capacity;
    pt->top = -1;
    pt->items = (Array*)malloc(sizeof(Array) * capacity);
    if (!pt->items){
        handleError("malloc");
    }
}

size_t StackArray_size(stackArray *pt) {
    return pt->top + 1;
}

int StackArray_isEmpty(stackArray *pt) {
    return pt->top == -1;
}

int StackArray_isFull(stackArray *pt) {
    return pt->top == pt->maxsize - 1;
}

void StackArray_expand(stackArray *pt){
    Array * new = (Array*)realloc(pt->items, sizeof(Array) * pt->maxsize * 2);
    if (!new){
        handleError("malloc");
    }
    pt->items = new;
    pt->maxsize *= 2;
}

void StackArray_push(stackArray *pt, Array x)
{
    if (StackArray_isFull(pt)) {
        StackArray_expand(pt);
    }
    pt->items[++pt->top] = x;
}

// Utility function to return the top element of the stack
Array StackArray_peek(stackArray *pt)
{
    if (!StackArray_isEmpty(pt)) {
        return pt->items[pt->top];
    } else {
        exit(EXIT_FAILURE);
    }
}

Array StackArray_pop(stackArray *pt)
{
    if (StackArray_isEmpty(pt)) {
        exit(EXIT_FAILURE);
    }

    return pt->items[pt->top--];
}
