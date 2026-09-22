#pragma once

#include <ct-core/utils/Clock.h>
#include <string>

namespace ct::utils
{
    std::string TimePointToStr(const Clock::time_point& tp);
    Clock::time_point StrToTimePoint(const std::string& str);
}
