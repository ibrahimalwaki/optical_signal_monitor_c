#include "queue.h"

#include <stdlib.h>
#include <string.h>

int queue_init(BlockQueue *q, size_t capacity, size_t block_size)
{
    if (!q || capacity == 0 || block_size == 0) return -1;

    q->capacity = capacity;
    q->block_size = block_size;
    q->head = 0;
    q->tail = 0;
    q->count = 0;

    q->data = (float *)malloc(sizeof(float) * capacity * block_size);
    if (!q->data) return -2;

    if (pthread_mutex_init(&q->mtx, NULL) != 0) return -3;
    if (pthread_cond_init(&q->not_empty, NULL) != 0) return -4;
    if (pthread_cond_init(&q->not_full, NULL) != 0) return -5;

    return 0;
}

void queue_destroy(BlockQueue *q)
{
    if (!q) return;

    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);
    pthread_mutex_destroy(&q->mtx);

    free(q->data);
    q->data = NULL;
}

static float *slot_ptr(BlockQueue *q, size_t index)
{
    return q->data + (index * q->block_size);
}

void queue_push(BlockQueue *q, const float *block_in)
{
    pthread_mutex_lock(&q->mtx);

    while (q->count == q->capacity) {
        pthread_cond_wait(&q->not_full, &q->mtx);
    }

    memcpy(slot_ptr(q, q->tail), block_in, sizeof(float) * q->block_size);

    q->tail = (q->tail + 1) % q->capacity;
    q->count++;

    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->mtx);
}

void queue_pop(BlockQueue *q, float *block_out)
{
    pthread_mutex_lock(&q->mtx);

    while (q->count == 0) {
        pthread_cond_wait(&q->not_empty, &q->mtx);
    }

    memcpy(block_out, slot_ptr(q, q->head), sizeof(float) * q->block_size);

    q->head = (q->head + 1) % q->capacity;
    q->count--;

    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->mtx);
}
