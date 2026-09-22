#include <ct-core/impl/TicketPlatformImpl.h>
#include <ct-core/utils/Exception.h>

namespace ct::impl
{
    TicketPlatformImpl::TicketPlatformImpl(DBApi& db, Timer& timer, EventListener* listener):
        db{db},
        timer{timer},
        listener{listener},
        settings{db.PlatformSettings_GetOne()}
    {
    }

    void TicketPlatformImpl::IncMovieSessionListeners(int movieSessionId)
    {
        std::lock_guard<std::mutex> guard{roomsLock};
        ++roomListeners[movieSessionId];
    }

    void TicketPlatformImpl::DecMovieSessionListeners(int movieSessionId)
    {
        std::lock_guard<std::mutex> guard{roomsLock};
        auto find = roomListeners.find(movieSessionId);
        if(find == roomListeners.end())
            return;
        --find->second;
        if(find->second == 0)
            roomListeners.erase(find);
    }

    model::Movie TicketPlatformImpl::GetMovie(int movieId) const
    {
        return db.Movie_Get(movieId);
    }

    model::Movie TicketPlatformImpl::ViewMovieDetails(int movieId, int day) const
    {
        return db.Movie_ViewDetailed(movieId, day);
    }

    std::vector<model::Movie> TicketPlatformImpl::GetAllMovies() const
    {
        return db.Movies_GetAll();
    }

    std::vector<model::Movie> TicketPlatformImpl::ViewMoviesByTheater(int theaterId, int day) const
    {
        return db.Movies_ViewByTheater(theaterId, day);
    }

    model::RoomSession TicketPlatformImpl::ViewRoomSessionDetails(int movieSessionId, int userKey) const
    {
        return db.RoomSession_ViewDetailed(movieSessionId, userKey);
    }

    model::Cart TicketPlatformImpl::ViewCartDetails(int userKey) const
    {
        auto activeCart = getCart(userKey);
        if(!activeCart)
            throw CartNotFoundException();

        std::lock_guard<std::mutex> guard{activeCart->lock};
        if(activeCart->expired)
            throw CartNotFoundException();

        return db.Cart_ViewDetailed(activeCart->dbCart.id);
    }

    model::Seat TicketPlatformImpl::GetSeat(int seatId, int userKey) const
    {
        return db.Seat_Get(seatId, userKey);
    }

    model::Order TicketPlatformImpl::ViewOrderDetails(const std::string& orderKey) const
    {
        return db.Order_ViewDetailed(orderKey);
    }

    bool TicketPlatformImpl::SelectSeat(int seatId, int userKey)
    {
        auto seat = db.Seat_Get(seatId, userKey);
        if(seat.state != model::Seat::FREE)
            return false;

        auto activeCart = getOrCreateCart(userKey);
        std::vector<model::Seat> oldSeats;

        {
            std::lock_guard<std::mutex> guard{activeCart->lock};
            if(activeCart->expired)
                return false;

            auto updatedCart = db.Cart_TryAddSeat(activeCart->dbCart.id, seatId);
            if(!updatedCart)
                return false;

            if(activeCart->dbCart.movieSessionId != updatedCart->movieSessionId)
                oldSeats = std::move(activeCart->dbCart.seats);

            activeCart->dbCart = updatedCart.value();

            if(updatedCart->seatCount == 1)
                resetCartTimer(*activeCart);
        }

        seatsUpdated(oldSeats);
        seatUpdated(seat);

        return true;
    }

    bool TicketPlatformImpl::DeselectSeat(int seatId, int userKey)
    {
        auto seat = db.Seat_Get(seatId, userKey);
        if(seat.state != model::Seat::SELECTED_BY_CURRENT_USER)
            return false;

        auto activeCart = getCart(userKey);
        if(!activeCart)
            return false;

        bool expired = false;

        {
            std::lock_guard<std::mutex> guard{activeCart->lock};
            if(activeCart->expired)
                return false;

            auto updatedCart = db.Cart_RemoveSeat(activeCart->dbCart.id, seatId);

            activeCart->dbCart = updatedCart;
            if(updatedCart.seatCount == 0)
            {
                activeCart->expired = true;
                expired = true;
                timer.Cancel(activeCart->timerId);
                db.Cart_TryDelete(activeCart->dbCart.id);
                removeActiveCart(userKey);
            }
        }

        seatUpdated(seat);
        if(expired)
            cartExpired(*activeCart);

        return true;
    }

