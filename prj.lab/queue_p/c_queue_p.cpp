#include <queue_p/c_queue_p.h>
#include <queue_p/queue_p.hpp>

#include <map>
#include <utility>

class QueueManager {
public:
    QueueManager(const QueueManager&) = delete;
    QueueManager(QueueManager&&) = delete;
    QueueManager& operator=(const QueueManager&) = delete;
    QueueManager& operator=(QueueManager&&) = delete;

    static QueueManager& instance() {
        static QueueManager queueManager{};
        return queueManager;
    }

    ErrorCode createQueue(QueueHandler* const queueHandler) {
        const QueueHandler newHandler{ ++last_ };
        try {
            queues_.emplace(std::make_pair(newHandler, QueueP{}));
        } catch (...) {
            return ErrorCode::kHandleError;
        }
        *queueHandler = newHandler;
        return ErrorCode::kGood;
    }

    ErrorCode cloneQueue(const QueueHandler src, QueueHandler* const dest) {
        const auto pQueue = queues_.find(src);
        if (pQueue == queues_.end()) {
            return ErrorCode::kHandleError;
        }
        const QueueHandler newHandler{ ++last_ };
        try {
            queues_.emplace(std::make_pair(newHandler, QueueP{ pQueue->second }));
        } catch (...) {
            return ErrorCode::kHandleError;
        }
        *dest = newHandler;
        return ErrorCode::kGood;
    }

    ErrorCode destroyQueue(QueueHandler* const queueHandler) {
        const auto pQueue = queues_.find(*queueHandler);
        if (pQueue == queues_.end()) {
            return ErrorCode::kHandleError;
        }
        queues_.erase(pQueue);
        *queueHandler = 0;
        return ErrorCode::kGood;
    }

    ErrorCode isEmpty(const QueueHandler queueHandler, int32_t* const result) {
        const auto pQueue = queues_.find(queueHandler);
        if (pQueue == queues_.end()) {
            return ErrorCode::kHandleError;
        }
        bool empty{ pQueue->second.isEmpty() };
        *result = empty ? 1 : 0;
        return ErrorCode::kGood;
    }

    ErrorCode top(const QueueHandler queueHandler, int32_t* const result) const {
        const auto pQueue = queues_.find(queueHandler);
        if (pQueue == queues_.end()) {
            return ErrorCode::kHandleError;
        }
        if (pQueue->second.isEmpty()) {
            return ErrorCode::kOutOfRange;
        }
        *result = pQueue->second.top();
        return ErrorCode::kGood;
    }

    ErrorCode pop(const QueueHandler queueHandler) {
        const auto pQueue = queues_.find(queueHandler);
        if (pQueue == queues_.end()) {
            return ErrorCode::kHandleError;
        }
        if (pQueue->second.isEmpty()) {
            return ErrorCode::kOutOfRange;
        }
        pQueue->second.pop();
        return ErrorCode::kGood;
    }

    ErrorCode push(const QueueHandler queueHandler, const int32_t value) {
        const auto pQueue = queues_.find(queueHandler);
        if (pQueue == queues_.end()) {
            return ErrorCode::kHandleError;
        }
        try {
            pQueue->second.push(value);
        } catch (...) {
            return ErrorCode::kHandleError;
        }
        return ErrorCode::kGood;
    }

    static const char* whatIs(const ErrorCode errorCode) {
        static const char* kGood{ "kGood" };
        static const char* kHandleError{ "kHandleError" };
        static const char* kOutOfRange{ "kOutOfRange" };
        static const char* badErrorCode{ "badErrorCode" };
        switch (errorCode) {
        case ErrorCode::kGood:
            return kGood;
        case ErrorCode::kHandleError:
            return kHandleError;
        case ErrorCode::kOutOfRange:
            return kOutOfRange;
        default:
            return badErrorCode;
        }
    }

    int32_t queueCnt() const {
        return static_cast<int32_t>(queues_.size());
    }

private:
    QueueManager() = default;
    ~QueueManager() = default;

    QueueHandler last_{};
    std::map<QueueHandler, QueueP> queues_;
};

extern "C" {

ErrorCode CreateQueue(QueueHandler* const queueHandler) {
    return QueueManager::instance().createQueue(queueHandler);
}

ErrorCode CloneQueue(const QueueHandler src, QueueHandler* const dest) {
    return QueueManager::instance().cloneQueue(src, dest);
}

ErrorCode DestroyQueue(QueueHandler* const queueHandler) {
    return QueueManager::instance().destroyQueue(queueHandler);
}

ErrorCode IsEmpty(const QueueHandler queueHandler, int32_t* const result) {
    return QueueManager::instance().isEmpty(queueHandler, result);
}

ErrorCode Top(const QueueHandler queueHandler, int32_t* const result) {
    return QueueManager::instance().top(queueHandler, result);
}

ErrorCode Pop(const QueueHandler queueHandler) {
    return QueueManager::instance().pop(queueHandler);
}

ErrorCode Push(const QueueHandler queueHandler, const int32_t value) {
    return QueueManager::instance().push(queueHandler, value);
}

const char* WhatIs(const ErrorCode errorCode) {
    return QueueManager::whatIs(errorCode);
}

int32_t QueueCnt() {
    return QueueManager::instance().queueCnt();
}

}
