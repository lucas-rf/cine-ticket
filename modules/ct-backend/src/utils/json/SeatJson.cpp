#include <ct-backend/utils/json/SeatJson.h>

namespace ct::model
{
    NLOHMANN_JSON_SERIALIZE_ENUM(Seat::State, {
        {Seat::INVALID, nullptr},
        {Seat::FREE, "FREE"},
        {Seat::SELECTED_BY_CURRENT_USER, "SELECTED_BY_CURRENT_USER"},
        {Seat::SELECTED_BY_OTHER_USER, "SELECTED_BY_OTHER_USER"},
        {Seat::ORDERED, "ORDERED"},
    })

    void to_json(nlohmann::json& j, const Seat& seat)
    {
        j = nlohmann::json{
            {"id", seat.id},
            {"movieSessionId", seat.movieSessionId},
            {"row", seat.row},
            {"column", seat.column},
            {"state", seat.state},
            {"cartId", seat.cartId},
            {"orderId", seat.orderId}
        };
    }

    void from_json(const nlohmann::json& j, Seat& seat)
    {
        j.at("id").get_to(seat.id);
        j.at("movieSessionId").get_to(seat.movieSessionId);
        j.at("row").get_to(seat.row);
        j.at("column").get_to(seat.column);
        j.at("state").get_to(seat.state);
        j.at("cartId").get_to(seat.cartId);
        j.at("orderId").get_to(seat.orderId);
    }
}
