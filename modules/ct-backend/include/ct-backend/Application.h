#pragma once

#include <ct-backend/api/controllers/QueryController.h>
#include <ct-backend/api/controllers/CommandController.h>
#include <ct-backend/api/Session.h>
#include <ct-core/TicketPlatform.h>
#include <ct-core/DBApi.h>
#include <ct-core/Timer.h>
#include <ct-core/EventListener.h>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <list>
#include <mutex>

namespace ct
{
    class Application: EventListener
    {
    public:
        Application(int port, bool multithread, const std::filesystem::path& dbFilePath);
        Application(int port, bool multithread, const std::string& dbContents);

        void Run();

    private:
        using WSConn = crow::websocket::connection;
        using WSConnList = std::list<WSConn*>;
        using WSConnMap = std::unordered_map<int, WSConnList>;

        struct ConnInfo
        {
            int movieSessionId;
            int userKey;
            WSConnMap::iterator mapIter;
            WSConnList::iterator listIter;
        };

        const std::unique_ptr<Timer> timer;
        const std::unique_ptr<DBApi> db;
        const std::unique_ptr<TicketPlatform> platform;
        api::CrowApp app;
        api::Session sessionMgr;
        api::QueryController queryCtrl;
        api::CommandController cmdCtrl;
        std::mutex lock;
        std::unordered_map<ConnInfo*, std::unique_ptr<ConnInfo>> connRequests;
        WSConnMap movieSessionListeners;
        std::unordered_map<WSConn*, std::unique_ptr<ConnInfo>> movieSessionListenerIndex;

        Application(int port, bool multithread, std::unique_ptr<DBApi>&& dbinstance);

        void setupWebSockets();

        static int getMovieSesionIdFromUrl(const std::string& url);
        bool movieSessionEventWsConnAccept(const crow::request& req, void** userdata);
        void movieSessionEventWsConnOpened(crow::websocket::connection& conn);
        void movieSessionEventWsConnClosed(crow::websocket::connection& conn, const std::string& reason, uint16_t with_status_code);

        virtual void SeatSelected(const model::Seat& seat, int userKey) override;
        virtual void SeatDeselected(const model::Seat& seat, int userKey) override;
        virtual void SeatsDeselected(const std::vector<model::Seat>& seats, int userKey) override;
        virtual void SeatsOrdered(const std::vector<model::Seat>& seats, int userKey) override;
        WSConnList getConnectionList(int movieSessionId);
        void sendSeats(const std::vector<model::Seat>& seats, const WSConnList& conns);

        virtual void CartCreated(int cartId, int userKey) override;
        virtual void CartRecreated(int cartId, int userKey) override;
        virtual void CartExpired(int cartId, int userKey) override;
    };
}
