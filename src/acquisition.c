#include "acquisition.h"

#include <math.h>
#include <stdint.h>

// Simple deterministic PRNG (fast, good enough for noise)
static uint32_t xorshift32(uint32_t *state) {
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

// Uniform in [-1, 1]
static float rand_uniform_signed(uint32_t *state) {
    // 24-bit mantissa style scaling
    uint32_t r = xorshift32(state) >> 8; // 24 bits
    float u = (float)r / (float)(0x00FFFFFFu); // [0,1]
    return 2.0f * u - 1.0f; // [-1,1]
}

// Approx gaussian noise by summing uniforms (central limit-ish)
static float approx_gaussian(uint32_t *state) {
    // Sum of 6 uniforms gives a decent bell shape
    float s = 0.0f;
    for (int i = 0; i < 6; i++) s += rand_uniform_signed(state);
    // Normalize roughly (not perfect, but fine for simulation)
    return s / 6.0f;
}

void acquisition_generate_block(float *out, size_t n,
                                const SignalConfig *cfg,
                                float *phase_rad)
{
    if (!out || !cfg || !phase_rad || n == 0) return;

    const float two_pi = 6.2831853071795864769f;
    const float phase_step = two_pi * (cfg->freq_hz / cfg->sample_rate_hz);

    // Seed from current phase (deterministic-ish across runs)
    uint32_t rng = (uint32_t)((*phase_rad) * 100000.0f) ^ 0xA5A5A5A5u;

    float phase = *phase_rad;

    for (size_t i = 0; i < n; i++) {
        float clean = cfg->amplitude * sinf(phase);

        float noise = 0.0f;
        if (cfg->noise_std > 0.0f) {
            noise = cfg->noise_std * approx_gaussian(&rng);
        }

        out[i] = clean + noise;

        phase += phase_step;
        if (phase >= two_pi) phase -= two_pi; // keep bounded
    }

    *phase_rad = phase;
}
