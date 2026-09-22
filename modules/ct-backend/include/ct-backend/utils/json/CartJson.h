#pragma once

#include <ct-core/model/Cart.h>
#include <nlohmann/json.hpp>

namespace ct::model
{
    void to_json(nlohmann::json& j, const Cart& cart);
    void from_json(const nlohmann::json& j, Cart& cart);
}
