#include <ct-core/impl/TicketPlatformImpl.h>
#include <ct-core/utils/Exception.h>
#include <algorithm>

namespace ct::impl
{
    TicketPlatformImpl::TicketPlatformImpl(DBApi& db, Timer& timer, EventListener* listener, const Clock& clock):
        clock{clock},
        db{db},
        timer{timer},
        listener{listener},
        settings{db.PlatformSettings_GetOne()},
        cartEventsActive{false}
    { }

    void TicketPlatformImpl::SetMovieSessionEvents(int movieSessionId, bool active)
    {
        if(!listener)
            return;
        std::lock_guard<std::mutex> guard{eventsLock};
        if(active)
            activeEvents.insert(movieSessionId);
        else
            activeEvents.erase(movieSessionId);
    }

    void TicketPlatformImpl::SetCartEvents(bool active)
    {
        if(!listener)
            return;
        cartEventsActive.store(active);
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

    model::Booking TicketPlatformImpl::ViewBookingDetails(const std::string& bookingKey) const
    {
        return db.Booking_ViewDetailed(bookingKey);
    }

    bool TicketPlatformImpl::SelectSeat(int seatId, int userKey)
    {
        auto seat = db.Seat_Get(seatId, userKey);
        if(seat.state != model::Seat::FREE)
            return false;

        auto activeCart = getOrCreateCart(userKey);
        bool cartIsNew = false;
        std::vector<model::Seat> oldSeats;

        {
            std::lock_guard<std::mutex> guard{activeCart->lock};
            if(activeCart->expired)
                return false;

            if(activeCart->dbCart.seatCount == 0)
                cartIsNew = true;

            auto updatedCart = db.Cart_TryAddSeat(activeCart->dbCart.id, seatId);
            if(!updatedCart)
                return false;

            if(activeCart->dbCart.movieSessionId != updatedCart->movieSessionId)
                oldSeats = std::move(activeCart->dbCart.seats);

            activeCart->dbCart = updatedCart.value();

            if(updatedCart->seatCount == 1)
                resetCartTimer(*activeCart);
        }

        if(cartIsNew)
        {
            cartCreated(*activeCart);
        }
        else if(!oldSeats.empty())
        {
            seatsDeselected(oldSeats, userKey);
            cartRecreated(*activeCart);
        }

        seatSelected(seat, userKey, activeCart->dbCart.id);

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

        seatDeselected(seat, userKey);
        if(expired)
            cartExpired(*activeCart);

        return true;
    }

    std::optional<model::Booking> TicketPlatformImpl::BookCart(int userKey, const std::string& bookingKey, const std::string& userEmail)
    {
        auto activeCart = getCart(userKey);
        if(!activeCart)
            return {};

        std::optional<model::Booking> booking;

        {
            std::lock_guard<std::mutex> guard{activeCart->lock};
            if(activeCart->expired)
                return {};

            activeCart->expired = true;
            timer.Cancel(activeCart->timerId);

            booking = db.Booking_CreateFromCart(activeCart->dbCart.id, bookingKey, userEmail);

            removeActiveCart(userKey);
        }

        seatsBooked(booking->seats, userKey);

        return booking;
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
            cart.dbCart.expirationTime,
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

            if(activeCart->dbCart.expirationTime > clock.now())
                return;

            activeCart->expired = true;

            db.Cart_TryDelete(activeCart->dbCart.id);

            removeActiveCart(userKey);
        }

        seatsDeselected(activeCart->dbCart.seats, userKey);
        cartExpired(*activeCart);
    }

    void TicketPlatformImpl::seatSelected(model::Seat& seat, int userKey, int cartId)
    {
        if(roomEventEnabled(seat.movieSessionId))
        {
            seat.cartId = cartId;
            seat.state = model::Seat::SELECTED_BY_CURRENT_USER;
            listener->SeatSelected(seat, userKey);
        }
    }

    void TicketPlatformImpl::seatDeselected(model::Seat& seat, int userKey)
    {
        if(roomEventEnabled(seat.movieSessionId))
        {
            seat.cartId = -1;
            seat.state = model::Seat::FREE;
            listener->SeatDeselected(seat, userKey);
        }
    }

    void TicketPlatformImpl::seatsDeselected(std::vector<model::Seat>& seats, int userKey)
    {
        if(!seats.empty() && roomEventEnabled(seats[0].movieSessionId))
        {
            for(auto& seat : seats)
            {
                seat.cartId = -1;
                seat.state = model::Seat::FREE;
            }
            listener->SeatsDeselected(seats, userKey);
        }
    }

    void TicketPlatformImpl::seatsBooked(std::vector<model::Seat>& seats, int userKey)
    {
        if(roomEventEnabled(seats[0].movieSessionId))
            listener->SeatsBooked(seats, userKey);
    }

    void TicketPlatformImpl::cartCreated(const ActiveCart& cart)
    {
        if(cartEventsActive.load())
            listener->CartCreated(cart.dbCart.id, cart.dbCart.userKey);
    }

    void TicketPlatformImpl::cartRecreated(const ActiveCart& cart)
    {
        if(cartEventsActive.load())
            listener->CartRecreated(cart.dbCart.id, cart.dbCart.userKey);
    }

    void TicketPlatformImpl::cartExpired(const ActiveCart& cart)
    {
        if(cartEventsActive.load())
            listener->CartExpired(cart.dbCart.id, cart.dbCart.userKey);
    }

    bool TicketPlatformImpl::roomEventEnabled(int movieSessionId)
    {
        std::lock_guard<std::mutex> guard{eventsLock};
        return activeEvents.contains(movieSessionId);
    }

}