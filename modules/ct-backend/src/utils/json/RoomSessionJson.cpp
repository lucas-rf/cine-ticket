#include <ct-backend/utils/json/RoomSessionJson.h>
#include <ct-backend/utils/json/TheaterJson.h>
#include <ct-backend/utils/json/SeatJson.h>

namespace ct::model
{
    void to_json(nlohmann::json& j, const RoomSession& roomSession)
    {
        j = nlohmann::json{
            {"theater", roomSession.theater},
            {"seats", roomSession.seats}
        };
    }

    void from_json(const nlohmann::json& j, RoomSession& roomSession)
    {
        j.at("theater").get_to(roomSession.theater);
        j.at("seats").get_to(roomSession.seats);
    }
}
