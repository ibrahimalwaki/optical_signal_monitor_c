#include <stdio.h>

#include "queue.h"

#define BLOCK_SIZE 8
#define CAPACITY   4

int main(void)
{
    BlockQueue q;
    if (queue_init(&q, CAPACITY, BLOCK_SIZE) != 0) {
        printf("queue_init failed\n");
        return 1;
    }

    float in[BLOCK_SIZE];
    float out[BLOCK_SIZE];

    for (int i = 0; i < BLOCK_SIZE; i++) in[i] = (float)(i + 1);

    queue_push(&q, in);
    queue_pop(&q, out);

    printf("queue demo output: ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%.0f ", out[i]);
    }
    printf("\n");

    queue_destroy(&q);
    return 0;
}
