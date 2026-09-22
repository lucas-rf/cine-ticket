#pragma once

#include <ct-core/model/Seat.h>
#include <nlohmann/json.hpp>

namespace ct::model
{
    void to_json(nlohmann::json& j, const Seat& seat);
    void from_json(const nlohmann::json& j, Seat& seat);
}
