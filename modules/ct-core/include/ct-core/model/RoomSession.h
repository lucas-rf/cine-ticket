#pragma once

#include <ct-core/model/Theater.h>
#include <ct-core/model/Movie.h>
#include <ct-core/model/Seat.h>
#include <vector>

namespace ct::model
{
    struct RoomSession
    {
        int id;
        int day;
        int time;
        float price;
        Theater theater;
        Room room;
        Movie movie;
        std::vector<Seat> seats;
    };
}
