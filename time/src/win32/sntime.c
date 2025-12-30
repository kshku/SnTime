#include "sntime/sntime.h"

#if defined(SN_OS_WINDOWS)

#include <windows.h>

static LARGE_INTEGER qpc_freequency = 0;

bool sn_time_init(void) {
    if (!QueryPerformanceFrequency(&qpc_freequency)) return false;
    return true;
}

void sn_time_deinit(void) {
    qpc_freequency = 0;
}

snTimeNs sn_time_now_ns(void) {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    return (snTimeNs)((counter.QuadPart * 1000000000ll) / qpc_freequency.QuadPart);
}

void sn_time_sleep_ns(snTimeNs ns) {
    if (ns <= 0) return;

    snTimeMs ms = ns / 1000000;
    if (ms == 0) ms = 1;
    Sleep((DWORD)ms);
}

void sn_time_sleep_ms(snTimeMs ms) {
    if (ms <= 0) return;
    Sleep((DWORD)ms);
}

snWallTime sn_wall_time_now(void) {
    FILETIME ft;
    GetSystemTimePreciseAsFileTime(&ft);

    uint64_t ticks = ((uint64_t)ft.dwHighDateTime << 32) | (uint64_t)ft.dwLowDateTime;

    // Convert FILETIME → Unix epoch
    ticks -= 116444736000000000ULL;

    return (snWallTime){.seconds = ticks / 10000000ULL, .nanoseconds = (int32_t)((ticks % 10000000ULL) * 100)};
}

bool sn_wall_time_to_utc(snWallTime wall, snWallTimeUtc* utc) {
    if (!utc) return false;
    if (!sn_wall_time_validate(wall)) return false;

    FILETIME ft;
    uint64_t ticks = ((uint64_t)wall.seconds * 10000000ULL) + (wall.nanoseconds / 100);

    ticks += 116444736000000000ULL;

    ft.dwLowDateTime = (DWORD)(ticks & 0xFFFFFFFF);
    ft.dwHighDateTime = (DWORD)(ticks >> 32);

    SYSTEMTIME st;
    if (!FileTimeToSystemTime(&ft, &st)) return false;

    *utc = (snWallTimeUtc) {
        .year = st.wYear,
        .month = (int8_t)st.wMonth,
        .day = (int8_t)st.wDay,
        .hour = (int8_t)st.wHour,
        .minute = (int8_t)st.wMinute,
        .second = (int8_t)st.wSecond,
        .nanosecond = wall.nanoseconds
    };

    return true;
}

#endif
