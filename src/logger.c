#include "logger.h"

int logger_open(Logger *lg, const char *path)
{
    if (!lg || !path) return -1;

    lg->fp = fopen(path, "w");
    if (!lg->fp) return -2;

    fprintf(lg->fp, "t_ms,raw_rms,raw_peak,fil_rms,fil_peak,fault\n");
    fflush(lg->fp);
    return 0;
}

void logger_close(Logger *lg)
{
    if (!lg || !lg->fp) return;
    fclose(lg->fp);
    lg->fp = NULL;
}

void logger_write(Logger *lg,
                  long long t_ms,
                  float raw_rms, float raw_peak,
                  float fil_rms, float fil_peak,
                  const char *fault)
{
    if (!lg || !lg->fp) return;

    fprintf(lg->fp, "%lld,%.6f,%.6f,%.6f,%.6f,%s\n",
            t_ms, raw_rms, raw_peak, fil_rms, fil_peak,
            fault ? fault : "NONE");

    fflush(lg->fp);
}
