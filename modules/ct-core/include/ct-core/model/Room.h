#pragma once

#include <ct-core/model/MovieSession.h>
#include <vector>
#include <string>

namespace ct::model
{
    struct Room
    {
        int id;
        int theaterId;
        int number;
        std::string type;
        int rows;
        int columns;
        std::vector<MovieSession> sessions;
    };
}
