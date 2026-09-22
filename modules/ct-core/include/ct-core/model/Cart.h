#pragma once

#include <ct-core/utils/Clock.h>
#include <ct-core/model/Seat.h>
#include <vector>

namespace ct::model
{
    struct Cart
    {
        int id;
        int movieSessionId;
        int userKey;
        Clock::time_point expirationTime;
        int seatCount;
        std::vector<Seat> seats;
    };
}
