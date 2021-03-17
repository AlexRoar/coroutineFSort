#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#define _XOPEN_SOURCE /* Mac compatibility. */

#include "coSort.h"
#include "stack.h"

#define STACK_SIZE (64 * 1024)
static CoPlanner planner;

void fileInputNumbers(ContextData *nowData, stack *input, int id);

void processFile(int id);

void printResults();

int main(int argc, const char **argv) {
    if (argc <= 1){
        printf("No latency in command line args\n");
        return (EXIT_FAILURE);
    }
    size_t latencyInp = 0;
    sscanf(argv[1], "%zu", &latencyInp);
    argv++;
    argc--;

    struct timeval latency = {latencyInp / 1000000, latencyInp % 1000000};
    printf("Desired latency: %ld.%06ld\n", (long int) latency.tv_sec, (long int) latency.tv_usec);
    CoPlanner_init(&planner, 10, latency);

    for (int i = 0; i < argc - 1; i++) {
        CoPlanner_add(&planner, STACK_SIZE, processFile);
        planner.data[i].userData.file = fopen(argv[i + 1], "rb+");
    }
    CoPlanner_fire(&planner);

    printResults();
    return 0;
}

void fileInputNumbers(ContextData *nowData, stack *input, int id) {
    fseek(nowData->userData.file, 0L, SEEK_END);
    CoPlanner_rollIfLatency(&planner);
    size_t size = ftell(nowData->userData.file);
    CoPlanner_rollIfLatency(&planner);
    rewind(nowData->userData.file);
    CoPlanner_rollIfLatency(&planner);

    char *fileRead = malloc(size + 1);
    if (!fileRead){
        printf("Error on malloc in coroutine %d", id);
        CoPlanner_finishCoroutine(&planner);
    }

    CoPlanner_rollIfLatency(&planner);
    fread(fileRead, 1, size, nowData->userData.file);
    CoPlanner_rollIfLatency(&planner);

    Stack_init(input, 1000);
    CoPlanner_rollIfLatency(&planner);

    char *lastPos = fileRead;
    int number = 0;
    CoPlanner_rollIfLatency(&planner);
    while (lastPos < fileRead + size) {
        CoPlanner_rollIfLatency(&planner);
        number *= 10;
        number += *lastPos - '0';
        lastPos++;
        if (*lastPos == ' ') {
            Stack_push(input, number);
            number = 0;
            while (*lastPos == ' ') {
                lastPos++;
                CoPlanner_rollIfLatency(&planner);
            }
        }
    }
    Stack_push(input, number);

    nowData->userData.count = Stack_size(input);
    nowData->userData.array = input->items;
    CoPlanner_rollIfLatency(&planner);
    free(fileRead);
    CoPlanner_rollIfLatency(&planner);
}

