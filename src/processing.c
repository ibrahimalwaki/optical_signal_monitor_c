#include "processing.h"
#include <math.h>

void lp_init(LowPassFilter *f, float alpha) {
    if (!f) return;

    if (alpha < 0.0f) alpha = 0.0f;
    if (alpha > 1.0f) alpha = 1.0f;

    f->alpha = alpha;
    f->prev_y = 0.0f;
}

// y[n] = alpha*x[n] + (1-alpha)*y[n-1] This is a first-order IIR filter.
void lp_apply(LowPassFilter *f, const float *in, float *out, size_t n) {
    if (!f || !in || !out || n == 0) return;

    float a = f->alpha;
    float y = f->prev_y;

    for (size_t i = 0; i < n; i++) {
        y = a * in[i] + (1.0f - a) * y;
        out[i] = y;
    }

    f->prev_y = y;
}

Metrics compute_metrics(const float *x, size_t n) {
    Metrics m = {0};

    if (!x || n == 0)
        return m;

    float sum_sq = 0.0f;
    float peak = 0.0f;

    for (size_t i = 0; i < n; i++) {
        float v = x[i];

        sum_sq += v * v;

        float abs_v = fabsf(v);
        if (abs_v > peak)
            peak = abs_v;
    }

    m.rms = sqrtf(sum_sq / (float)n);
    m.peak = peak;

    return m;
}
