#pragma once

#include <ct-core/model/Theater.h>
#include <nlohmann/json.hpp>

namespace ct::model
{
    void to_json(nlohmann::json& j, const Theater& theater);
    void from_json(const nlohmann::json& j, Theater& theater);
}
