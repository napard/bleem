/*
 * time.c
 * Timing.
 *
 * 05 ago 2022 -- 09:09 -03
 * Notes:
 */

#include <stdbool.h>
#include <time.h>
#include "risc.h"

static const char* THIS_FILE = "time.c";

extern risc_vm_t* g_cpu;

static _Bool g_starting = true;
static double g_start_value;
static double g_two_to_thirty_two = 4294967296;

double risc_get_elapsed_time()
{
#ifdef __linux__
    struct timespec start;

    if (clock_gettime(CLOCK_MONOTONIC, &start))
        risc_log_warn(g_cpu, "'clock_gettime(...)' failed !!!", THIS_FILE, __LINE__);

    double elapsed = g_two_to_thirty_two;
    if (g_starting)
    {
        g_start_value =
            (double)start.tv_sec +
            (double)start.tv_nsec / 1.0e9;
        g_starting = false;
    }
    else
    {
        elapsed +=
            (double)start.tv_sec +
            (double)start.tv_nsec / 1.0e9 -
            g_start_value;
    }

    return elapsed;
#endif /* __linux__ */

    return 0;
}
