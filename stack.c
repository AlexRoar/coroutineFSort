//
// Created by Александр Дремов on 17.03.2021.
//

#include "stack.h"


stack* newStack(int capacity) {
    stack *pt = (stack*)malloc(sizeof(stack));
    Stack_init(pt, capacity);
    return pt;
}

void Stack_init(stack *pt, int capacity){
    pt->maxsize = capacity;
    pt->top = -1;
    pt->items = (int*)malloc(sizeof(int) * capacity);
}

size_t Stack_size(stack *pt) {
    return pt->top + 1;
}

int Stack_isEmpty(stack *pt) {
    return pt->top == -1;
}

int Stack_isFull(stack *pt) {
    return pt->top == pt->maxsize - 1;
}

void Stack_expand(stack *pt){
    pt->items = (int*)realloc(pt->items, sizeof(int) * pt->maxsize * 2);
    pt->maxsize *= 2;
}

void Stack_push(stack *pt, int x)
{
    if (Stack_isFull(pt)) {
        Stack_expand(pt);
    }
    pt->items[++pt->top] = x;
}

// Utility function to return the top element of the stack
int Stack_peek(stack *pt)
{
    if (!Stack_isEmpty(pt)) {
        return pt->items[pt->top];
    } else {
        exit(EXIT_FAILURE);
    }
}

int Stack_pop(stack *pt)
{
    if (Stack_isEmpty(pt)) {
        exit(EXIT_FAILURE);
    }

    return pt->items[pt->top--];
}
