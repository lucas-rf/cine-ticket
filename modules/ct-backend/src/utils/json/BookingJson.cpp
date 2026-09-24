#include <ct-backend/utils/json/BookingJson.h>
#include <ct-backend/utils/json/SeatJson.h>
#include <ct-core/utils/Time.h>

namespace ct::model
{
    void to_json(nlohmann::json& j, const Booking& booking)
    {
        j = nlohmann::json{
            {"id", booking.id},
            {"movieSessionId", booking.movieSessionId},
            {"userEmail", booking.userEmail},
            {"bookingKey", booking.bookingKey},
            {"bookingTime", utils::TimePointToStr(booking.bookingTime)},
            {"seatCount", booking.seatCount},
            {"seats", booking.seats}
        };
    }

    void from_json(const nlohmann::json& j, Booking& booking)
    {
        j.at("id").get_to(booking.id);
        j.at("movieSessionId").get_to(booking.movieSessionId);
        j.at("userEmail").get_to(booking.userEmail);
        j.at("bookingKey").get_to(booking.bookingKey);
        booking.bookingTime = utils::StrToTimePoint(j.at("bookingTime"));
        j.at("seatCount").get_to(booking.seatCount);
        j.at("seats").get_to(booking.seats);
    }
}
