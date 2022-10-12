#ifndef CLIB_H_

typedef struct Queue Queue;
typedef int el_t;
typedef int s_t;

#ifdef __cplusplus
extern "C" {
#endif

Queue* queue_new();
Queue* queue_copy(const Queue*);
Queue* queue_move(Queue*);

void queue_delete(Queue*);

void queue_assign(Queue*, const Queue*);
void queue_move_assign(Queue*, Queue*);

s_t queue_size(const Queue*);
el_t queue_top(const Queue*);

void queue_pop(Queue*);
void queue_push(Queue*, el_t);

#ifdef __cplusplus
}
#endif

#endif
