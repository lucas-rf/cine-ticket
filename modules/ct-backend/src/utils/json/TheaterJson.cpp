#include <ct-backend/utils/json/TheaterJson.h>
#include <ct-backend/utils/json/RoomJson.h>

namespace ct::model
{
    void to_json(nlohmann::ordered_json& j, const Theater& theater)
    {
        j = nlohmann::ordered_json{
            {"id", theater.id},
            {"name", theater.name},
            {"rooms", theater.rooms}
        };
    }

    void from_json(const nlohmann::ordered_json& j, Theater& theater)
    {
        j.at("id").get_to(theater.id);
        j.at("name").get_to(theater.name);
        j.at("rooms").get_to(theater.rooms);
    }
}
