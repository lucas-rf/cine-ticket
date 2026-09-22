#include <ct-core/utils/Time.h>
#include <sstream>
#include <format>

namespace ct::utils
{
    std::string TimePointToStr(const Clock::time_point& tp)
    {
        return std::format("{:%Y-%m-%d %H:%M:%S}", std::chrono::floor<std::chrono::seconds>(tp));
    }

    Clock::time_point StrToTimePoint(const std::string& str)
    {
        std::stringstream ss{str};
        Clock::time_point tp;
        ss >> std::chrono::parse("%Y-%m-%d %H:%M:%S", tp);
        return tp;
    }
}
