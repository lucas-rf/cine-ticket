#pragma once

#include <chrono>

namespace ct
{
    // using Clock = std::chrono::system_clock;

    class Clock
    {
    public:
        using ref_clock = std::chrono::system_clock;
        using time_point = ref_clock::time_point;
        using duration = ref_clock::duration;

        virtual time_point now() const noexcept = 0;

        virtual ~Clock() = default;
    };
}
