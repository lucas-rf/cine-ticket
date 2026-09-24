#pragma once

#include <ct-core/model/Booking.h>
#include <nlohmann/json.hpp>

namespace ct::model
{
    void to_json(nlohmann::json& j, const Booking& booking);
    void from_json(const nlohmann::json& j, Booking& booking);
}
