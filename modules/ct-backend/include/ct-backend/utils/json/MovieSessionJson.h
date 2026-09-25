#pragma once

#include <ct-core/model/MovieSession.h>
#include <nlohmann/json.hpp>

namespace ct::model
{
    void to_json(nlohmann::ordered_json& j, const MovieSession& movieSession);
    void from_json(const nlohmann::ordered_json& j, MovieSession& movieSession);
}
