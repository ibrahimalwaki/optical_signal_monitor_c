#include "fault.h"

void fault_init(FaultDetector *fd,
                float dropout_rms_threshold,
                float spike_peak_threshold,
                int cooldown_blocks)
{
    if (!fd) return;

    fd->dropout_rms_threshold = dropout_rms_threshold;
    fd->spike_peak_threshold = spike_peak_threshold;
    fd->cooldown_blocks = (cooldown_blocks < 0) ? 0 : cooldown_blocks;
    fd->cooldown_left = 0;
}

FaultType fault_update(FaultDetector *fd, Metrics raw)
{
    if (!fd) return FAULT_NONE;

    // Cooldown to avoid alert spam
    if (fd->cooldown_left > 0) {
        fd->cooldown_left--;
        return FAULT_NONE;
    }

    if (raw.rms < fd->dropout_rms_threshold) {
        fd->cooldown_left = fd->cooldown_blocks;
        return FAULT_DROPOUT;
    }

    if (raw.peak > fd->spike_peak_threshold) {
        fd->cooldown_left = fd->cooldown_blocks;
        return FAULT_NOISE_SPIKE;
    }

    return FAULT_NONE;
}

const char *fault_name(FaultType t)
{
    switch (t) {
        case FAULT_DROPOUT:     return "DROPOUT";
        case FAULT_NOISE_SPIKE: return "NOISE_SPIKE";
        default:                return "NONE";
    }
}
