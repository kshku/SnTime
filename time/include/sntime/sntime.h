#pragma once

#include "sntime/api.h"

#include <sncore/defines.h>

/**
 * @brief Signed 64-bit time value representing nanoseconds.
 *
 * Monotonic time is guaranteed to never go backwards.
 * The absolute origin is unspecified and meaningless.
 *
 * @defgroup TimeTypes
 * @{
 */
typedef int64_t SnTimeNs;
typedef int64_t SnTimeUs;
typedef int64_t SnTimeMs;
/** } */

/**
 * @brief Get current monotonic time.
 *
 * @return Returns the current monotonic time in nanoseconds.
 */
SN_TIME_API SnTimeNs sn_time_now_ns(void);

/**
 * @brief Get current monotonic time.
 *
 * @return Returns current monotonic time in microseconds.
 */
SN_FORCE_INLINE SnTimeUs sn_time_now_us(void) {
    return sn_time_now_ns() / 1000;
}

/**
 * @brief Get current monotonic time.
 *
 * @return Returns current monotonic time in milliseconds.
 */
SN_FORCE_INLINE SnTimeMs sn_time_now_ms(void) {
    return sn_time_now_ns() / 1000000;
}

/**
 * @brief Compute the duration between two monotonic time values.
 *
 * The result is end - start.
 *
 * @param start Starting time (in ns)
 * @param end Ending time (in ns)
 *
 * @return Get the difference.
 *
 * @note start and end must originate from the same clock domain.
 */
SN_FORCE_INLINE SnTimeNs sn_time_diff_ns(SnTimeNs start, SnTimeNs end) {
    return end - start;
}

/**
 * @brief Compute the duration between two monotonic time values.
 *
 * The result is end - start.
 *
 * @param start Starting time (in us)
 * @param end Ending time (in us)
 *
 * @return Get the difference.
 *
 * @note start and end must originate from the same clock domain.
 */
SN_FORCE_INLINE SnTimeUs sn_time_diff_us(SnTimeUs start, SnTimeUs end) {
    return end - start;
}

/**
 * @brief Compute the duration between two monotonic time values.
 *
 * The result is end - start.
 *
 * @param start Starting time (in ms)
 * @param end Ending time (in ms)
 *
 * @return Get the difference.
 *
 * @note start and end must originate from the same clock domain.
 */
SN_FORCE_INLINE SnTimeMs sn_time_diff_ms(SnTimeMs start, SnTimeMs end) {
    return end - start;
}

/**
 * @brief Convert from SnTimeNs to seconds.
 *
 * @param ns Time in ns.
 *
 * @return Returns time in secnonds (converted to double).
 */
SN_FORCE_INLINE double sn_time_ns_to_seconds(SnTimeNs ns) {
    return (double)ns / 1e9;
}

/**
 * @brief Convert from SnTimeUs to seconds.
 *
 * @param us Time in us.
 *
 * @return Returns time in secnonds (converted to double).
 */
SN_FORCE_INLINE double sn_time_us_to_seconds(SnTimeUs us) {
    return (double)us / 1e6;
}

/**
 * @brief Convert from SnTimeMs to seconds.
 *
 * @param ms Time in ms.
 *
 * @return Returns time in secnonds (converted to double).
 */
SN_FORCE_INLINE double sn_time_ms_to_seconds(SnTimeMs ms) {
    return (double)ms / 1e3;
}

/**
 * @brief Convert from seconds to SnTimeNs.
 *
 * @param seconds Time in seconds.
 *
 * @return Returns time SnTimeNs.
 */
SN_FORCE_INLINE SnTimeNs sn_time_seconds_to_ns(double seconds) {
    return (SnTimeNs)(seconds * 1e9);
}

/**
 * @brief Convert from seconds to SnTimeUs.
 *
 * @param seconds Time in seconds.
 *
 * @return Returns time SnTimeUs.
 */
SN_FORCE_INLINE SnTimeUs sn_time_seconds_to_us(double seconds) {
    return (SnTimeUs)(seconds * 1e6);
}

/**
 * @brief Convert from seconds to SnTimeMs.
 *
 * @param seconds Time in seconds.
 *
 * @return Returns time SnTimeMs.
 */
SN_FORCE_INLINE SnTimeMs sn_time_seconds_to_ms(double seconds) {
    return (SnTimeMs)(seconds * 1e3);
}

/**
 * @struct SnTimePoint
 * @brief Opaque time point captured from the monotonic clock.
 *
 * Time points should only be compared with other time points
 * from the same clock.
 */
typedef struct SnTimePoint {
    SnTimeNs t;
} SnTimePoint;

/**
 * @brief Capture the current monotonic time as a time point.
 *
 * @return Returns time point.
 */
SN_FORCE_INLINE SnTimePoint sn_time_point_now(void) {
    return (SnTimePoint){.t = sn_time_now_ns()};
}

/**
 * @brief Compute elapsed time between two time points.
 *
 * @param start Starting time point
 * @param end ending time point
 *
 * @return Get elapsed time in ns.
 */
SN_FORCE_INLINE SnTimeNs sn_time_elapsed_ns(SnTimePoint start, SnTimePoint end) {
    return end.t - start.t;
}

/**
 * @brief Suspend execution of the current thread for at least the
 * specified duration.
 *
 * Note:
 *  - Actual sleep time may be longer due to scheduling.
 *  - This function may be interrupted and resumed internally.
 *
 * @defgroup sleep
 * @{
 */
/**
 * @param ns Time to sleep in ns.
 */
SN_TIME_API void sn_time_sleep_ns(SnTimeNs ns);
/**
 * @param ms Time to sleep in ms.
 */
SN_TIME_API void sn_time_sleep_ms(SnTimeMs ms);

/** @} */

/**
 * @struct SnWallTime
 * @brief Wall clock time expressed in UTC.
 *
 * Represents a specific point in real-world time.
 * This clock may jump forwards or backwards.
 */
typedef struct SnWallTime {
    int64_t seconds; /**< Seconds since Unix epoch (1970-01-01 UTC) */
    int32_t nanoseconds; /**< 0 .. 999,999,999 */
} SnWallTime;

/**
 * @struct SnWallTimeUtc
 * @brief Wall clock time expressed in UTC.
 *
 * Represents a specific point in real-world time.
 * This clock may jump forwards or backwards.
 */
typedef struct SnWallTimeUtc {
    int32_t nanosecond; /**< 0 .. 999,999,999 */
    int16_t year; /**< ex: 2025 */
    int8_t month; /**< 1-12 */
    int8_t day; /**< 1-31 */
    int8_t hour; /**< 0-23 */
    int8_t minute; /**< 0-59 */
    int8_t second; /**< 0-60 (leap seconds safe) */
} SnWallTimeUtc;

/**
 * @brief Query the current wall clock time in UTC.
 *
 * @return Get the wall time.
 */
SN_TIME_API SnWallTime sn_wall_time_now(void);

/**
 * @brief Query the current wall clock time in UTC.
 *
 * @param wall_time The wall time.
 * @param utc Pointer to return the utc time.
 *
 * @return Returns true on success, else false.
 */
SN_TIME_API bool sn_wall_time_to_utc(SnWallTime wall_time, SnWallTimeUtc *utc);

/**
 * @brief Validates the wall time struct.
 *
 * @param t The wall time.
 *
 * @return Returns true if wall time is valid, else false.
 */
SN_FORCE_INLINE bool sn_wall_time_validate(SnWallTime t) {
    return t.nanoseconds >= 0 && t.nanoseconds < 1000000000;
}
