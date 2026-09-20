#pragma once

#include <ct-core/utils/Clock.h>
#include <ct-core/model/Seat.h>
#include <vector>
#include <string>

namespace ct::model
{
    struct Order
    {
        int id;
        int movieSessionId;
        std::string userEmail;
        std::string orderKey;
        Clock::time_point orderTime;
        int seatCount;
        std::vector<Seat> seats;
    };
}
