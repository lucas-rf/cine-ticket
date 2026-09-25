#include <ct-backend/utils/json/SeatJson.h>

namespace ct::model
{
    NLOHMANN_JSON_SERIALIZE_ENUM(Seat::State, {
        {Seat::INVALID, nullptr},
        {Seat::FREE, "FREE"},
        {Seat::SELECTED_BY_CURRENT_USER, "SELECTED_BY_CURRENT_USER"},
        {Seat::SELECTED_BY_OTHER_USER, "SELECTED_BY_OTHER_USER"},
        {Seat::BOOKED, "BOOKED"},
    })

    void to_json(nlohmann::ordered_json& j, const Seat& seat)
    {
        j = nlohmann::ordered_json{
            {"id", seat.id},
            {"movieSessionId", seat.movieSessionId},
            {"row", seat.row},
            {"column", seat.column},
            {"state", seat.state},
            {"cartId", seat.cartId},
            {"bookingId", seat.bookingId}
        };
    }

    void from_json(const nlohmann::ordered_json& j, Seat& seat)
    {
        j.at("id").get_to(seat.id);
        j.at("movieSessionId").get_to(seat.movieSessionId);
        j.at("row").get_to(seat.row);
        j.at("column").get_to(seat.column);
        j.at("state").get_to(seat.state);
        j.at("cartId").get_to(seat.cartId);
        j.at("bookingId").get_to(seat.bookingId);
    }
}
