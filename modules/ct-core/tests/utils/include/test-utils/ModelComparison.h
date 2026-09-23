#pragma once

#include <ct-core/model/Movie.h>
#include <ct-core/model/RoomSession.h>
#include <ct-core/model/MovieSession.h>
#include <ct-core/model/Cart.h>
#include <ct-core/model/Order.h>

namespace ct::model
{
    inline bool operator==(const Seat& left, const Seat& right)
    {
        return
            left.id == right.id &&
            left.movieSessionId == right.movieSessionId &&
            left.row == right.row &&
            left.column == right.column &&
            left.state == right.state &&
            left.cartId == right.cartId &&
            left.orderId == right.orderId;
    }

    inline bool operator==(const MovieSession& left, const MovieSession& right)
    {
        return
            left.id == right.id &&
            left.roomId == right.roomId &&
            left.day == right.day &&
            left.time == right.time &&
            left.price == right.price;
    }

    inline bool operator==(const Room& left, const Room& right)
    {
        return
            left.id == right.id &&
            left.theaterId == right.theaterId &&
            left.number == right.number &&
            left.type == right.type &&
            left.rows == right.rows &&
            left.columns == right.columns &&
            left.sessions == right.sessions;
    }

    inline bool operator==(const Theater& left, const Theater& right)
    {
        return
            left.id == right.id &&
            left.name == right.name &&
            left.rooms == right.rooms;
    }

    inline bool operator==(const Movie& left, const Movie& right)
    {
        return
            left.id == right.id &&
            left.title == right.title &&
            left.synopsis == right.synopsis &&
            left.classification == right.classification &&
            left.genre == right.genre &&
            left.coverImage == right.coverImage &&
            left.backgroundImage == right.backgroundImage &&
            left.runtime == right.runtime &&
            left.theaters == right.theaters;
    }

    inline bool operator==(const Cart& left, const Cart& right)
    {
        return
            left.id == right.id &&
            left.movieSessionId == right.movieSessionId &&
            left.userKey == right.userKey &&
            left.expirationTime == right.expirationTime &&
            left.seatCount == right.seatCount &&
            left.seats == right.seats;
    }

    inline bool operator==(const Order& left, const Order& right)
    {
        return
            left.id == right.id &&
            left.movieSessionId == right.movieSessionId &&
            left.userEmail == right.userEmail &&
            left.orderKey == right.orderKey &&
            left.orderTime == right.orderTime &&
            left.seatCount == right.seatCount &&
            left.seats == right.seats;
    }
}
