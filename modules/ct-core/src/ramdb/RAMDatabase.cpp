#include <ct-core/impl/ramdb/RAMDatabase.h>
#include <ct-core/utils/Exception.h>
#include <nlohmann/json.hpp>
#include <algorithm>

namespace ct::impl
{
    RAMDatabase::RAMDatabase(const std::filesystem::path& dataFilePath, const Clock& clock):
        clock{clock},
        data(dataFilePath),
        nextBookingId{static_cast<int>(data.bookings.size())}
    { }

    RAMDatabase::RAMDatabase(const std::string& dataContents, const Clock& clock):
        clock{clock},
        data(dataContents),
        nextBookingId{static_cast<int>(data.bookings.size())}
    { }

    model::PlatformSettings RAMDatabase::PlatformSettings_GetOne() const
    {
        return data.settings;
    }

    model::Movie RAMDatabase::Movie_Get(int movieId) const
    {
        if(movieId < 0 || movieId >= data.movies.size())
            throw MovieNotFoundException(movieId);
        return generateModelMovie(data.movies[movieId], {});
    }

    model::Movie RAMDatabase::Movie_ViewDetailed(int movieId, int day) const
    {
        if(movieId < 0 || movieId >= data.movies.size())
            throw MovieNotFoundException(movieId);

        auto& movie = data.movies[movieId];

        std::vector<model::Theater> theaters;
        if(day >= 0 && day < movie.idxDaysToSessions.size())
        {
            std::unordered_map<int, std::unordered_map<int, std::vector<int>>> theatersToRoomToSessions;

            for(auto sessionId : movie.idxDaysToSessions[day])
            {
                auto& session = data.movieSessions[sessionId];
                auto& room = data.rooms[session.roomId];
                theatersToRoomToSessions[room.theaterId][session.roomId].push_back(sessionId);
            }

            theaters = expandTheaters(theatersToRoomToSessions);
        }

        return generateModelMovie(movie, std::move(theaters));
    }

    std::vector<model::Movie> RAMDatabase::Movies_GetAll() const
    {
        std::vector<model::Movie> moviesVec;
        moviesVec.reserve(data.movies.size());
        for(int movieId = 0; movieId < data.movies.size(); ++movieId)
            moviesVec.push_back(generateModelMovie(data.movies[movieId], {}));
        return moviesVec;
    }

    std::vector<model::Movie> RAMDatabase::Movies_ViewByTheater(int theaterId, int day) const
    {
        if(theaterId < 0 || theaterId >= data.theaters.size())
            throw TheaterNotFoundException(theaterId);

        std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>> auxMoviesToTheatersToRoomsToSessions;

        auto& index = data.theaters[theaterId].idxMoviesToDaysToSessions;
        for(int movieId = 0; movieId < index.size(); ++movieId)
        {
            auto& daysToSessions = index[movieId];
            if(day < 0 || day >= daysToSessions.size())
                continue;

            auto& auxTheatersToRoomsToSessions = auxMoviesToTheatersToRoomsToSessions[movieId];
            for(auto sessionId : daysToSessions[day])
            {
                auto& session = data.movieSessions[sessionId];
                auto& room = data.rooms[session.roomId];
                auxTheatersToRoomsToSessions[room.theaterId][session.roomId].push_back(sessionId);
            }
        }

        std::vector<model::Movie> moviesVec;
        moviesVec.reserve(auxMoviesToTheatersToRoomsToSessions.size());
        for(auto& [movieId, theatersToRoomsToSessions] : auxMoviesToTheatersToRoomsToSessions)
            moviesVec.push_back(generateModelMovie(data.movies[movieId], expandTheaters(theatersToRoomsToSessions)));

        return moviesVec;
    }

    std::vector<model::Theater> RAMDatabase::Theaters_GetAll() const
    {
        std::vector<model::Theater> theaters;
        theaters.reserve(data.theaters.size());
        for(auto& theater : data.theaters)
            theaters.emplace_back(theater.id, theater.name);
        return theaters;
    }

    model::RoomSession RAMDatabase::RoomSession_ViewDetailed(int movieSessionId, int userKey) const
    {
        if(movieSessionId < 0 || movieSessionId >= data.movieSessions.size())
            throw MovieSessionNotFoundException(movieSessionId);

        auto& session = data.movieSessions[movieSessionId];
        auto& room = data.rooms[session.roomId];
        auto& theater = data.theaters[room.theaterId];
        return model::RoomSession{
            session.id,
            session.day,
            session.time,
            session.price,
            model::Theater{theater.id, theater.name},
            generateRoomModel(room, {}),
            generateModelMovie(data.movies[session.movieId], {}),
            generateSeatModels(session.startingSeatId, session.startingSeatId + room.rows * room.columns, userKey)
        };
    }

