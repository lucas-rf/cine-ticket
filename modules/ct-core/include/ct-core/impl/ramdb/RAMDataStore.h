#pragma once

#include <ct-core/model/PlatformSettings.h>
#include <ct-core/impl/ramdb/RAMMovie.h>
#include <ct-core/impl/ramdb/RAMMovieSession.h>
#include <ct-core/impl/ramdb/RAMRoom.h>
#include <ct-core/impl/ramdb/RAMTheater.h>
#include <ct-core/impl/ramdb/RAMSeat.h>
#include <ct-core/impl/ramdb/RAMBooking.h>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <string>

namespace ct::impl
{
    class RAMDataStore
    {
    public:
        explicit RAMDataStore(const std::filesystem::path& dataFilePath);
        explicit RAMDataStore(const std::string& dataContents);

    public:
        model::PlatformSettings settings{};
        std::vector<RAMMovie> movies;
        std::vector<RAMTheater> theaters;
        std::vector<RAMRoom> rooms;
        std::vector<RAMMovieSession> movieSessions;
        std::vector<RAMSeat> seats;
        std::unordered_map<std::string, RAMBooking> bookings;
    };
}
