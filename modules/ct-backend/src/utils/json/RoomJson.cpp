#include <ct-backend/utils/json/RoomJson.h>
#include <ct-backend/utils/json/MovieSessionJson.h>

namespace ct::model
{
    void to_json(nlohmann::ordered_json& j, const Room& room)
    {
        j = nlohmann::ordered_json{
            {"id", room.id},
            {"theaterId", room.theaterId},
            {"number", room.number},
            {"type", room.type},
            {"rows", room.rows},
            {"columns", room.columns},
            {"sessions", room.sessions}
        };
    }

    void from_json(const nlohmann::ordered_json& j, Room& room)
    {
        j.at("id").get_to(room.id);
        j.at("theaterId").get_to(room.theaterId);
        j.at("number").get_to(room.number);
        j.at("type").get_to(room.type);
        j.at("rows").get_to(room.rows);
        j.at("columns").get_to(room.columns);
        j.at("sessions").get_to(room.sessions);
    }
}
