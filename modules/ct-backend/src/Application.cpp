#include <ct-backend/Application.h>
#include <ct-core/impl/TicketPlatformImplBuilder.h>
#include <ct-core/impl/TimerImplBuilder.h>
#include <ct-core/impl/ramdb/RAMDatabaseBuilder.h>
#include <ct-backend/utils/json/SeatJson.h>
#include <regex>

namespace ct
{
    Application::Application(int port, bool multithread, const std::filesystem::path& dbFilePath) :
        Application(port, multithread, impl::RAMDatabaseBuilder::Instance.CreateDBApi(dbFilePath))
    { }

    Application::Application(int port, bool multithread, const std::string& dbContents) :
        Application(port, multithread, impl::RAMDatabaseBuilder::Instance.CreateDBApi(dbContents))
    { }

    void Application::Run()
    {
        app.run();
    }

    Application::Application(int port, bool multithread, std::unique_ptr<DBApi>&& dbinstance) :
        timer{impl::TimerImplBuilder::Instance.CreateTimer()},
        db{std::move(dbinstance)},
        platform{impl::TicketPlatformImplBuilder::Instance.CreateTicketPlatform(*db, *timer, this)},
        app{ct::api::CrowSession{
            crow::CookieParser::Cookie("session").max_age(24 * 60 * 60).path("/"),
            8,
            crow::InMemoryStore{}
        }},
        sessionMgr{app},
        queryCtrl{*platform, app, sessionMgr},
        cmdCtrl{*platform, app, sessionMgr}
    {
        app.port(port);
        if(multithread)
            app.multithreaded();
        setupWebSockets();
    }

    void Application::setupWebSockets()
    {
        using namespace std::placeholders;

        CROW_WEBSOCKET_ROUTE(app, "/movieSession/<int>/events")
            .onaccept(std::bind(&Application::movieSessionEventWsConnAccept, this, _1, _2))
            .onopen(std::bind(&Application::movieSessionEventWsConnOpened, this, _1))
            .onclose(std::bind(&Application::movieSessionEventWsConnClosed, this, _1, _2, _3));
    }

    int Application::getMovieSesionIdFromUrl(const std::string& url)
    {
        static const std::regex pattern(R"(^\/movieSession\/([0-9]+)\/events$)");
        std::smatch matches;
        std::regex_search(url, matches, pattern);
        return std::stoi(matches[1].str());
    }

    bool Application::movieSessionEventWsConnAccept(const crow::request& req, void** userdata)
    {
        std::string cookie_header = req.get_header_value("Cookie");

        auto movieSessionId = getMovieSesionIdFromUrl(req.url);
        auto userKey = sessionMgr.GetUserKey(req);

        std::lock_guard<std::mutex> guard{lock};
        auto connReq = std::make_unique<ConnInfo>(movieSessionId, userKey);
        *userdata = connReq.get();
        connRequests.emplace(connReq.get(), std::move(connReq));
        return true;
    }

    void Application::movieSessionEventWsConnOpened(crow::websocket::connection& conn)
    {
        std::lock_guard<std::mutex> guard{lock};

        auto connReqNode = connRequests.extract(static_cast<ConnInfo*>(conn.userdata()));
        conn.userdata(nullptr);
        auto& connInfo = connReqNode.mapped();

        auto [mslIter, added] = movieSessionListeners.try_emplace(connInfo->movieSessionId);
        auto msConnIter = mslIter->second.insert(mslIter->second.end(), &conn);

        connInfo->mapIter = mslIter;
        connInfo->listIter = msConnIter;
        auto movieSessionId = connInfo->movieSessionId;

        movieSessionListenerIndex.try_emplace(&conn, std::move(connInfo));

        if(mslIter->second.size() == 1)
            platform->SetMovieSessionEvents(movieSessionId, true);
    }

    void Application::movieSessionEventWsConnClosed(crow::websocket::connection& conn, const std::string& reason, uint16_t with_status_code)
    {
        std::lock_guard<std::mutex> guard{lock};
        auto itersNode = movieSessionListenerIndex.extract(&conn);
        auto& connList = itersNode.mapped()->mapIter->second;
        connList.erase(itersNode.mapped()->listIter);
        if(connList.empty())
        {
            auto movieSessionId = itersNode.mapped()->mapIter->first;
            platform->SetMovieSessionEvents(movieSessionId, false);
            movieSessionListeners.erase(itersNode.mapped()->mapIter);
        }
    }

    void Application::SeatSelected(const model::Seat& seat, int userKey)
    {
        WSConnList currentUserConns;
        WSConnList otherUsersConns;

        {
            std::lock_guard<std::mutex> guard{lock};
            auto iter = movieSessionListeners.find(seat.movieSessionId);
            if(iter == movieSessionListeners.end())
                return;
            for(auto conn : iter->second)
            {
                if(movieSessionListenerIndex.at(conn)->userKey == userKey)
                    currentUserConns.push_back(conn);
                else
                    otherUsersConns.push_back(conn);
            }
        }

        if(!currentUserConns.empty())
        {
            std::vector<model::Seat> seats{seat};
            sendSeats(seats, currentUserConns);
        }

        if(!otherUsersConns.empty())
        {
            std::vector<model::Seat> seats{seat};
            seats[0].state = model::Seat::SELECTED_BY_OTHER_USER;
            sendSeats(seats, otherUsersConns);
        }
    }

    void Application::SeatDeselected(const model::Seat& seat, int userKey)
    {
        sendSeats({seat}, getConnectionList(seat.movieSessionId));
    }

    void Application::SeatsDeselected(const std::vector<model::Seat>& seats, int userKey)
    {
        sendSeats(seats, getConnectionList(seats[0].movieSessionId));
    }

    void Application::SeatsOrdered(const std::vector<model::Seat>& seats, int userKey)
    {
        sendSeats(seats, getConnectionList(seats[0].movieSessionId));
    }

    Application::WSConnList Application::getConnectionList(int movieSessionId)
    {
        std::lock_guard<std::mutex> guard{lock};
        auto iter = movieSessionListeners.find(movieSessionId);
        if(iter != movieSessionListeners.end())
            return iter->second;
        return {};
    }

    void Application::sendSeats(const std::vector<model::Seat>& seats, const WSConnList& conns)
    {
        nlohmann::json obj(seats);
        auto text = obj.dump();

        for(auto conn : conns)
        {
            std::lock_guard<std::mutex> guard{lock};
            if(movieSessionListenerIndex.contains(conn))
                conn->send_text(text);
        }
    }

    void Application::CartCreated(int cartId, int userKey)
    {
        // To be implemented in the future
    }

    void Application::CartRecreated(int cartId, int userKey)
    {
        // To be implemented in the future
    }

    void Application::CartExpired(int cartId, int userKey)
    {
        // To be implemented in the future
    }

}
