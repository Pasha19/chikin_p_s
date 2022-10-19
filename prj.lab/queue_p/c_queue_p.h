#ifndef C_QUEUE_P_H_
#define C_QUEUE_P_H_

#ifdef __cplusplus

#include <cstdint>

using std::uint64_t;
using std::int32_t;

extern "C" {

#else

#include <stdint.h>

#endif

typedef enum {
    kGood = 0,
    kHandleError,
    kOutOfRange
} ErrorCode;

typedef uint64_t QueueHandler;

ErrorCode CreateQueue(QueueHandler* queueHandler);

ErrorCode CloneQueue(QueueHandler src, QueueHandler* dest);

ErrorCode DestroyQueue(QueueHandler* queueHandler);

ErrorCode IsEmpty(QueueHandler queueHandler, int32_t* result);

ErrorCode Top(QueueHandler queueHandler, int32_t* result);

ErrorCode Pop(QueueHandler queueHandler);

ErrorCode Push(QueueHandler queueHandler, int32_t value);

const char* WhatIs(ErrorCode errorCode);

int32_t QueueCnt();

#ifdef __cplusplus
}
#endif

#endif
