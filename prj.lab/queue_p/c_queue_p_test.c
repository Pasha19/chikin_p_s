#include <queue_p/c_queue_p.h>

#include <inttypes.h>
#include <stdio.h>

int main() {
    QueueHandler queue = 0;
    ErrorCode code = kGood;
    if ((code = CreateQueue(&queue)) != kGood) {
        fprintf(stderr, "Code: %d\nMsg: %s\n", code, WhatIs(code));
        return 1;
    }
    printf("Created queue: %" PRIu64 "\n", queue);
    const int array[] = { 1, 10, 2, 5, 6, 9 };
    const int size = sizeof(array) / sizeof(array[0]);
    for (int i = 0; i < size; ++i) {
        if ((code = Push(queue, array[i])) != kGood) {
            fprintf(stderr, "Code: %d\nMsg: %s\n", code, WhatIs(code));
            DestroyQueue(&queue);
            return 1;
        }
        printf("Inserted %d: %d\n", i + 1, array[i]);
    }
    while (1) {
        int empty = 0;
        if ((code = IsEmpty(queue, &empty)) != kGood) {
            fprintf(stderr, "Code: %d\nMsg: %s\n", code, WhatIs(code));
            DestroyQueue(&queue);
            return 1;
        }
        if (empty) {
            break;
        }
        int value = 0;
        if ((code = Top(queue, &value)) != kGood) {
            fprintf(stderr, "Code: %d\nMsg: %s\n", code, WhatIs(code));
            DestroyQueue(&queue);
            return 1;
        }
        printf("From queue: %d\n", value);
        if ((code = Pop(queue)) != kGood) {
            fprintf(stderr, "Code: %d\nMsg: %s\n", code, WhatIs(code));
            DestroyQueue(&queue);
            return 1;
        }
    }
    printf("Queues count: %d\n", QueueCnt());
    printf("Deleting queue: %" PRIu64 "\n", queue);
    if ((code = DestroyQueue(&queue)) != kGood) {
        fprintf(stderr, "Code: %d\nMsg: %s\n", code, WhatIs(code));
        return 1;
    }
    printf("Queues count: %d\n", QueueCnt());
    return 0;
}
