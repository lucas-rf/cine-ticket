#pragma once

#include <ct-core/model/Room.h>
#include <vector>
#include <string>

namespace ct::model
{
    struct Theater
    {
        int id;
        std::string name;
        std::vector<Room> rooms;
    };
}
