#pragma once

#include <ct-core/model/Seat.h>
#include <vector>

namespace ct
{
    class EventListener
    {
    public:
        virtual void SeatSelected(const model::Seat& seat, int userKey) = 0;
        virtual void SeatDeselected(const model::Seat& seat, int userKey) = 0;
        virtual void SeatsDeselected(const std::vector<model::Seat>& seats, int userKey) = 0;
        virtual void SeatsBooked(const std::vector<model::Seat>& seats, int userKey) = 0;
        virtual void CartCreated(int cartId, int userKey) = 0;
        virtual void CartRecreated(int cartId, int userKey) = 0;
        virtual void CartExpired(int cartId, int userKey) = 0;

        virtual ~EventListener() = default;
    };
}