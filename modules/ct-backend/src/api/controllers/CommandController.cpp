#include <ct-backend/api/controllers/CommandController.h>
#include <ct-backend/utils/wrappers/JsonConverterWrapper.h>
#include <ct-backend/utils/json/OrderJson.h>

namespace ct::api
{
    constexpr int ORDER_KEY_SIZE = 6;

    static auto getRandomSeed()
    {
        std::random_device randDev;
        return randDev();
    }

    CommandController::CommandController(TicketPlatform& platform, CrowApp& app, Session& session):
        platform{platform},
        app{app},
        session{session},
        randGen{getRandomSeed()}
    {
        using namespace std::placeholders;

        CROW_ROUTE(app, "/seat/<int>/select").methods("POST"_method)(std::bind(&CommandController::SelectSeat, this, _1, _2));
        CROW_ROUTE(app, "/seat/<int>/deselect").methods("POST"_method)(std::bind(&CommandController::DeselectSeat, this, _1, _2));
        CROW_ROUTE(app, "/order/<string>").methods("POST"_method)(std::bind(&CommandController::OrderCart, this, _1, _2));
    }

    crow::response CommandController::SelectSeat(const crow::request& request, int seatId)
    {
        return utils::JsonConverterWrapper([this, seatId, userKey = session.GetUserKey(request)]() {
            return platform.SelectSeat(seatId, userKey);
        });
    }

    crow::response CommandController::DeselectSeat(const crow::request& request, int seatId)
    {
        try
        {
            return utils::JsonConverterWrapper([this, seatId, userKey = session.GetUserKey(request)]() {
                return platform.DeselectSeat(seatId, userKey);
            });
        }
        catch(SeatNotInCartException& exc)
        {
            return crow::response(crow::status::BAD_REQUEST, exc.what());
        }
    }

    crow::response CommandController::OrderCart(const crow::request& request, const std::string& userEmail)
    {
        auto order = platform.OrderCart(session.GetUserKey(request), generateOrderKey(), userEmail);
        nlohmann::json obj;
        if(order)
            obj = order.value();
        else
            obj = false;
        return crow::response("application/json", obj.dump());
    }

    std::string CommandController::generateOrderKey()
    {
        std::uniform_int_distribution<int> dist(0, 'Z' - 'A' + 10);
        std::string key(ORDER_KEY_SIZE, 0);
        for(int i = 0; i < ORDER_KEY_SIZE; ++i)
        {
            auto pos = dist(randGen);
            key[i] = pos + 'A' > 'Z' ? pos + '0' - 'Z' + 'A' - 1 : pos + 'A';
        }
        return key;
    }

}