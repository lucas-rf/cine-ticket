#pragma once

#include <string>

namespace ct::impl
{
    struct RAMMovieSession
    {
        int id;
        int roomId;
        int day;
        int time;
        float price;
        int startingSeatId;
    };
}
