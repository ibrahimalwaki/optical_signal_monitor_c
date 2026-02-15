#ifndef FAULT_H
#define FAULT_H

#include "processing.h"

typedef enum {
    FAULT_NONE = 0,
    FAULT_DROPOUT,
    FAULT_NOISE_SPIKE
} FaultType;

typedef struct {
    float dropout_rms_threshold;   // if rms < this => dropout
    float spike_peak_threshold;    // if peak > this => spike
    int cooldown_blocks;           // avoid spamming alerts
    int cooldown_left;
} FaultDetector;

void fault_init(FaultDetector *fd,
                float dropout_rms_threshold,
                float spike_peak_threshold,
                int cooldown_blocks);

FaultType fault_update(FaultDetector *fd, Metrics raw);

const char *fault_name(FaultType t);

#endif
