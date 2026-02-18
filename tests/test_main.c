#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "processing.h"
#include "fault.h"
#include "queue.h"

#define ASSERT_TRUE(cond) do { \
    if (!(cond)) { \
        printf("FAIL: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        return 1; \
    } \
} while (0)

#define ASSERT_NEAR(a,b,eps) do { \
    float _aa = (float)(a); \
    float _bb = (float)(b); \
    float _diff = fabsf(_aa - _bb); \
    if (_diff > (eps)) { \
        printf("FAIL: %s:%d: |%s - %s| = %.6f > %.6f\n", \
               __FILE__, __LINE__, #a, #b, _diff, (float)(eps)); \
        return 1; \
    } \
} while (0)

static int test_metrics_rms_peak(void)
{
    // Known signal: [1, -1, 1, -1]
    // peak = 1
    // rms = 1
    float x[4] = { 1.0f, -1.0f, 1.0f, -1.0f };
    Metrics m = compute_metrics(x, 4);
    ASSERT_NEAR(m.peak, 1.0f, 1e-6f);
    ASSERT_NEAR(m.rms,  1.0f, 1e-6f);
    return 0;
}

static int test_fault_detector(void)
{
    FaultDetector fd;
    fault_init(&fd, 0.20f, 1.40f, 0);

    Metrics m1 = { .rms = 0.10f, .peak = 1.0f };
    ASSERT_TRUE(fault_update(&fd, m1) == FAULT_DROPOUT);

    Metrics m2 = { .rms = 0.80f, .peak = 1.50f };
    ASSERT_TRUE(fault_update(&fd, m2) == FAULT_NOISE_SPIKE);

    Metrics m3 = { .rms = 0.80f, .peak = 1.00f };
    ASSERT_TRUE(fault_update(&fd, m3) == FAULT_NONE);

    return 0;
}

static int test_queue_fifo_single_thread(void)
{
    const size_t block_size = 4;
    BlockQueue q;
    ASSERT_TRUE(queue_init(&q, 2, block_size) == 0);

    float a[4] = {1,2,3,4};
    float b[4] = {5,6,7,8};
    float out[4] = {0};

    queue_push(&q, a);
    queue_push(&q, b);

    queue_pop(&q, out);
    ASSERT_NEAR(out[0], 1, 1e-6f);
    ASSERT_NEAR(out[3], 4, 1e-6f);

    queue_pop(&q, out);
    ASSERT_NEAR(out[0], 5, 1e-6f);
    ASSERT_NEAR(out[3], 8, 1e-6f);

    queue_destroy(&q);
    return 0;
}

int main(void)
{
    int rc = 0;

    rc = test_metrics_rms_peak();           if (rc) return rc;
    rc = test_fault_detector();             if (rc) return rc;
    rc = test_queue_fifo_single_thread();   if (rc) return rc;

    printf("ALL TESTS PASSED\n");
    return 0;
}
