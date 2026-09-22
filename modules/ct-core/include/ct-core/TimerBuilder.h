#pragma once

#include <ct-core/Timer.h>
#include <memory>

namespace ct
{
    class TimerBuilder
    {
    public:
        virtual std::unique_ptr<Timer> CreateTimer() const = 0;

        virtual ~TimerBuilder() = 0 {}
    };
}