    std::pair<model::Cart, bool> RAMDatabase::Cart_GetOrCreate(int userKey)
    {
        std::lock_guard<std::mutex> guard{cartLock};

        bool created = false;
        auto iter = cartsByUserKey.find(userKey);
        if(iter == cartsByUserKey.end())
        {
            auto cartId = nextCartId++;
            auto [idIter, addedById] = cartsById.try_emplace(cartId, cartId, -1, userKey, clock.now() + data.settings.cartDuration);
            auto [keyIter, addedByKey] = cartsByUserKey.try_emplace(userKey, &idIter->second);
            iter = keyIter;
            created = true;
        }

        return {generateModelCart(*iter->second, true), created};
    }

    bool RAMDatabase::Cart_TryDelete(int cartId)
    {
        std::lock_guard<std::mutex> guard{cartLock};

        auto ext = cartsById.extract(cartId);
        if(ext.empty())
            return false;

        auto& cart = ext.mapped();
        cartsByUserKey.extract(cart.userKey);

        for(auto seatId : cart.idxSeats)
            data.seats[seatId].cartId = -1;

        return true;
    }

    std::vector<model::Cart> RAMDatabase::Carts_GetAll() const
    {
        std::vector<model::Cart> cartsVec;
        std::lock_guard<std::mutex> guard{cartLock};
        cartsVec.reserve(cartsById.size());
        for(auto& [cartId, cart] : cartsById)
            cartsVec.push_back(generateModelCart(cart, false));
        return cartsVec;
    }

    model::Cart RAMDatabase::Cart_ViewDetailed(int cartId) const
    {
        std::lock_guard<std::mutex> guard{cartLock};
        auto iter = cartsById.find(cartId);
        if(iter == cartsById.end())
            throw CartNotFoundException(cartId);
        return generateModelCart(iter->second, true);
    }

    std::optional<model::Cart> RAMDatabase::Cart_TryAddSeat(int cartId, int seatId)
    {
        if(seatId < 0 || seatId >= data.seats.size())
            throw SeatNotFoundException(seatId);

        std::lock_guard<std::mutex> guard{cartLock};

        auto find = cartsById.find(cartId);
        if(find == cartsById.end())
            throw CartNotFoundException(cartId);

        auto& cart = find->second;
        auto& seat = data.seats[seatId];

        if(seat.cartId >= 0 || seat.bookingId >= 0)
            return {};

        if(cart.movieSessionId >= 0 && seat.movieSessionId != cart.movieSessionId)
        {
            for(auto seatId : cart.idxSeats)
                data.seats[seatId].cartId = -1;
            cart.idxSeats.clear();
        }

        if(cart.idxSeats.empty())
            cart.expirationTime = clock.now() + data.settings.cartDuration;

        seat.cartId = cart.id;
        seat.userKey = cart.userKey;
        cart.idxSeats.insert(seatId);
        cart.movieSessionId = seat.movieSessionId;

        return generateModelCart(cart, true);
    }

    model::Cart RAMDatabase::Cart_RemoveSeat(int cartId, int seatId)
    {
        if(seatId < 0 || seatId >= data.seats.size())
            throw SeatNotFoundException(seatId);

        std::lock_guard<std::mutex> guard{cartLock};

        auto find = cartsById.find(cartId);
        if(find == cartsById.end())
            throw CartNotFoundException(cartId);

        auto& cart = find->second;
        auto& seat = data.seats[seatId];

        if(seat.cartId != cartId)
            throw SeatNotInCartException(cartId, seatId);

        seat.cartId = -1;
        seat.userKey = -1;

        cart.idxSeats.extract(seatId);
        if(cart.idxSeats.empty())
            cart.movieSessionId = -1;

        return generateModelCart(cart, true);
    }

    model::Seat RAMDatabase::Seat_Get(int seatId, int userKey) const
    {
        if(seatId < 0 || seatId >= data.seats.size())
            throw SeatNotFoundException(seatId);

        std::lock_guard<std::mutex> guard{cartLock};
        return generateSeatModel(data.seats[seatId], userKey);
    }

    model::Booking RAMDatabase::Booking_ViewDetailed(const std::string& bookingKey) const
    {
        const RAMBooking* booking;

        {
            std::lock_guard<std::mutex> guard{cartLock};
            auto iter = data.bookings.find(bookingKey);
            if(iter == data.bookings.end())
                throw BookingNotFoundException(bookingKey);
            booking = &iter->second;
        }

        return generateBookingModel(*booking);
    }

    model::Booking RAMDatabase::Booking_CreateFromCart(int cartId, const std::string& bookingKey, const std::string& userEmail)
    {
        RAMBooking* booking;

        {
            std::lock_guard<std::mutex> guard{cartLock};

            auto find = cartsById.find(cartId);
            if(find == cartsById.end())
                throw CartNotFoundException(cartId);

            auto& cart = find->second;
            auto emp = data.bookings.try_emplace(bookingKey, nextBookingId++, cart.movieSessionId, userEmail, bookingKey, clock.now());
            booking = &emp.first->second;

            booking->seats.reserve(cart.idxSeats.size());
            for(auto seatId : cart.idxSeats)
            {
                auto& seat = data.seats[seatId];
                seat.cartId = -1;
                seat.userKey = -1;
                seat.bookingId = booking->id;
                booking->seats.push_back(seatId);
            }
            std::sort(booking->seats.begin(), booking->seats.end());

            cartsByUserKey.erase(cart.userKey);
            cartsById.erase(cartId);
        }

        return generateBookingModel(*booking);
    }

