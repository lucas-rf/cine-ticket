#pragma once

#include <string>
#include <vector>

namespace ct::impl
{
    struct RAMMovie
    {
        int id;
        std::string title;
        std::string synopsis;
        std::string classification;
        std::vector<std::string> genre;
        std::string coverImage;
        std::string backgroundImage;
        int runtime;
        std::vector<std::vector<int>> idxDaysToSessions;
    };
}
