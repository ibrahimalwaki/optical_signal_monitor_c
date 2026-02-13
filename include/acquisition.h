#ifndef ACQUISITION_H
#define ACQUISITION_H

#include <stddef.h>

typedef struct {
    float freq_hz;
    float sample_rate_hz;
    float amplitude;
    float noise_std;   // standard deviation of gaussian-ish noise (approx)
} SignalConfig;

// Generates n samples into out[]. Keeps phase continuous across calls via *phase_rad.
void acquisition_generate_block(float *out, size_t n,
                                const SignalConfig *cfg,
                                float *phase_rad);

#endif
