#pragma once

#include <ct-core/model/Movie.h>
#include <ct-core/model/Cart.h>
#include <ct-core/model/Order.h>
#include <ct-core/model/RoomSession.h>
#include <vector>
#include <optional>

namespace ct
{
    class TicketPlatform
    {
    public:
        virtual void SetMovieSessionEvents(int movieSessionId, bool active) = 0;
        virtual void SetCartEvents(bool active) = 0;

        virtual model::Movie GetMovie(int movieId) const = 0;
        virtual model::Movie ViewMovieDetails(int movieId, int day) const = 0;
        virtual std::vector<model::Movie> GetAllMovies() const = 0;
        virtual std::vector<model::Movie> ViewMoviesByTheater(int theaterId, int day) const = 0;
        virtual model::RoomSession ViewRoomSessionDetails(int movieSessionId, int userKey) const = 0;
        virtual model::Cart ViewCartDetails(int userKey) const = 0;
        virtual model::Seat GetSeat(int seatId, int userKey) const = 0;
        virtual model::Order ViewOrderDetails(const std::string& orderKey) const = 0;

        virtual bool SelectSeat(int seatId, int userKey) = 0;
        virtual bool DeselectSeat(int seatId, int userKey) = 0;
        virtual std::optional<model::Order> OrderCart(int userKey, const std::string& orderKey, const std::string& userEmail) = 0;

        virtual ~TicketPlatform() = default;
    };
}
