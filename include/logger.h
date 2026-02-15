#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

typedef struct {
    FILE *fp;
} Logger;

// Returns 0 on success
int logger_open(Logger *lg, const char *path);
void logger_close(Logger *lg);

// Writes one CSV row
void logger_write(Logger *lg,
                  long long t_ms,
                  float raw_rms, float raw_peak,
                  float fil_rms, float fil_peak,
                  const char *fault);

#endif