    std::optional<model::Order> TicketPlatformImpl::OrderCart(int userKey, const std::string& orderKey, const std::string& userEmail)
    {
        auto activeCart = getCart(userKey);
        if(!activeCart)
            return {};

        std::optional<model::Order> order;

        {
            std::lock_guard<std::mutex> guard{activeCart->lock};
            if(activeCart->expired)
                return {};

            activeCart->expired = true;
            timer.Cancel(activeCart->timerId);

            order = db.Order_CreateFromCart(activeCart->dbCart.id, orderKey, userEmail);

            removeActiveCart(userKey);
        }

        seatsUpdated(activeCart->dbCart.seats);

        return order;
    }

    std::shared_ptr<TicketPlatformImpl::ActiveCart> TicketPlatformImpl::getCart(int userKey) const
    {
        std::lock_guard<std::mutex> guard{cartsLock};
        auto find = activeCarts.find(userKey);
        if(find == activeCarts.end())
            return {};
        return find->second;
    }

    void TicketPlatformImpl::removeActiveCart(int userKey)
    {
        std::lock_guard<std::mutex> guard{cartsLock};
        activeCarts.extract(userKey);
    }

    std::shared_ptr<TicketPlatformImpl::ActiveCart> TicketPlatformImpl::getOrCreateCart(int userKey)
    {
        std::lock_guard<std::mutex> guard{cartsLock};
        auto [iter, added] = activeCarts.try_emplace(userKey);
        auto& activeCart = iter->second;
        if(!activeCart)
        {
            auto [cart, created] = db.Cart_GetOrCreate(userKey);
            activeCart.reset(new ActiveCart{
                .lock{},
                .expired{false},
                .timerId{-1},
                .dbCart{std::move(cart)}
            });
        }
        return activeCart;
    }

    void TicketPlatformImpl::resetCartTimer(ActiveCart& cart)
    {
        if(cart.timerId >= 0)
            timer.Cancel(cart.timerId);

        cart.timerId = timer.Set(
            cart.dbCart.startTime + settings.cartDuration,
            std::bind(&TicketPlatformImpl::cartTimerExpired, this, cart.dbCart.userKey)
        );
    }

    void TicketPlatformImpl::cartTimerExpired(int userKey)
    {
        auto activeCart = getCart(userKey);
        if(!activeCart)
            return;

        {
            std::lock_guard<std::mutex> guard{activeCart->lock};

            if(activeCart->expired)
                return;

            if(activeCart->dbCart.startTime + settings.cartDuration > Clock::now())
                return;

            activeCart->expired = true;

            db.Cart_TryDelete(activeCart->dbCart.id);

            removeActiveCart(userKey);
        }

        seatsUpdated(activeCart->dbCart.seats);
        cartExpired(*activeCart);
    }

    void TicketPlatformImpl::seatUpdated(const model::Seat& seat)
    {
        if(listener && roomEventEnabled(seat.movieSessionId))
            listener->SeatsUpdated({seat.id});
    }

    void TicketPlatformImpl::seatsUpdated(const std::vector<model::Seat>& seats)
    {
        if(listener && !seats.empty() && roomEventEnabled(seats[0].movieSessionId))
        {
            std::vector<int> ids;
            ids.reserve(seats.size());
            for(auto& seat : seats)
                ids.push_back(seat.id);
            listener->SeatsUpdated(std::move(ids));
        }
    }

    void TicketPlatformImpl::cartExpired(const ActiveCart& cart)
    {
        if(listener)
            listener->CartExpired(cart.dbCart.id);
    }

    bool TicketPlatformImpl::roomEventEnabled(int movieSessionId)
    {
        std::lock_guard<std::mutex> guard{roomsLock};
        return roomListeners.contains(movieSessionId);
    }


}