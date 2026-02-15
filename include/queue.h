#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include <pthread.h>

typedef struct {
    float *data;           // backing storage (capacity * block_size floats)
    size_t capacity;       // number of blocks
    size_t block_size;     // floats per block

    size_t head;           // pop index
    size_t tail;           // push index
    size_t count;          // blocks currently in queue

    pthread_mutex_t mtx;
    pthread_cond_t  not_empty;
    pthread_cond_t  not_full;
} BlockQueue;

// Returns 0 on success, non-zero on failure
int queue_init(BlockQueue *q, size_t capacity, size_t block_size);
void queue_destroy(BlockQueue *q);

// Push/pop one block (blocks are float[block_size])
// These BLOCK until they can proceed (classic producer/consumer).
void queue_push(BlockQueue *q, const float *block_in);
void queue_pop(BlockQueue *q, float *block_out);

#endif
