#include <stdio.h>
#include "acquisition.h"
#include "processing.h"

#define N 256

int main(void)
{
    static float block[N];
    static float filtered[N];

    SignalConfig cfg = {
        .freq_hz = 1000.0f,
        .sample_rate_hz = 48000.0f,
        .amplitude = 1.0f,
        .noise_std = 0.1f
    };

    float phase = 0.0f;

    LowPassFilter lp;
    lp_init(&lp, 0.15f);

    for (int i = 0; i < 5; i++) {

        acquisition_generate_block(block, N, &cfg, &phase);

        Metrics raw = compute_metrics(block, N);

        lp_apply(&lp, block, filtered, N);
        Metrics fil = compute_metrics(filtered, N);

        printf("raw: rms=%.4f peak=%.4f | filtered: rms=%.4f peak=%.4f\n",
               raw.rms, raw.peak,
               fil.rms, fil.peak);
    }

    return 0;
}
