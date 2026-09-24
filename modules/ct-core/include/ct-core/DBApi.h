#pragma once

#include <ct-core/model/PlatformSettings.h>
#include <ct-core/model/Movie.h>
#include <ct-core/model/Cart.h>
#include <ct-core/model/Booking.h>
#include <ct-core/model/RoomSession.h>
#include <vector>
#include <optional>

namespace ct
{
    class DBApi
    {
    public:
        virtual model::PlatformSettings PlatformSettings_GetOne() const = 0;

        virtual model::Movie Movie_Get(int movieId) const = 0;
        virtual model::Movie Movie_ViewDetailed(int movieId, int day) const = 0;
        virtual std::vector<model::Movie> Movies_GetAll() const = 0;
        virtual std::vector<model::Movie> Movies_ViewByTheater(int theaterId, int day) const = 0;

        virtual std::vector<model::Theater> Theaters_GetAll() const = 0;

        virtual model::RoomSession RoomSession_ViewDetailed(int movieSessionId, int userKey) const = 0;

        virtual std::pair<model::Cart, bool> Cart_GetOrCreate(int userKey) = 0;
        virtual bool Cart_TryDelete(int cartId) = 0;
        virtual std::vector<model::Cart> Carts_GetAll() const = 0;
        virtual model::Cart Cart_ViewDetailed(int cartId) const = 0;

        virtual std::optional<model::Cart> Cart_TryAddSeat(int cartId, int seatId) = 0; // Also resets start time when needed
        virtual model::Cart Cart_RemoveSeat(int cartId, int seatId) = 0;

        virtual model::Seat Seat_Get(int seatId, int userKey) const = 0;

        virtual model::Booking Booking_ViewDetailed(const std::string& bookingKey) const = 0;
        virtual model::Booking Booking_CreateFromCart(int cartId, const std::string& bookingKey, const std::string& userEmail) = 0;

        virtual ~DBApi() = default;
    };
}
