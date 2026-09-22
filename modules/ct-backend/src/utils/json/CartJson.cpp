#include <ct-backend/utils/json/CartJson.h>
#include <ct-backend/utils/json/SeatJson.h>
#include <ct-core/utils/Time.h>

namespace ct::model
{
    void to_json(nlohmann::json& j, const Cart& cart)
    {
        j = nlohmann::json{
            {"id", cart.id},
            {"movieSessionId", cart.movieSessionId},
            {"expirationTime", utils::TimePointToStr(cart.expirationTime)},
            {"seatCount", cart.seatCount},
            {"seats", cart.seats}
        };
    }

    void from_json(const nlohmann::json& j, Cart& cart)
    {
        j.at("id").get_to(cart.id);
        j.at("movieSessionId").get_to(cart.movieSessionId);
        cart.expirationTime = utils::StrToTimePoint(j.at("expirationTime"));
        j.at("seatCount").get_to(cart.seatCount);
        j.at("seats").get_to(cart.seats);
        cart.userKey = -1;
    }
}
