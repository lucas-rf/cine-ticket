#include <ct-backend/utils/json/OrderJson.h>
#include <ct-backend/utils/json/SeatJson.h>
#include <ct-core/utils/Time.h>

namespace ct::model
{
    void to_json(nlohmann::json& j, const Order& order)
    {
        j = nlohmann::json{
            {"id", order.id},
            {"movieSessionId", order.movieSessionId},
            {"userEmail", order.userEmail},
            {"orderKey", order.orderKey},
            {"orderTime", utils::TimePointToStr(order.orderTime)},
            {"seatCount", order.seatCount},
            {"seats", order.seats}
        };
    }

    void from_json(const nlohmann::json& j, Order& order)
    {
        j.at("id").get_to(order.id);
        j.at("movieSessionId").get_to(order.movieSessionId);
        j.at("userEmail").get_to(order.userEmail);
        j.at("orderKey").get_to(order.orderKey);
        order.orderTime = utils::StrToTimePoint(j.at("orderTime"));
        j.at("seatCount").get_to(order.seatCount);
        j.at("seats").get_to(order.seats);
    }
}
