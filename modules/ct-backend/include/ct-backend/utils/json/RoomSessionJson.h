#pragma once

#include <ct-core/model/RoomSession.h>
#include <nlohmann/json.hpp>

namespace ct::model
{
    void to_json(nlohmann::ordered_json& j, const RoomSession& roomSession);
    void from_json(const nlohmann::ordered_json& j, RoomSession& roomSession);
}
