# SnTime

Cross-platform time abstraction library written in C.

Provides high-resolution monotonic clock, wall-clock (UTC) time, sleep utilities,
and time conversion helpers.

## Clock Model

### 1. Monotonic Clock
Properties:
- Never goes backwards
- Immune to system clock changes
- Origin is unspecified and meaningless
- Safe for arithmetic comparisons

### 2. Wall Clock (UTC)
Properties:
- Represents real-world UTC time
- May jump forwards or backwards
- Subject to system clock adjustments

## Types

### Duration Types

```c
typedef int64_t SnTimeNs;  // nanoseconds
typedef int64_t SnTimeUs;  // microseconds
typedef int64_t SnTimeMs;  // milliseconds
```

### Time Point

```c
typedef struct SnTimePoint {
    SnTimeNs t;
} SnTimePoint;
```

A `SnTimePoint` captures a moment from the monotonic clock. Time points must only be compared with other time points from the same clock domain.

### Wall Time

```c
typedef struct SnWallTime {
    int64_t seconds;      // seconds since unix epoch (UTC)
    int64_t nanoseconds;  // 0 .. 999,999,999
} SnWallTime;

typedef struct SnWallTimeUtc {
    int32_t nanosecond;   // 0 .. 999,999,999
    int16_t year;         // e.g. 2025
    int8_t month;         // 1-12
    int8_t day;           // 1-31
    int8_t hour;          // 0-23
    int8_t minute;        // 0-59
    int8_t second;        // 0-60 (leap seconds safe)
} SnWallTimeUtc;
```

## API

### Monotonic Clock

| Function | Description |
|----------|-------------|
| `SnTimeNs sn_time_now_ns(void)` | Current monotonic time in nanoseconds |
| `SnTimeUs sn_time_now_us(void)` | Current monotonic time in microseconds |
| `SnTimeMs sn_time_now_ms(void)` | Current monotonic time in milliseconds |

### Time Difference

| Function | Description |
|----------|-------------|
| `SnTimeNs sn_time_diff_ns(SnTimeNs start, SnTimeNs end)` | `end - start` in nanoseconds |
| `SnTimeUs sn_time_diff_us(SnTimeUs start, SnTimeUs end)` | `end - start` in microseconds |
| `SnTimeMs sn_time_diff_ms(SnTimeMs start, SnTimeMs end)` | `end - start` in milliseconds |

### Unit Conversions

| Function | Description |
|----------|-------------|
| `double sn_time_ns_to_seconds(SnTimeNs ns)` | Nanoseconds to seconds |
| `double sn_time_us_to_seconds(SnTimeUs us)` | Microseconds to seconds |
| `double sn_time_ms_to_seconds(SnTimeMs ms)` | Milliseconds to seconds |
| `SnTimeNs sn_time_seconds_to_ns(double seconds)` | Seconds to nanoseconds |
| `SnTimeUs sn_time_seconds_to_us(double seconds)` | Seconds to microseconds |
| `SnTimeMs sn_time_seconds_to_ms(double seconds)` | Seconds to milliseconds |

### Time Points

| Function | Description |
|----------|-------------|
| `SnTimePoint sn_time_point_now(void)` | Capture current monotonic time point |
| `SnTimeNs sn_time_elapsed_ns(SnTimePoint start, SnTimePoint end)` | Elapsed time in nanoseconds |

### Sleep

| Function | Description |
|----------|-------------|
| `void sn_time_sleep_ns(SnTimeNs ns)` | Sleep for at least `ns` nanoseconds |
| `void sn_time_sleep_ms(SnTimeMs ms)` | Sleep for at least `ms` milliseconds |

### Wall Clock

| Function | Description |
|----------|-------------|
| `SnWallTime sn_wall_time_now(void)` | Current UTC wall clock time |
| `bool sn_wall_time_to_utc(SnWallTime wall_time, SnWallTimeUtc *utc)` | Convert to broken-down UTC |
| `bool sn_wall_time_validate(SnWallTime t)` | Validate nanosecond field range |

## Usage

```c
#include <sntime/sntime.h>
#include <stdio.h>

int main(void) {
    // Monotonic timing
    SnTimeNs t1 = sn_time_now_ns();
    sn_time_sleep_ms(10);
    SnTimeNs t2 = sn_time_now_ns();
    printf("Elapsed: %.3f ms\n", sn_time_ns_to_seconds(t2 - t1) * 1000.0);

    // Wall clock
    SnWallTime wt = sn_wall_time_now();
    SnWallTimeUtc utc;
    if (sn_wall_time_to_utc(wt, &utc))
        printf("UTC: %04d-%02d-%02d %02d:%02d:%02d\n",
               utc.year, utc.month, utc.day,
               utc.hour, utc.minute, utc.second);

    return 0;
}
```

## Adding to your project

```cmake
include(FetchContent)
FetchContent_Declare(sntime
    GIT_REPOSITORY https://github.com/kshku/SnTime.git
    GIT_TAG <tag>  # e.g., v0.1.0
)
FetchContent_MakeAvailable(sntime)

target_link_libraries(myapp PRIVATE sntime)
```

## Build

```sh
cmake -B build
cmake --build build
```

| Option | Default | Description |
|--------|---------|-------------|
| `SN_TIME_BUILD_SHARED` | `OFF` | Build as shared library |
| `SN_TIME_BUILD_TEST` | `OFF` | Build tests |

## Platform Notes

| Function | Linux | macOS | Windows |
|----------|-------|-------|---------|
| `sn_time_now_ns` | `CLOCK_MONOTONIC_RAW` | `CLOCK_MONOTONIC` | `QueryPerformanceCounter` |
| `sn_time_sleep_ns` | `nanosleep` (EINTR retry) | `nanosleep` (EINTR retry) | `Sleep` (rounded up to 1 ms) |
| `sn_wall_time_now` | `clock_gettime(CLOCK_REALTIME)` | `clock_gettime(CLOCK_REALTIME)` | `GetSystemTimePreciseAsFileTime` |

## Dependencies

- **SnCore** — fetched automatically via FetchContent
