#pragma once

#include <ct-core/utils/Clock.h>
#include <ct-core/model/Seat.h>
#include <vector>
#include <string>

namespace ct::model
{
    struct Booking
    {
        int id;
        int movieSessionId;
        std::string userEmail;
        std::string bookingKey;
        Clock::time_point bookingTime;
        int seatCount;
        std::vector<Seat> seats;
    };
}
