#pragma once

#include <string>
#include <vector>

namespace ct::impl
{
    struct RAMTheater
    {
        int id;
        std::string name;
        std::vector<std::vector<std::vector<int>>> idxMoviesToDaysToSessions;
    };
}
