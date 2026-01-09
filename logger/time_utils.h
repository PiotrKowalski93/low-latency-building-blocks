#pragma once

#include <chrono>
#include <ctime>
#include <string>

namespace Common {
    typedef int64_t Nanos;

    constexpr Nanos NANOS_TO_MICROS = 1000;
    constexpr Nanos MICROS_TO_MILLIS = 1000;
    constexpr Nanos MILLIS_TO_SECS = 1000;

    constexpr Nanos NANOS_TO_MILLIS = NANOS_TO_MICROS * MICROS_TO_MILLIS;
    constexpr Nanos NANOS_TO_SECS = NANOS_TO_MILLIS * MILLIS_TO_SECS;

    inline auto getCurrentNanos() noexcept {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    inline auto getCurrentTimeStr(std::string* time_str){

    }
} 