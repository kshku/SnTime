#include <sntime/sntime.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TEST_ASSERT(cond) do { \
    if (!(cond)) { \
        fprintf(stderr, "[FAIL] %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        exit(1); \
    } \
} while (0)

#define TEST_INFO(msg) \
    fprintf(stdout, "[INFO] %s\n", msg)

#include "sntime/sntime.h"
#include <stdio.h>

/* ---------------------------
   A. Init / deinit
--------------------------- */

static void test_init_deinit(void) {
    TEST_INFO("Init / deinit");

    TEST_ASSERT(sn_time_init() == true);
    sn_time_deinit();

    /* deinit without init should be safe */
    sn_time_deinit();

    /* Re-init */
    TEST_ASSERT(sn_time_init() == true);
}

/* ---------------------------
   B. Monotonic Clock
--------------------------- */

static void test_monotonic_order(void) {
    TEST_INFO("Monotonic ordering");

    snTimeNs t1 = sn_time_now_ns();
    snTimeNs t2 = sn_time_now_ns();

    TEST_ASSERT(t2 >= t1);
}

static void test_monotonic_progress(void) {
    TEST_INFO("Monotonic progress over sleep");

    snTimeNs t1 = sn_time_now_ns();
    sn_time_sleep_ms(10);
    snTimeNs t2 = sn_time_now_ns();

    TEST_ASSERT(t2 > t1);
}

static void test_time_diff(void) {
    TEST_INFO("Time diff correctness");

    snTimeNs start = sn_time_now_ns();
    sn_time_sleep_ms(5);
    snTimeNs end = sn_time_now_ns();

    snTimeNs diff = sn_time_diff_ns(start, end);
    TEST_ASSERT(diff > 0);
}

static void test_time_point(void) {
    TEST_INFO("Time point API");

    snTimePoint a = sn_time_point_now();
    sn_time_sleep_ms(2);
    snTimePoint b = sn_time_point_now();

    snTimeNs elapsed = sn_time_elapsed_ns(a, b);
    TEST_ASSERT(elapsed > 0);
}

/* ---------------------------
   C. Sleep Semantics
--------------------------- */

static void test_sleep_ns_minimum(void) {
    TEST_INFO("Sleep minimum guarantee");

    const snTimeNs req = 5 * 1000 * 1000; /* 5 ms */

    snTimeNs start = sn_time_now_ns();
    sn_time_sleep_ns(req);
    snTimeNs end = sn_time_now_ns();

    snTimeNs elapsed = end - start;

    /* Must not wake early */
    TEST_ASSERT(elapsed >= req);
}

static void test_sleep_zero(void) {
    TEST_INFO("Sleep zero duration");

    snTimeNs start = sn_time_now_ns();
    sn_time_sleep_ns(0);
    snTimeNs end = sn_time_now_ns();

    TEST_ASSERT(end >= start);
}

static void test_sleep_overshoot_tolerance(void) {
    TEST_INFO("Sleep overshoot tolerance");

    const snTimeMs req_ms = 10;

    snTimeNs start = sn_time_now_ns();
    sn_time_sleep_ms(req_ms);
    snTimeNs end = sn_time_now_ns();

    snTimeNs elapsed_ms = (end - start) / 1000000;

    /* Allow large overshoot on Windows */
    TEST_ASSERT(elapsed_ms >= req_ms);
    TEST_ASSERT(elapsed_ms < req_ms + 100); /* sanity cap */
}

/* ---------------------------
   D. Wall Clock (UTC)
--------------------------- */

static void test_wall_time_basic(void) {
    TEST_INFO("Wall time basic validity");

    snWallTime t = sn_wall_time_now();

    TEST_ASSERT(sn_wall_time_validate(t));
    TEST_ASSERT(t.seconds > 1600000000); /* > 2020 */
}

static void test_wall_time_progress(void) {
    TEST_INFO("Wall time progresses");

    snWallTime a = sn_wall_time_now();
    sn_time_sleep_ms(20);
    snWallTime b = sn_wall_time_now();

    TEST_ASSERT(b.seconds >= a.seconds);
}

static void test_wall_time_to_utc(void) {
    TEST_INFO("Wall time to UTC conversion");

    snWallTime t = sn_wall_time_now();
    snWallTimeUtc utc;

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

    snWallTime a = sn_wall_time_now();
    snWallTime b = sn_wall_time_now();

    TEST_ASSERT(b.seconds >= a.seconds);
}

/* ---------------------------
   Entry Point
--------------------------- */

int main(void) {
    TEST_INFO("===== SnTime Tests Begin =====");

    test_init_deinit();

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

    sn_time_deinit();

    TEST_INFO("===== SnTime Tests PASSED =====");
    return 0;
}

