# Changelog

## [0.2.2] - 2026-09-28

### Changed
- -Wconversion and -Wsign-conversion are on for gcc and clang. sn_wall_time_now
  and sn_wall_time_to_utc left their narrowing from the timespec and struct tm
  fields to implicit conversions; the values are in range by contract, so the
  casts are now explicit


## [0.2.1] - 2026-09-28

### Changed
- Take sncore v0.3.1 rather than v0.2.0

## [0.2.0] - 2026-06-29

### Changed
- Updated the dependency versions

## [0.1.0] - 2026-06-11

- First release. See [0.0.0] section in CHANGELOG.md for full changelog.

## [0.0.0] - 2025-12-30

### Added
- Monotonic clock (`sn_time_monotonic`) for elapsed time measurement
- Wall clock (`sn_time_wall`) for UTC date/time
- Wall clock to UTC struct conversion (`sn_time_to_utc_struct`)
- Millisecond-precision sleep (`sn_time_sleep_ms`)
- Microsecond-precision sleep (`sn_time_sleep_us`)
- Time conversion helpers
- POSIX backend (`clock_gettime` / `nanosleep`)
- Windows backend (`QueryPerformanceCounter` / `Sleep`)
- SnCore dependency
- Test suite (basic validity, progression, UTC conversion, non-decreasing monotonic clock)
- CI workflows (Linux, macOS, Windows, formatting)
