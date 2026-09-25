#pragma once

#include <ct-core/model/Seat.h>
#include <nlohmann/json.hpp>

namespace ct::model
{
    void to_json(nlohmann::ordered_json& j, const Seat& seat);
    void from_json(const nlohmann::ordered_json& j, Seat& seat);
}
