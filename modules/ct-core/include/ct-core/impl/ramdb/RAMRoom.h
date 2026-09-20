#pragma once

#include <string>

namespace ct::impl
{
    struct RAMRoom
    {
        int id;
        int theaterId;
        int number;
        std::string type;
        int rows;
        int columns;
    };
}
