#pragma once

#include <ct-core/utils/Clock.h>

namespace ct::test
{
    class MockClock: public Clock
    {
    public:
        virtual time_point now() const noexcept override
        {
            return NowValue;
        }

        time_point NowValue{};
    };
}
