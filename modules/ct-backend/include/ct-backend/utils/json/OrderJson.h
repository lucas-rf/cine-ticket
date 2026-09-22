#pragma once

#include <ct-core/model/Order.h>
#include <nlohmann/json.hpp>

namespace ct::model
{
    void to_json(nlohmann::json& j, const Order& order);
    void from_json(const nlohmann::json& j, Order& order);
}
