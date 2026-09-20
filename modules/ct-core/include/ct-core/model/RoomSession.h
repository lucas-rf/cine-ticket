#pragma once

#include <ct-core/model/Theater.h>
#include <ct-core/model/Seat.h>
#include <vector>

namespace ct::model
{
    struct RoomSession
    {
        Theater theater;
        std::vector<Seat> seats;
    };
}
