#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>

typedef struct Queue Queue;
typedef Queue* (*f_queue_new)();
typedef void (*f_queue_delete)(Queue* const);
typedef void (*f_queue_push)(Queue* const, const int);
typedef int (*f_queue_top)(const Queue* const);
typedef void (*f_queue_pop)(Queue* const);

int main() {
    void* qlib = dlopen("/mnt/c/study/prog/cmake-build-debug/prj.lab/clib/libqueue.so", RTLD_NOW);
    if (qlib == NULL) {
        fprintf(stderr, "Failed load library\n");
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }
    f_queue_new queue_new = (f_queue_new)(intptr_t) dlsym(qlib, "queue_new");
    if (queue_new == NULL) {
        fprintf(stderr, "Failed load queue_new\n");
        fprintf(stderr, "%s\n", dlerror());
        dlclose(qlib);
        return 1;
    }
    f_queue_delete queue_delete = (f_queue_delete)(intptr_t) dlsym(qlib, "queue_delete");
    if (queue_delete == NULL) {
        fprintf(stderr, "Failed load queue_delete\n");
        fprintf(stderr, "%s\n", dlerror());
        dlclose(qlib);
        return 1;
    }
    Queue* queue = queue_new();
    f_queue_push queue_push = (f_queue_push)(intptr_t) dlsym(qlib, "queue_push");
    if (queue_push == NULL) {
        queue_delete(queue);
        fprintf(stderr, "Failed load queue_push\n");
        fprintf(stderr, "%s\n", dlerror());
        dlclose(qlib);
        return 1;
    }
    queue_push(queue, 3);
    queue_push(queue, 10);
    queue_push(queue, 1);
    f_queue_top queue_top = (f_queue_top)(intptr_t) dlsym(qlib, "queue_top");
    if (queue_top == NULL) {
        queue_delete(queue);
        fprintf(stderr, "Failed load queue_top\n");
        fprintf(stderr, "%s\n", dlerror());
        dlclose(qlib);
        return 1;
    }
    f_queue_pop queue_pop = (f_queue_pop)(intptr_t) dlsym(qlib, "queue_pop");
    if (queue_pop == NULL) {
        queue_delete(queue);
        fprintf(stderr, "Failed load queue_pop\n");
        fprintf(stderr, "%s\n", dlerror());
        dlclose(qlib);
        return 1;
    }
    int a = queue_top(queue);
    queue_pop(queue);
    int b = queue_top(queue);
    queue_pop(queue);
    int c = queue_top(queue);
    printf("%d %d %d\n", a, b, c);
    queue_delete(queue);
    dlclose(qlib);
    return 0;
}
