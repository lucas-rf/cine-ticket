#pragma once

#include <ct-core/Timer.h>
#include <thread>
#include <mutex>

namespace ct::impl
{
    class TimerImpl: public Timer
    {
    public:
        TimerImpl();

        virtual ID Set(TimePoint when, Callback&& callback) override;
        virtual bool Cancel(ID id) noexcept override;

    private:
        void threadEntry();
    };
}