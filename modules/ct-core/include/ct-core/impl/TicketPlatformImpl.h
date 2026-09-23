#pragma once

#include <ct-core/TicketPlatform.h>
#include <ct-core/DBApi.h>
#include <ct-core/Timer.h>
#include <ct-core/EventListener.h>
#include <ct-core/impl/utils/ClockImpl.h>
#include <mutex>
#include <unordered_set>
#include <atomic>

namespace ct::impl
{
    class TicketPlatformImpl: public TicketPlatform
    {
    public:
        TicketPlatformImpl(DBApi& db, Timer& timer, EventListener* listener, const Clock& clock = ClockImpl::Instance);

        virtual void SetMovieSessionEvents(int movieSessionId, bool active) override;
        virtual void SetCartEvents(bool active) override;

        virtual model::Movie GetMovie(int movieId) const override;
        virtual model::Movie ViewMovieDetails(int movieId, int day) const override;
        virtual std::vector<model::Movie> GetAllMovies() const override;
        virtual std::vector<model::Movie> ViewMoviesByTheater(int theaterId, int day) const override;
        virtual model::RoomSession ViewRoomSessionDetails(int movieSessionId, int userKey) const override;
        virtual model::Cart ViewCartDetails(int userKey) const override;
        virtual model::Seat GetSeat(int seatId, int userKey) const override;
        virtual model::Order ViewOrderDetails(const std::string& orderKey) const override;

        virtual bool SelectSeat(int seatId, int userKey) override;
        virtual bool DeselectSeat(int seatId, int userKey) override;

        virtual std::optional<model::Order> OrderCart(int userKey, const std::string& orderKey, const std::string& userEmail) override;

    private:
        struct ActiveCart
        {
            std::mutex lock;
            bool expired;
            Timer::ID timerId;
            model::Cart dbCart;
        };

        const Clock& clock;
        DBApi& db;
        Timer& timer;
        EventListener* const listener;
        const model::PlatformSettings settings;
        mutable std::mutex cartsLock;
        std::unordered_map<int, std::shared_ptr<ActiveCart>> activeCarts;
        std::mutex eventsLock;
        std::unordered_set<int> activeEvents;
        std::atomic<bool> cartEventsActive;

        std::shared_ptr<ActiveCart> getCart(int userKey) const;
        void removeActiveCart(int userKey);
        std::shared_ptr<ActiveCart> getOrCreateCart(int userKey);

        void resetCartTimer(ActiveCart& cart);
        void cartTimerExpired(int userKey);

        inline void seatSelected(model::Seat& seat, int userKey, int cartId);
        inline void seatDeselected(model::Seat& seat, int userKey);
        inline void seatsDeselected(std::vector<model::Seat>& seats, int userKey);
        inline void seatsOrdered(std::vector<model::Seat>& seats, int userKey);

        void cartCreated(const ActiveCart& cart);
        void cartRecreated(const ActiveCart& cart);
        void cartExpired(const ActiveCart& cart);

        bool roomEventEnabled(int movieSessionId);
    };
}
