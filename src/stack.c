// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "stack.h"

#define handleError(msg) \
   do { perror(msg); exit(EXIT_FAILURE); } while (0)

stack* newStack(int capacity) {
    stack *pt = (stack*)malloc(sizeof(stack));
    if (!pt){
        handleError("malloc");
    }
    Stack_init(pt, capacity);
    return pt;
}

void Stack_init(stack *pt, int capacity){
    pt->maxsize = capacity;
    pt->top = -1;
    pt->items = (int*)malloc(sizeof(int) * capacity);
    if (!pt->items){
        handleError("malloc");
    }
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
    int* new = (int*)realloc(pt->items, sizeof(int) * pt->maxsize * 2);
    if (!new){
        handleError("malloc");
    }
    pt->items = new;
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
