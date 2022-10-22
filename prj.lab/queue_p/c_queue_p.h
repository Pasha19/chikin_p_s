#ifndef C_QUEUE_P_H_
#define C_QUEUE_P_H_

#ifdef _WINDOWS
    #define DLL_CALL __cdecl
#else
    #define DLL_CALL
#endif // _WINDOWS

#ifdef C_QUEUE_P_EXPORTS
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT
#endif // C_QUEUE_P_EXPORTS

#ifdef __cplusplus

    #include <cstdint>

    using std::uint64_t;
    using std::int32_t;

    extern "C" {

#else

    #include <stdint.h>

#endif // __cplusplus

typedef enum {
    kGood = 0,
    kHandleError,
    kOutOfRange
} ErrorCode;

typedef uint64_t QueueHandler;

DLL_EXPORT ErrorCode DLL_CALL CreateQueue(QueueHandler* queueHandler);

DLL_EXPORT ErrorCode DLL_CALL CloneQueue(QueueHandler src, QueueHandler* dest);

DLL_EXPORT ErrorCode DLL_CALL DestroyQueue(QueueHandler* queueHandler);

DLL_EXPORT ErrorCode DLL_CALL IsEmpty(QueueHandler queueHandler, int32_t* result);

DLL_EXPORT ErrorCode DLL_CALL Top(QueueHandler queueHandler, int32_t* result);

DLL_EXPORT ErrorCode DLL_CALL Pop(QueueHandler queueHandler);

DLL_EXPORT ErrorCode DLL_CALL Push(QueueHandler queueHandler, int32_t value);

DLL_EXPORT const char* DLL_CALL WhatIs(ErrorCode errorCode);

DLL_EXPORT int32_t DLL_CALL QueueCnt();

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // C_QUEUE_P_H_
