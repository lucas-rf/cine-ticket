#include <ct-backend/utils/json/MovieSessionJson.h>

namespace ct::model
{
    void to_json(nlohmann::ordered_json& j, const MovieSession& movieSession)
    {
        j = nlohmann::ordered_json{
            {"id", movieSession.id},
            {"roomId", movieSession.roomId},
            {"movieId", movieSession.movieId},
            {"day", movieSession.day},
            {"time", movieSession.time},
            {"price", movieSession.price}
        };
    }

    void from_json(const nlohmann::ordered_json& j, MovieSession& movieSession)
    {
        j.at("id").get_to(movieSession.id);
        j.at("roomId").get_to(movieSession.roomId);
        j.at("movieId").get_to(movieSession.movieId);
        j.at("day").get_to(movieSession.day);
        j.at("time").get_to(movieSession.time);
        j.at("price").get_to(movieSession.price);
    }
}
