#pragma once

#include <vector>

namespace ct
{
    class EventListener
    {
    public:
        virtual void SeatsUpdated(std::vector<int> seatIds) = 0;
        virtual void CartExpired(int cartId) = 0;

        virtual ~EventListener() = 0 {}
    };
}