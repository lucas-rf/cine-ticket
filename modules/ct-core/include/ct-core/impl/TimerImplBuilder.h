#pragma once

#include <ct-core/TimerBuilder.h>

namespace ct::impl
{
    class TimerImplBuilder: public TimerBuilder
    {
    public:
        virtual std::unique_ptr<Timer> CreateTimer() const override;

        static const TimerImplBuilder Instance;

    private:
        TimerImplBuilder();
    };
}
