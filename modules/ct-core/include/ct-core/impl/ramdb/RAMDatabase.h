#pragma once

#include <ct-core/DBApi.h>
#include <ct-core/impl/ramdb/RAMDataStore.h>
#include <ct-core/impl/ramdb/RAMCart.h>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <filesystem>

namespace ct::impl
{
    class RAMDatabase: public DBApi
    {
    public:
        explicit RAMDatabase(const std::filesystem::path& dataFilePath);
        explicit RAMDatabase(const std::string& dataContents);

        virtual model::PlatformSettings PlatformSettings_GetOne() const override;

        virtual model::Movie Movie_Get(int movieId) const override;
        virtual model::Movie Movie_ViewDetailed(int movieId, int day) const override;
        virtual std::vector<model::Movie> Movies_GetAll() const override;
        virtual std::vector<model::Movie> Movies_ViewByTheater(int theaterId, int day) const override;

        virtual model::RoomSession RoomSession_ViewDetailed(int movieSessionId, int userKey) const override;

        virtual std::pair<model::Cart, bool> Cart_GetOrCreate(int userKey) override;
        virtual bool Cart_TryDelete(int cartId) override;
        virtual std::vector<model::Cart> Carts_GetAll() const override;
        virtual model::Cart Cart_ViewDetailed(int cartId) const override;

        virtual std::optional<model::Cart> Cart_TryAddSeat(int cartId, int seatId) override;
        virtual model::Cart Cart_RemoveSeat(int cartId, int seatId) override;

        virtual model::Seat Seat_Get(int seatId, int userKey) const override;

        virtual model::Order Order_ViewDetailed(const std::string& orderKey) const override;
        virtual model::Order Order_CreateFromCart(int cartId, const std::string& orderKey, const std::string& userEmail) override;

    private:
        mutable std::mutex cartLock;
        RAMDataStore data;
        std::unordered_map<int, RAMCart> cartsById;
        std::unordered_map<int, RAMCart*> cartsByUserKey;
        int nextCartId{0};
        int nextOrderId;
        
        std::vector<model::Theater> expandTheaters(const std::unordered_map<int, std::unordered_map<int, std::vector<int>>>& theatersToRoomsToSessions) const;
        model::Cart generateModelCart(const RAMCart& ramCart, bool includeSeats) const;
        model::Order generateModelOrder(const RAMOrder& ramOrder) const;
        static model::Movie generateModelMovie(const RAMMovie& ramMovie, std::vector<model::Theater>&& theaters);
        static model::Seat generateSeatModel(const RAMSeat& ramSeat, int userKey);
        std::vector<model::Seat> generateSeatModels(const std::vector<int>& seatsIds, int userKey) const;
        std::vector<model::Seat> generateSeatModels(int beginId, int endId, int userKey) const;
    };
}