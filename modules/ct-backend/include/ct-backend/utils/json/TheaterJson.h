#pragma once

#include <ct-core/model/Theater.h>
#include <nlohmann/json.hpp>

namespace ct::model
{
    void to_json(nlohmann::ordered_json& j, const Theater& theater);
    void from_json(const nlohmann::ordered_json& j, Theater& theater);
}
