#pragma once

#include <ct-core/utils/Clock.h>
#include <functional>
#include <chrono>

namespace ct
{
    class Timer
    {
    public:
        using ID = long long int;
        using TimePoint = Clock::time_point;
        using Callback = std::function<void()>;

        virtual ID Set(TimePoint when, Callback&& callback) = 0;
        virtual bool Cancel(ID id) noexcept = 0;

        virtual ~Timer() = default;
    };
}
