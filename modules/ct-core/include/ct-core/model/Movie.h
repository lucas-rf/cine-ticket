#pragma once

#include <ct-core/model/Theater.h>
#include <vector>
#include <string>

namespace ct::model
{
    struct Movie
    {
        int id;
        std::string title;
        std::string synopsis;
        std::string classification;
        std::vector<std::string> genre;
        std::string coverImage;
        std::string backgroundImage;
        int runtime;
        std::vector<Theater> theaters;
    };
}

