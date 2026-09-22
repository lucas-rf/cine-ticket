#pragma once

#include <ct-backend/api/Session.h>
#include <ct-core/TicketPlatform.h>
#include <random>

namespace ct::api
{
    class CommandController
    {
    public:
        CommandController(TicketPlatform& platform, CrowApp& app, Session& session);

        crow::response SelectSeat(const crow::request& request, int seatId);
        crow::response DeselectSeat(const crow::request& request, int seatId);
        crow::response OrderCart(const crow::request& request, const std::string& userEmail);

    private:
        TicketPlatform& platform;
        CrowApp& app;
        Session& session;
        std::mt19937 randGen;

        std::string generateOrderKey();
    };
}
