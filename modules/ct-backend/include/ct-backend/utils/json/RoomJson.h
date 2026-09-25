#pragma once

#include <ct-core/model/Room.h>
#include <nlohmann/json.hpp>

namespace ct::model
{
    void to_json(nlohmann::ordered_json& j, const Room& room);
    void from_json(const nlohmann::ordered_json& j, Room& room);
}
