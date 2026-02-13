#include <stdio.h>
#include <stddef.h>

#include "acquisition.h"

#define N 256

static void print_block_stats(const float *x, size_t n) {
    float minv = x[0], maxv = x[0], sum = 0.0f;
    for (size_t i = 0; i < n; i++) {
        if (x[i] < minv) minv = x[i];
        if (x[i] > maxv) maxv = x[i];
        sum += x[i];
    }
    float avg = sum / (float)n;
    printf("block stats: min=%.4f max=%.4f avg=%.4f\n", minv, maxv, avg);
}

int main(void) {
    static float block[N];

    SignalConfig cfg = {
        .freq_hz = 1000.0f,
        .sample_rate_hz = 48000.0f,
        .amplitude = 1.0f,
        .noise_std = 0.05f
    };

    float phase = 0.0f;

    for (int i = 0; i < 5; i++) {
        acquisition_generate_block(block, N, &cfg, &phase);
        print_block_stats(block, N);
    }

    return 0;
}
