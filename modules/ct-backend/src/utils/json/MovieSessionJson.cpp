#include <ct-backend/utils/json/MovieSessionJson.h>

namespace ct::model
{
    void to_json(nlohmann::json& j, const MovieSession& movieSession)
    {
        j = nlohmann::json{
            {"id", movieSession.id},
            {"roomId", movieSession.roomId},
            {"day", movieSession.day},
            {"time", movieSession.time},
            {"price", movieSession.price}
        };
    }

    void from_json(const nlohmann::json& j, MovieSession& movieSession)
    {
        j.at("id").get_to(movieSession.id);
        j.at("roomId").get_to(movieSession.roomId);
        j.at("day").get_to(movieSession.day);
        j.at("time").get_to(movieSession.time);
        j.at("price").get_to(movieSession.price);
    }
}
