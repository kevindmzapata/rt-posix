#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "periodic_settings.h"

#define NSEC_PER_SEC 1000000000ULL

void current_time()
{
    time_t sec;
    suseconds_t usec;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    sec = tv.tv_sec;
    usec = tv.tv_usec;

    printf("El tiempo actual es: %ld.%06ld\n", sec, usec);
}

void timespec_add_us(struct timespec *t, uint64_t us)
{
    t->tv_nsec += us * 1000;
    if (t->tv_nsec > NSEC_PER_SEC)
    {
        t->tv_nsec -= NSEC_PER_SEC;
        t->tv_sec++;
    }
}

int timespec_cmp(struct timespec *a, struct timespec *b)
{
    if (a->tv_sec > b->tv_sec) return 1;
    else if (a->tv_sec < b->tv_sec) return -1;
    else if (a->tv_sec == b->tv_sec)
    {
        if (a->tv_nsec > b->tv_nsec) return 1;
        else if (a->tv_nsec == b->tv_nsec) return 0;
        else return -1;
    }
}

void wait_next_activation(struct periodic_thread *t)
{
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &t->r, NULL);
    timespec_add_us(&t->r, t->period);
}

void start_periodic_timer(struct periodic_thread *perthread)
{
    // printf("Este hilo tiene un periodo de %d us\n", perthread->period);
    printf("El Hilo %d se ejecutara cada %d us\n", perthread->thread_id, perthread->period);
    printf("El offset de este hilo es %d us\n\n", perthread->offset);

    clock_gettime(CLOCK_REALTIME, &perthread->r);      // Get current time
    timespec_add_us(&perthread->r, perthread->offset); // Add offset
}
