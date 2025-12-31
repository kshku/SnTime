# SnTime

SnTime is cross-platform time abstraction library.

It Provides high-resolution monotonic clock, wall-clock(UTC) time, sleep utilities,
and time conversion helpers.

## Clock Model

### 1. Monotonic Clock
Properties:
- Never goes backwards
- Immune to system clock changes
- Origin is unspecified and meaning less
- Safe for arithmetic comparisions

### 2. Wall Clock(UTC)
Properties:
- Represents real-world UTC time
- May jump forwards or backwards
- Subject to system clock adjustments

## Types
### Duration Types
```c
typedef int64_t snTimeNs;
typedef int64_t snTimeMs;
typedef int64_t snTImeUs;
```
All durations are 64 bit integers.

### Time Point
```c
typedef struct snTimePoint {
    snTimeNs t;
} snTimePoint;
```
A `snTimePoint` captures moment from the monotonic clock.

Time points:
- Must only be compared with other time points
- Must originate from same clock domain
- Are opaque by convention (value should not be interpreted)

### Wall time
```c
typedef struct snWallTime {
    int64_t seconds; /**< seconds since unix epoch (UTC) */
    int64_t nanoseconds; /**< 0 .. 999,999,999 */
} snWallTime;
```
```c
typedef struct snWallTimeUtc {
    int32_t nanosecond; /**< 0 .. 999,999,999 */
    int16_t year; /**< ex: 2025 */
    int8_t month; /**< 1-12 */
    int8_t day; /**< 1-31 */
    int8_t hour; /**< 0-23 */
    int8_t minute; /**< 0-59 */
    int8_t second; /**< 0-60 (leap seconds safe) */
} snWallTimeUtc;
```
