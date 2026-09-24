#include <ct-backend/utils/json/RoomSessionJson.h>
#include <ct-backend/utils/json/TheaterJson.h>
#include <ct-backend/utils/json/RoomJson.h>
#include <ct-backend/utils/json/MovieJson.h>
#include <ct-backend/utils/json/SeatJson.h>

namespace ct::model
{
    void to_json(nlohmann::json& j, const RoomSession& roomSession)
    {
        j = nlohmann::json{
            {"id", roomSession.id},
            {"day", roomSession.day},
            {"time", roomSession.time},
            {"price", roomSession.price},
            {"theater", roomSession.theater},
            {"room", roomSession.room},
            {"movie", roomSession.movie},
            {"seats", roomSession.seats}
        };
    }

    void from_json(const nlohmann::json& j, RoomSession& roomSession)
    {
        j.at("id").get_to(roomSession.id);
        j.at("day").get_to(roomSession.day);
        j.at("time").get_to(roomSession.time);
        j.at("price").get_to(roomSession.price);
        j.at("theater").get_to(roomSession.theater);
        j.at("room").get_to(roomSession.room);
        j.at("movie").get_to(roomSession.movie);
        j.at("seats").get_to(roomSession.seats);
    }
}
