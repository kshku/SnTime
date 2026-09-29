#define _GNU_SOURCE
#include "sntime/sntime.h"

#if defined(SN_OS_LINUX) || defined(SN_OS_MAC)

    #include <errno.h>
    #include <time.h>

SnTimeNs sn_time_now_ns(void) {
    struct timespec ts;
    #if defined(SN_OS_LINUX)
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    #else
    clock_gettime(CLOCK_MONOTONIC, &ts);
    #endif
    return (SnTimeNs)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

void sn_time_sleep_ns(SnTimeNs ns) {
    if (ns <= 0) return;

    struct timespec ts;
    ts.tv_sec = ns / 1000000000LL;
    ts.tv_nsec = ns % 1000000000LL;

    while (nanosleep(&ts, &ts) == -1 && errno == EINTR);  // retries
}

void sn_time_sleep_ms(SnTimeMs ms) {
    sn_time_sleep_ns(ms * 1000000LL);
}

SnWallTime sn_wall_time_now(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    SnWallTime t;
    /* POSIX bounds tv_nsec to 0 .. 999999999, which fits int32_t. */
    t.seconds = (int64_t)ts.tv_sec;
    t.nanoseconds = (int32_t)ts.tv_nsec;
    return t;
}

bool sn_wall_time_to_utc(SnWallTime wall, SnWallTimeUtc *utc) {
    if (!utc) return false;
    if (!sn_wall_time_validate(wall)) return false;

    time_t sec = (time_t)wall.seconds;
    struct tm tm;

    #if defined(SN_OS_LINUX) || defined(SN_OS_MAC)
    if (!gmtime_r(&sec, &tm)) return false;
    #else
    struct tm *tmp = gmtime(&sec);
    if (!tmp) return false;
    tm = *tmp;
    #endif

    /* A successful gmtime_r leaves every field in its documented range, which is
     * what the narrow SnWallTimeUtc fields are sized for. The casts make that
     * dependence explicit instead of leaving it to an implicit conversion. */
    *utc = (SnWallTimeUtc){
        .year = (int16_t)(tm.tm_year + 1900),
        .month = (int8_t)(tm.tm_mon + 1),
        .day = (int8_t)tm.tm_mday,
        .hour = (int8_t)tm.tm_hour,
        .minute = (int8_t)tm.tm_min,
        .second = (int8_t)tm.tm_sec,  // may be 60
        .nanosecond = wall.nanoseconds};

    return true;
}

#endif