    std::vector<model::Theater> RAMDatabase::expandTheaters(const std::unordered_map<int, std::unordered_map<int, std::vector<int>>>& theatersToRoomsToSessions) const
    {
        std::vector<model::Theater> theatersVec;

        theatersVec.reserve(theatersToRoomsToSessions.size());
        for(auto& [theaterId, roomsToSessions] : theatersToRoomsToSessions)
        {
            theatersVec.emplace_back(model::Theater{theaterId, data.theaters[theaterId].name});
            auto& theaterRooms = theatersVec.back().rooms;

            theaterRooms.reserve(roomsToSessions.size());
            for(auto& [roomId, sessionsIds] : roomsToSessions)
            {
                auto& room = data.rooms[roomId];
                theaterRooms.push_back(generateRoomModel(room, sessionsIds));
            }
        }

        return theatersVec;
    }

    model::Cart RAMDatabase::generateModelCart(const RAMCart& ramCart, bool includeSeats) const
    {
        auto genSeats = [this, &ramCart]() -> decltype(model::Cart::seats)
        {
            std::vector<int> seatsIds(ramCart.idxSeats.begin(), ramCart.idxSeats.end());
            std::sort(seatsIds.begin(), seatsIds.end());
            return generateSeatModels(seatsIds, ramCart.userKey);
        };

        return model::Cart{
            ramCart.id,
            ramCart.movieSessionId,
            ramCart.userKey,
            ramCart.expirationTime,
            static_cast<int>(ramCart.idxSeats.size()),
            includeSeats ? genSeats() : decltype(model::Cart::seats){}
        };
    }

    model::Room RAMDatabase::generateRoomModel(const RAMRoom& ramRoom, const std::vector<int>& sessionsIds) const
    {
        std::vector<model::MovieSession> roomSessions;

        roomSessions.reserve(sessionsIds.size());
        for(auto sessionId : sessionsIds)
        {
            auto& session = data.movieSessions[sessionId];
            roomSessions.push_back(model::MovieSession{
                sessionId,
                ramRoom.id,
                session.movieId,
                session.day,
                session.time,
                session.price
            });
        }

        return model::Room{
            ramRoom.id,
            ramRoom.theaterId,
            ramRoom.number,
            ramRoom.type,
            ramRoom.rows,
            ramRoom.columns,
            std::move(roomSessions)
        };
    }

    model::Booking RAMDatabase::generateBookingModel(const RAMBooking& ramBooking) const
    {
        return model::Booking{
            ramBooking.id,
            ramBooking.movieSessionId,
            ramBooking.userEmail,
            ramBooking.bookingKey,
            ramBooking.bookingTime,
            static_cast<int>(ramBooking.seats.size()),
            generateSeatModels(ramBooking.seats, -1)
        };
    }

    model::Movie RAMDatabase::generateModelMovie(const RAMMovie& ramMovie, std::vector<model::Theater>&& theaters)
    {
        return model::Movie{
            ramMovie.id,
            ramMovie.title,
            ramMovie.synopsis,
            ramMovie.classification,
            ramMovie.genre,
            ramMovie.coverImage,
            ramMovie.backgroundImage,
            ramMovie.runtime,
            std::move(theaters)
        };
    }

    model::Seat RAMDatabase::generateSeatModel(const RAMSeat& ramSeat, int userKey)
    {
        model::Seat::State state = ramSeat.bookingId >= 0 ?
            model::Seat::BOOKED :
            (ramSeat.cartId >= 0 ?
                (ramSeat.userKey == userKey ?
                    model::Seat::SELECTED_BY_CURRENT_USER :
                    model::Seat::SELECTED_BY_OTHER_USER) :
                model::Seat::FREE);

        return model::Seat{
            ramSeat.id,
            ramSeat.movieSessionId,
            ramSeat.row,
            ramSeat.column,
            state,
            ramSeat.cartId,
            ramSeat.bookingId,
        };
    }

    std::vector<model::Seat> RAMDatabase::generateSeatModels(const std::vector<int>& seatsIds, int userKey) const
    {
        std::vector<model::Seat> seatsVec;
        seatsVec.reserve(seatsIds.size());
        for(auto seatId : seatsIds)
            seatsVec.push_back(generateSeatModel(data.seats[seatId], userKey));
        return seatsVec;
    }

    std::vector<model::Seat> RAMDatabase::generateSeatModels(int beginId, int endId, int userKey) const
    {
        std::vector<model::Seat> seatsVec;
        seatsVec.reserve(endId - beginId);
        for(int seatId = beginId; seatId < endId; ++seatId)
            seatsVec.push_back(generateSeatModel(data.seats[seatId], userKey));
        return seatsVec;
    }


}