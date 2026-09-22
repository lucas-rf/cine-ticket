#pragma once

#include <ct-backend/api/Session.h>
#include <ct-core/TicketPlatform.h>

namespace ct::api
{
    class QueryController
    {
    public:
        QueryController(TicketPlatform& platform, CrowApp& app, Session& session);

        crow::response Movie_Get(const crow::request& request, int movieId) const;
        crow::response Movie_ViewDetailed(const crow::request& request, int movieId, int day) const;
        crow::response Movies_GetAll(const crow::request& request) const;
        crow::response Movies_ViewAllByTheater(const crow::request& request, int theaterId, int day) const;
        crow::response RoomSession_ViewDetailed(const crow::request& request, int movieSessionId) const;
        crow::response Cart_ViewDetailed(const crow::request& request) const;
        crow::response Seat_Get(const crow::request& request, int seatId) const;
        crow::response Order_ViewDetailed(const crow::request& request, const std::string& orderKey) const;

    private:
        TicketPlatform& platform;
        CrowApp& app;
        Session& session;
    };
}
