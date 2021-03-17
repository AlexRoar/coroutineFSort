//
// Created by Александр Дремов on 17.03.2021.
//

#ifndef COROUTINES_STACK_H
#define COROUTINES_STACK_H
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    size_t maxsize;    // define max capacity of the stack
    size_t top;
    int *items;
} stack;

stack* newStack(int capacity);

void Stack_init(stack *pt, int capacity);

size_t Stack_size(stack *pt);

int Stack_isEmpty(stack *pt);

int Stack_isFull(stack *pt);

void Stack_expand(stack *pt);

void Stack_push(stack *pt, int x);

int Stack_peek(stack *pt);

int Stack_pop(stack *pt);

#endif //COROUTINES_STACK_H
