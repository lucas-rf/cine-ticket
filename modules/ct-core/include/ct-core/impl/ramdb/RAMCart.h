#pragma once

#include <ct-core/utils/Clock.h>
#include <unordered_set>

namespace ct::impl
{
    struct RAMCart
    {
        int id;
        int movieSessionId;
        int userKey;
        Clock::time_point expirationTime;
        std::unordered_set<int> idxSeats;
    };
}
