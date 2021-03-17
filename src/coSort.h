//
// Created by Александр Дремов on 17.03.2021.
//

#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/time.h>

#ifndef COROUTINES_COSORT_H
#define COROUTINES_COSORT_H

#define true 1
#define false 0
#define bool char

#define handleError(msg) \
   do { perror(msg); exit(EXIT_FAILURE); } while (0)


typedef struct {
    FILE *file;
    int *array;
    size_t count;
} ContextDataUser;


typedef struct {
    struct timeval elapsed;
    size_t switched;
    void* initialSp;
    ContextDataUser userData;
} ContextData;


typedef struct {
    unsigned capacity, count, now;

    ucontext_t *contexts;
    bool *active;
    ucontext_t main;

    ContextData *data;

    struct timeval coEntered, latency, latencyByN, entry, finish;
    size_t switches;
} CoPlanner;

void CoPlanner_init(CoPlanner *this, unsigned noCon, struct timeval latency);

void CoPlanner_destroy(CoPlanner *this);

void CoPlanner_add(CoPlanner *this, size_t stackSize, void *func);

bool CoPlanner_isActive(CoPlanner *this);

void CoPlanner_fire(CoPlanner *this);

bool CoPlanner_roll(CoPlanner *this);

bool CoPlanner_rollIfLatency(CoPlanner *this);

void CoPlanner_finishCoroutine(CoPlanner *this);

unsigned CoPlanner_nextAvailable(CoPlanner *this);

void CoPlanner_setLatencyN(CoPlanner *this);

void CoPlanner_addCoElapsed(CoPlanner *this);

void CoPlanner_swapToNowFrom(CoPlanner *this, ucontext_t *enterC);

ContextData *CoPlanner_dataNow(CoPlanner *this);

ContextData *CoPlanner_dataIth(CoPlanner *this, unsigned i);

struct timeval CoPlanner_elapsed(CoPlanner *this);

struct timeval getNowFastTime();

static void *allocateStack(size_t size);

#endif //COROUTINES_COSORT_H
