#define _GNU_SOURCE
#include "sntime/sntime.h"

#if defined(SN_OS_LINUX) || defined(SN_OS_MAC)

#include <time.h>
#include <errno.h>

snTimeNs sn_time_now_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (snTimeNs)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

void sn_time_sleep_ns(snTimeNs ns) {
    if (ns <= 0) return;

    struct timespec ts;
    ts.tv_sec = ns / 1000000000LL;
    ts.tv_nsec = ns % 1000000000LL;

    while (nanosleep(&ts, &ts) == -1 && errno == EINTR); // retries
}

void sn_time_sleep_ms(snTimeMs ms) {
    sn_time_sleep_ns(ms * 1000000LL);
}

snWallTime sn_wall_time_now(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    snWallTime t;
    t.seconds = ts.tv_sec;
    t.nanoseconds = ts.tv_nsec;
    return t;
}

bool sn_wall_time_to_utc(snWallTime wall, snWallTimeUtc* utc) {
    if (!utc) return false;
    if (!sn_wall_time_validate(wall)) return false;

    time_t sec = (time_t)wall.seconds;
    struct tm tm;

#if defined(SN_OS_LINUX) || defined(SN_OS_MAC)
    if (!gmtime_r(&sec, &tm)) return false;
#else
    struct tm* tmp = gmtime(&sec);
    if (!tmp) return false;
    tm = *tmp;
#endif

    *utc = (snWallTimeUtc) {
        .year = tm.tm_year + 1900,
        .month = tm.tm_mon + 1,
        .day = tm.tm_mday,
        .hour = tm.tm_hour,
        .minute = tm.tm_min,
        .second = tm.tm_sec, // may be 60
        .nanosecond = wall.nanoseconds
    };

    return true;
}

#endif
