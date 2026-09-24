#pragma once

#include <ct-core/utils/Clock.h>
#include <string>

namespace ct::impl
{
    struct RAMBooking
    {
        int id;
        int movieSessionId;
        std::string userEmail;
        std::string bookingKey;
        Clock::time_point bookingTime;
        std::vector<int> seats;
    };
}
