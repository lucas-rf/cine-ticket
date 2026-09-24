#pragma once

#include <stdexcept>
#include <format>

namespace ct
{
    class CTException: public std::exception
    {
    public:
        CTException(std::string&& msg):
            message{std::move(msg)}
        { }
        
        CTException(const CTException& other):
            CTException(std::string{other.message})
        { }
        
        CTException(CTException&& other) noexcept:
            message(std::move(other.message))
        { }

        virtual const char* what() const noexcept override
        {
            return message.c_str();
        }

    private:
        std::string message;
    };

    class ItemNotFoundException: public CTException
    {
    public:
        using CTException::CTException;
    };

    class MovieNotFoundException: public ItemNotFoundException
    {
    public:
        MovieNotFoundException(int movieId):
            ItemNotFoundException(std::format("Movie id #{} not found", movieId))
        { }
    };

    class TheaterNotFoundException: public ItemNotFoundException
    {
    public:
        TheaterNotFoundException(int theaterId):
            ItemNotFoundException(std::format("Theater id #{} not found", theaterId))
        { }
    };

    class MovieSessionNotFoundException: public ItemNotFoundException
    {
    public:
        MovieSessionNotFoundException(int movieSessionId):
            ItemNotFoundException(std::format("Movie session id #{} not found", movieSessionId))
        { }
    };

    class CartNotFoundException: public ItemNotFoundException
    {
    public:
        CartNotFoundException():
            ItemNotFoundException("No cart found for the current session")
        { }

        CartNotFoundException(int cartId):
            ItemNotFoundException(std::format("Cart id #{} not found", cartId))
        { }
    };

    class SeatNotFoundException: public ItemNotFoundException
    {
    public:
        SeatNotFoundException(int seatId):
            ItemNotFoundException(std::format("Seat id #{} not found", seatId))
        { }
    };

    class SeatNotInCartException: public CTException
    {
    public:
        SeatNotInCartException(int cartId, int seatId):
            CTException(std::format("Seat id #{} is not in cart id #{}", seatId, cartId))
        { }
    };

    class BookingNotFoundException: public ItemNotFoundException
    {
    public:
        BookingNotFoundException(const std::string& bookingKey):
            ItemNotFoundException(std::format("Booking with key #{} not found", bookingKey))
        { }
    };


}