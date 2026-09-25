#pragma once

#include <ct-core/model/Booking.h>
#include <nlohmann/json.hpp>

namespace ct::model
{
    void to_json(nlohmann::ordered_json& j, const Booking& booking);
    void from_json(const nlohmann::ordered_json& j, Booking& booking);
}
