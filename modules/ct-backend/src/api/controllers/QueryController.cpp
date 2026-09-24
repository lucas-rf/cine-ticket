#include <ct-backend/api/controllers/QueryController.h>
#include <ct-backend/utils/json/CartJson.h>
#include <ct-backend/utils/json/MovieJson.h>
#include <ct-backend/utils/json/MovieSessionJson.h>
#include <ct-backend/utils/json/BookingJson.h>
#include <ct-backend/utils/json/RoomJson.h>
#include <ct-backend/utils/json/RoomSessionJson.h>
#include <ct-backend/utils/json/SeatJson.h>
#include <ct-backend/utils/json/TheaterJson.h>
#include <ct-backend/utils/wrappers/JsonConverterWrapper.h>

namespace ct::api
{
    QueryController::QueryController(TicketPlatform& platform, CrowApp& app, Session& session) :
        platform{platform},
        app{app},
        session{session}
    {
        using namespace std::placeholders;

        CROW_ROUTE(app, "/movie/<int>")(std::bind(&QueryController::Movie_Get, this, _1, _2));
        CROW_ROUTE(app, "/movie/<int>/<int>")(std::bind(&QueryController::Movie_ViewDetailed, this, _1, _2, _3));
        CROW_ROUTE(app, "/movies")(std::bind(&QueryController::Movies_GetAll, this, _1));
        CROW_ROUTE(app, "/movies/<int>/<int>")(std::bind(&QueryController::Movies_ViewAllByTheater, this, _1, _2, _3));
        CROW_ROUTE(app, "/theaters")(std::bind(&QueryController::Theaters_GetAll, this, _1));
        CROW_ROUTE(app, "/roomSession/<int>")(std::bind(&QueryController::RoomSession_ViewDetailed, this, _1, _2));
        CROW_ROUTE(app, "/cart")(std::bind(&QueryController::Cart_ViewDetailed, this, _1));
        CROW_ROUTE(app, "/seat/<int>")(std::bind(&QueryController::Seat_Get, this, _1, _2));
        CROW_ROUTE(app, "/booking/<string>")(std::bind(&QueryController::Booking_ViewDetailed, this, _1, _2));
    }

    crow::response QueryController::Movie_Get(const crow::request& request, int movieId) const
    {
        return utils::JsonConverterWrapper([this, movieId]() { return platform.GetMovie(movieId); });
    }

    crow::response QueryController::Movie_ViewDetailed(const crow::request& request, int movieId, int day) const
    {
        return utils::JsonConverterWrapper([this, movieId, day]() { return platform.ViewMovieDetails(movieId, day); });
    }

    crow::response QueryController::Movies_GetAll(const crow::request& request) const
    {
        return utils::JsonConverterWrapper([this]() { return platform.GetAllMovies(); });
    }

    crow::response QueryController::Movies_ViewAllByTheater(const crow::request& request, int theaterId, int day) const
    {
        return utils::JsonConverterWrapper([this, theaterId, day]() { return platform.ViewMoviesByTheater(theaterId, day); });
    }

    crow::response QueryController::Theaters_GetAll(const crow::request& request) const
    {
        return utils::JsonConverterWrapper([this]() { return platform.GetAllTheaters(); });
    }

    crow::response QueryController::RoomSession_ViewDetailed(const crow::request& request, int movieSessionId) const
    {
        return utils::JsonConverterWrapper([this, movieSessionId, userKey = session.GetUserKey(request)]() {
            return platform.ViewRoomSessionDetails(movieSessionId, userKey);
        });
    }

    crow::response QueryController::Cart_ViewDetailed(const crow::request& request) const
    {
        return utils::JsonConverterWrapper([this, userKey = session.GetUserKey(request)]() {
            return platform.ViewCartDetails(userKey);
        });
    }

    crow::response QueryController::Seat_Get(const crow::request& request, int seatId) const
    {
        std::string cookie_header = request.get_header_value("Cookie");

        return utils::JsonConverterWrapper([this, seatId, userKey = session.GetUserKey(request)]() {
            return platform.GetSeat(seatId, userKey);
        });
    }

    crow::response QueryController::Booking_ViewDetailed(const crow::request& request, const std::string& bookingKey) const
    {
        return utils::JsonConverterWrapper([this, bookingKey]() { return platform.ViewBookingDetails(bookingKey); });
    }

}