void processFile(int id) {
    stack input = {};
    ContextData *nowData = CoPlanner_dataNow(&planner);
    CoPlanner_rollIfLatency(&planner);
    fileInputNumbers(nowData, &input, id);
    CoPlanner_rollIfLatency(&planner);

    size_t n = nowData->userData.count;
    int *arr = nowData->userData.array;
    for (size_t i = 0; i < n - 1; i++) {
        CoPlanner_rollIfLatency(&planner);
        for (size_t j = 0; j < n - i - 1; j++) {
            CoPlanner_rollIfLatency(&planner);
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }

    CoPlanner_rollIfLatency(&planner);
    rewind(nowData->userData.file);
    CoPlanner_rollIfLatency(&planner);

    for (size_t i = 0; i < n; i++) {
        fprintf(nowData->userData.file, "%d ", arr[i]);
        CoPlanner_rollIfLatency(&planner);
    }

    free(input.items);
    CoPlanner_finishCoroutine(&planner);
}

void printResults() {
    for (int i = 0; i < planner.count; ++i) {
        printf("Switched (id %d) %zu times, total coroutine time: %ld.%06ld\n", i, planner.data[i].switched,
               (long int) planner.data[i].elapsed.tv_sec, (long int) planner.data[i].elapsed.tv_usec);
    }
    struct timeval tval_result;
    timersub(&planner.finish, &planner.entry, &tval_result);
    printf("Total time: %ld.%06ld, %zu switches\n", (long int) tval_result.tv_sec, (long int) tval_result.tv_usec,
           planner.switches);

    size_t latencyCalc = tval_result.tv_sec * 1000000 + tval_result.tv_usec;
    if (planner.switches)
        latencyCalc /= planner.switches;
    latencyCalc *= planner.count;
    printf("Real latency: %ld.%06ld\n", (long int) latencyCalc / 1000000, (long int) latencyCalc % 1000000);
}


void CoPlanner_init(CoPlanner *this, unsigned noCon, struct timeval latency) {
    this->capacity = noCon;
    this->latency = latency;
    this->latencyByN = latency;
    this->switches = 0;
    this->count = 0;
    this->now = -1;
    this->contexts = malloc(noCon * sizeof(ucontext_t));
    this->data = malloc(noCon * sizeof(ContextData));
    this->active = calloc(noCon, sizeof(char));
    if (!this->contexts || !this->data || !this->active)
        handleError("malloc");
}

void CoPlanner_add(CoPlanner *this, size_t stackSize, void *func) {
    if (this->count >= this->capacity)
        handleError("capacity overflow");
    ucontext_t *newCont = this->contexts + this->count;
    char *stack = allocateStack(stackSize);
    if (getcontext(newCont) == -1)
        handleError("getcontext");

    newCont->uc_stack.ss_sp = stack;
    newCont->uc_stack.ss_size = stackSize;
    newCont->uc_link = &this->main;

    makecontext(newCont, func, 1, this->count);
    this->count++;
}

bool CoPlanner_isActive(CoPlanner *this) {
    for (unsigned i = 0; i < this->count; i++)
        if (this->active[i])
            return true;
    return false;
}

void CoPlanner_fire(CoPlanner *this) {
    if (this->count == 0)
        return;

    struct timeval zeroed = {0, 0};
    for (unsigned i = 0; i < this->count; i++) {
        this->active[i] = true;
        this->data[i].switched = 0;
        this->data[i].elapsed = zeroed;
    }

    CoPlanner_setLatencyN(this);

    this->switches = 0;
    this->entry = getNowFastTime();

    while (CoPlanner_isActive(this)) {
        this->now = CoPlanner_nextAvailable(this);
        CoPlanner_swapToNowFrom(this, &this->main);
    }
    this->finish = getNowFastTime();
    this->now = -1;
}

bool CoPlanner_roll(CoPlanner *this) {
    CoPlanner_addCoElapsed(this);
    ucontext_t *enterC = &this->contexts[this->now];
    this->now = CoPlanner_nextAvailable(this);
    if (this->now >= this->count)
        return false;
    CoPlanner_swapToNowFrom(this, enterC);
    return true;
}

void CoPlanner_swapToNowFrom(CoPlanner *this, ucontext_t *enterC) {
    this->coEntered = getNowFastTime();
    this->data[this->now].switched++;
    this->switches++;
    swapcontext(enterC, &this->contexts[this->now]);
}

bool CoPlanner_rollIfLatency(CoPlanner *this) {
    struct timeval elapsed = CoPlanner_elapsed(this);
    if (elapsed.tv_sec > this->latencyByN.tv_sec)
        return CoPlanner_roll(this);
    else if (elapsed.tv_sec < this->latencyByN.tv_sec)
        return false;
    else if (elapsed.tv_usec >= this->latencyByN.tv_usec)
        return CoPlanner_roll(this);
    return false;
}

unsigned CoPlanner_nextAvailable(CoPlanner *this) {
    if (this->now >= this->count)
        return 0;
    unsigned next = this->now + 1;
    next %= (unsigned) (this->count);
    for (unsigned i = 0; i < this->count && !this->active[next]; i++) {
        next++;
        next %= this->count;
    }
    if (!this->active[next])
        return -1;
    return next;
}

ContextData *CoPlanner_dataNow(CoPlanner *this) {
    if (this->now >= this->count)
        return NULL;
    return this->data + this->now;
}

ContextData *CoPlanner_dataIth(CoPlanner *this, unsigned i) {
    if (i >= this->count)
        return NULL;
    return this->data + i;
}

void CoPlanner_finishCoroutine(CoPlanner *this) {
    this->active[this->now] = false;
    CoPlanner_addCoElapsed(this);
}

struct timeval CoPlanner_elapsed(CoPlanner *this) {
    struct timeval tval_now, tval_result;
    tval_now = getNowFastTime();
    timersub(&tval_now, &this->coEntered, &tval_result);
    return tval_result;
}

void CoPlanner_setLatencyN(CoPlanner *this) {
    this->latencyByN = this->latency;
    size_t microSum = (this->latency.tv_sec * 1000000) + this->latency.tv_usec;
    microSum /= this->count;

    this->latencyByN.tv_sec = microSum / 1000000;
    this->latencyByN.tv_usec = microSum % 1000000;
}

void CoPlanner_addCoElapsed(CoPlanner *this) {
    struct timeval elapsedNew = CoPlanner_elapsed(this);
    size_t total = this->data[this->now].elapsed.tv_sec * 1000000 + this->data[this->now].elapsed.tv_usec;
    total += elapsedNew.tv_sec * 1000000 + elapsedNew.tv_usec;
    this->data[this->now].elapsed.tv_sec = total / 1000000;
    this->data[this->now].elapsed.tv_usec = total % 1000000;
}

struct timeval getNowFastTime() {
    struct timeval out = {};
    gettimeofday(&out, NULL); // Эта строчка занимает 55% времени
    return out;
}

static void *allocateStack(size_t size) {
    return mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC,
                MAP_ANON | MAP_PRIVATE, -1, 0);
}

#pragma clang diagnostic pop