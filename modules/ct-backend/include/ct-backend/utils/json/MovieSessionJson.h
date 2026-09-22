#pragma once

#include <ct-core/model/MovieSession.h>
#include <nlohmann/json.hpp>

namespace ct::model
{
    void to_json(nlohmann::json& j, const MovieSession& movieSession);
    void from_json(const nlohmann::json& j, MovieSession& movieSession);
}
