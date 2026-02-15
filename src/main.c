#include <stdio.h>
#include <pthread.h>
#include <time.h>

#include "acquisition.h"
#include "processing.h"
#include "queue.h"

#define N 256
#define QUEUE_CAPACITY 8
#define PERIOD_MS 1000  // producer generates every 1 s

static void sleep_ms(long ms)
{
    struct timespec req;
    req.tv_sec = ms / 1000;
    req.tv_nsec = (ms % 1000) * 1000000L;
    nanosleep(&req, NULL);
}

typedef struct {
    BlockQueue *q;
    SignalConfig cfg;
    float phase;
    int running;
} ProducerCtx;

typedef struct {
    BlockQueue *q;
    LowPassFilter lp;
    int running;
} ConsumerCtx;

static void *producer_thread(void *arg)
{
    ProducerCtx *ctx = (ProducerCtx *)arg;
    float block[N];

    while (ctx->running) {
        acquisition_generate_block(block, N, &ctx->cfg, &ctx->phase);
        queue_push(ctx->q, block);
        sleep_ms(PERIOD_MS);
    }
    return NULL;
}

static void *consumer_thread(void *arg)
{
    ConsumerCtx *ctx = (ConsumerCtx *)arg;
    float block[N];
    float filtered[N];

    while (ctx->running) {
        queue_pop(ctx->q, block);

        Metrics raw = compute_metrics(block, N);

        lp_apply(&ctx->lp, block, filtered, N);
        Metrics fil = compute_metrics(filtered, N);

        printf("raw: rms=%.4f peak=%.4f | filtered: rms=%.4f peak=%.4f\n",
               raw.rms, raw.peak, fil.rms, fil.peak);
        fflush(stdout);
    }
    return NULL;
}

int main(void)
{
    BlockQueue q;
    if (queue_init(&q, QUEUE_CAPACITY, N) != 0) {
        printf("queue_init failed\n");
        return 1;
    }

    ProducerCtx prod = {
        .q = &q,
        .cfg = {
            .freq_hz = 1000.0f,
            .sample_rate_hz = 48000.0f,
            .amplitude = 1.0f,
            .noise_std = 0.10f,
            .dropout_rate = 0.02f,
            .spike_rate = 0.02f,
            .spike_noise_std = 0.60f,
        },
        .phase = 0.0f,
        .running = 1
    };

    ConsumerCtx cons = {
        .q = &q,
        .running = 1
    };
    lp_init(&cons.lp, 0.15f);

    pthread_t pt, ct;
    pthread_create(&pt, NULL, producer_thread, &prod);
    pthread_create(&ct, NULL, consumer_thread, &cons);

    printf("Running (2-thread pipeline). Press Ctrl+C to stop.\n");

    while (1) {
        sleep_ms(1000);
    }  
}
