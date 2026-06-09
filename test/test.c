#include <math.h>
#include <sntime/sntime.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_ASSERT(cond)                                                     \
    do {                                                                      \
        if (!(cond)) {                                                        \
            fprintf(stderr, "[FAIL] %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            exit(1);                                                          \
        }                                                                     \
    } while (0)

#define TEST_INFO(msg) fprintf(stdout, "[INFO] %s\n", msg)

#include "sntime/sntime.h"

#include <stdio.h>

/* ---------------------------
   B. Monotonic Clock
--------------------------- */

static void test_monotonic_order(void) {
    TEST_INFO("Monotonic ordering");

    SnTimeNs t1 = sn_time_now_ns();
    SnTimeNs t2 = sn_time_now_ns();

    TEST_ASSERT(t2 >= t1);
}

static void test_monotonic_progress(void) {
    TEST_INFO("Monotonic progress over sleep");

    SnTimeNs t1 = sn_time_now_ns();
    sn_time_sleep_ms(10);
    SnTimeNs t2 = sn_time_now_ns();

    TEST_ASSERT(t2 > t1);
}

static void test_time_diff(void) {
    TEST_INFO("Time diff correctness");

    SnTimeNs start = sn_time_now_ns();
    sn_time_sleep_ms(5);
    SnTimeNs end = sn_time_now_ns();

    SnTimeNs diff = sn_time_diff_ns(start, end);
    TEST_ASSERT(diff > 0);
}

static void test_time_point(void) {
    TEST_INFO("Time point API");

    SnTimePoint a = sn_time_point_now();
    sn_time_sleep_ms(2);
    SnTimePoint b = sn_time_point_now();

    SnTimeNs elapsed = sn_time_elapsed_ns(a, b);
    TEST_ASSERT(elapsed > 0);
}

/* ---------------------------
   C. Sleep Semantics
--------------------------- */

static void test_sleep_ns_minimum(void) {
    TEST_INFO("Sleep minimum guarantee");

    const SnTimeNs req = 5 * 1000 * 1000; /* 5 ms */

    SnTimeNs start = sn_time_now_ns();
    sn_time_sleep_ns(req);
    SnTimeNs end = sn_time_now_ns();

    SnTimeNs elapsed = end - start;

    /* Must not wake early */
    TEST_ASSERT(elapsed >= req);
}

static void test_sleep_zero(void) {
    TEST_INFO("Sleep zero duration");

    SnTimeNs start = sn_time_now_ns();
    sn_time_sleep_ns(0);
    SnTimeNs end = sn_time_now_ns();

    TEST_ASSERT(end >= start);
}

static void test_sleep_overshoot_tolerance(void) {
    TEST_INFO("Sleep overshoot tolerance");

    const SnTimeMs req_ms = 10;

    SnTimeNs start = sn_time_now_ns();
    sn_time_sleep_ms(req_ms);
    SnTimeNs end = sn_time_now_ns();

    SnTimeNs elapsed_ms = (end - start) / 1000000;

    /* Allow large overshoot on Windows */
    TEST_ASSERT(elapsed_ms >= req_ms);
    TEST_ASSERT(elapsed_ms < req_ms + 100); /* sanity cap */
}

/* ---------------------------
   D. Wall Clock (UTC)
--------------------------- */

static void test_wall_time_basic(void) {
    TEST_INFO("Wall time basic validity");

    SnWallTime t = sn_wall_time_now();

    TEST_ASSERT(sn_wall_time_validate(t));
    TEST_ASSERT(t.seconds > 1600000000); /* > 2020 */
}

static void test_wall_time_progress(void) {
    TEST_INFO("Wall time progresses");

    SnWallTime a = sn_wall_time_now();
    sn_time_sleep_ms(20);
    SnWallTime b = sn_wall_time_now();

    TEST_ASSERT(b.seconds >= a.seconds);
}

static void test_wall_time_to_utc(void) {
    TEST_INFO("Wall time to UTC conversion");

    SnWallTime t = sn_wall_time_now();
    SnWallTimeUtc utc;

    TEST_ASSERT(sn_wall_time_to_utc(t, &utc));

    TEST_ASSERT(utc.year >= 2020);
    TEST_ASSERT(utc.month >= 1 && utc.month <= 12);
    TEST_ASSERT(utc.day >= 1 && utc.day <= 31);
    TEST_ASSERT(utc.hour >= 0 && utc.hour <= 23);
    TEST_ASSERT(utc.minute >= 0 && utc.minute <= 59);
    TEST_ASSERT(utc.second >= 0 && utc.second <= 60);
    TEST_ASSERT(utc.nanosecond >= 0 && utc.nanosecond < 1000000000);
}

static void test_wall_time_monotonicish(void) {
    TEST_INFO("Wall time non-decreasing");

    SnWallTime a = sn_wall_time_now();
    SnWallTime b = sn_wall_time_now();

    TEST_ASSERT(b.seconds >= a.seconds);
}

/* ---------------------------
   Entry Point
--------------------------- */

int main(void) {
    TEST_INFO("===== SnTime Tests Begin =====");

    test_monotonic_order();
    test_monotonic_progress();
    test_time_diff();
    test_time_point();

    test_sleep_ns_minimum();
    test_sleep_zero();
    test_sleep_overshoot_tolerance();

    test_wall_time_basic();
    test_wall_time_progress();
    test_wall_time_to_utc();
    test_wall_time_monotonicish();

    TEST_INFO("===== SnTime Tests PASSED =====");
    return 0;
}

