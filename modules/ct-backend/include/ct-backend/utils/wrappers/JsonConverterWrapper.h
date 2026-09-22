#pragma once

#include <ct-core/utils/Exception.h>
#include <crow/http_response.h>
#include <nlohmann/json.hpp>

namespace ct::utils
{
    template<typename F>
    crow::response JsonConverterWrapper(F&& function)
    {
        try
        {
            nlohmann::json obj(function());
            return crow::response("application/json", obj.dump());
        }
        catch(ItemNotFoundException& exc)
        {
            return crow::response(crow::status::NOT_FOUND, exc.what());
        }
    }
}
