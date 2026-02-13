#ifndef PROCESSING_H
#define PROCESSING_H

#include <stddef.h>

typedef struct {
    float alpha;      // smoothing factor
    float prev_y;     // filter state (previous output)
} LowPassFilter;

typedef struct {
    float rms;
    float peak;
} Metrics;

void lp_init(LowPassFilter *f, float alpha);
void lp_apply(LowPassFilter *f, const float *in, float *out, size_t n);

Metrics compute_metrics(const float *x, size_t n);

#endif
