#pragma once

#include <ct-core/utils/Clock.h>

namespace ct::impl
{
    class ClockImpl: public Clock
    {
    public:
        ClockImpl();
        virtual time_point now() const noexcept override;

        static const ClockImpl Instance;
    };
}
