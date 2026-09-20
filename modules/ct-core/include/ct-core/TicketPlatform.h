#pragma once

#include <ct-core/model/Movie.h>
#include <ct-core/model/Cart.h>
#include <ct-core/model/Order.h>
#include <vector>
#include <optional>

namespace ct
{
    class TicketPlatform
    {
    public:
        virtual void IncMovieSessionListeners(int movieSessionId) = 0;
        virtual void DecMovieSessionListeners(int movieSessionId) = 0;

        virtual model::Movie GetMovie(int movieId) const = 0;
        virtual model::Movie ViewMovieDetails(int movieId, int day) const = 0;
        virtual std::vector<model::Movie> GetAllMovies() const = 0;
        virtual std::vector<model::Movie> ViewMoviesByTheater(int theaterId, int day) const = 0;

        virtual bool SelectSeat(int seatId, int userKey) = 0;
        virtual bool UnselectSeat(int seatId, int userKey) = 0;

        virtual std::optional<model::Order> OrderCart(int userKey, const std::string& orderKey, const std::string& userEmail) = 0;

        virtual ~TicketPlatform() = 0 {}
    };
}
