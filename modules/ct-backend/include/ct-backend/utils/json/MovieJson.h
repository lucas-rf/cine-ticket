#pragma once

#include <ct-core/model/Movie.h>
#include <nlohmann/json.hpp>

namespace ct::model
{
    void to_json(nlohmann::ordered_json& j, const Movie& movie);
    void from_json(const nlohmann::ordered_json& j, Movie& movie);
}
