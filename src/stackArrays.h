//
// Created by Александр Дремов on 17.03.2021.
//

#include <stdlib.h>
#include <stdio.h>

#ifndef COROUTINES_STACKARRAYS_H
#define COROUTINES_STACKARRAYS_H

typedef struct {
    void * array;
    size_t count;
}Array;

typedef struct {
    size_t maxsize;    // define max capacity of the stack
    size_t top;
    Array *items;
} stackArray;

stackArray* newStackArray(int capacity);

void StackArray_init(stackArray *pt, int capacity);

size_t StackArray_size(stackArray *pt);

int StackArray_isEmpty(stackArray *pt);

int StackArray_isFull(stackArray *pt);

void StackArray_expand(stackArray *pt);

void StackArray_push(stackArray *pt, Array x);

Array StackArray_peek(stackArray *pt);

Array StackArray_pop(stackArray *pt);

#endif //COROUTINES_STACKARRAYS_H
