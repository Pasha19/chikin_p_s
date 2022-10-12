#include <clib/queue.h>
#include <queuep/queuep.h>

using QueueType = QueueP;

extern "C" {

    Queue* queue_new() {
        return reinterpret_cast<Queue*>(new QueueType());
    }

    Queue* queue_copy(const Queue* const queue) {
        return reinterpret_cast<Queue*>(
            new QueueType(
                *reinterpret_cast<QueueType*>(const_cast<Queue*>(queue))
            )
        );
    }

    Queue* queue_move(Queue* const queue) {
        return reinterpret_cast<Queue*>(
            new QueueType(
                std::move(*reinterpret_cast<QueueType*>(queue))
            )
        );
    }

    void queue_delete(Queue* const queue) {
        delete reinterpret_cast<QueueType*>(queue);
    }

    void queue_assign(Queue* const lhs, const Queue* const rhs) {
        reinterpret_cast<QueueType*>(lhs)->operator=(
            *reinterpret_cast<QueueType*>(const_cast<Queue*>(rhs))
        );
    }

    void queue_move_assign(Queue* const lhs, Queue* const rhs) {
        reinterpret_cast<QueueType*>(lhs)->operator=(std::move(*reinterpret_cast<QueueType*>(rhs)));
    }

    s_t queue_size(const Queue* const queue) {
        return reinterpret_cast<QueueP*>(const_cast<Queue*>(queue))->size();
    }

    el_t queue_top(const Queue* const queue) {
        return reinterpret_cast<QueueP*>(const_cast<Queue*>(queue))->top();
    }

    void queue_pop(Queue* const queue) {
        reinterpret_cast<QueueP*>(queue)->pop();
    }

    void queue_push(Queue* const queue, const el_t value) {
        reinterpret_cast<QueueP*>(queue)->push(value);
    }
}
