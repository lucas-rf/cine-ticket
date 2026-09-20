#pragma once

#include <ct-core/utils/Clock.h>
#include <string>

namespace ct::impl
{
    struct RAMOrder
    {
        int id;
        int movieSessionId;
        std::string userEmail;
        std::string orderKey;
        Clock::time_point orderTime;
        std::vector<int> seats;
    };
}
