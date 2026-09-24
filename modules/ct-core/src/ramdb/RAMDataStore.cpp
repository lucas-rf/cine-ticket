#include <ct-core/impl/ramdb/RAMDataStore.h>
#include <ct-core/utils/Time.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>

namespace ct::impl
{
    static void loadFromJsonContent(RAMDataStore& data, const nlohmann::json& content)
    {
        auto& settings = content["settings"];
        data.settings.cartDuration = std::chrono::seconds(settings["cartDuration"].get<int>());

        auto& movies = content["movies"];
        std::unordered_map<std::string, std::reference_wrapper<RAMMovie>> moviesByKey;
        data.movies.reserve(movies.size());
        int nextMovieId = 0;
        for(auto& movie : movies)
        {
            data.movies.push_back(RAMMovie{
                nextMovieId++,
                movie["title"],
                movie["synopsis"],
                movie["classification"],
                movie["genre"],
                movie["coverImage"],
                movie["backgroundImage"],
                movie["runtime"],
            });
            moviesByKey.try_emplace(movie["key"], data.movies.back());
        }

        auto& bookings = content["bookings"];
        int nextBookingId = 0;
        for(auto& booking : bookings)
            data.bookings.try_emplace(
                booking["key"],
                nextBookingId++,
                -1,
                booking["email"],
                booking["key"],
                utils::StrToTimePoint(booking["time"])
            );

        auto& theaters = content["theaters"];
        data.theaters.reserve(theaters.size());
        int nextTheaterId = 0;
        int nextRoomId = 0;
        int nextSessionId = 0;
        int nextSeatId = 0;
        for(auto& theater : theaters)
        {
            data.theaters.push_back(RAMTheater{
                nextTheaterId++,
                theater["name"],
                decltype(RAMTheater::idxMoviesToDaysToSessions)(data.movies.size())
            });
            auto& ramTheater = data.theaters.back();
            int nextRoomNumber = 1;
            for(auto& room : theater["rooms"])
            {
                data.rooms.push_back(RAMRoom{
                    nextRoomId++,
                    ramTheater.id,
                    nextRoomNumber++,
                    room["type"],
                    room["rows"],
                    room["columns"]
                });
                auto& ramRoom = data.rooms.back();

                int nextDay = 0;
                for(auto& day : room["sessions"])
                {
                    for(auto& session : day)
                    {
                        data.movieSessions.push_back(RAMMovieSession{
                            nextSessionId++,
                            ramRoom.id,
                            nextDay,
                            session["time-hour"].get<int>() * 60 + session["time-minute"].get<int>(),
                            session["price"],
                            nextSeatId
                        });
                        auto& ramSession = data.movieSessions.back();

                        for(int row = 0; row < ramRoom.rows; ++row)
                            for(int col = 0; col < ramRoom.columns; ++col)
                                data.seats.push_back(RAMSeat{
                                    nextSeatId + row * ramRoom.columns + col,
                                    ramSession.id,
                                    row,
                                    col,
                                    -1,
                                    -1,
                                    -1
                                });

                        for(auto& sessionBooking : session["bookings"])
                        {
                            auto& booking = data.bookings.at(sessionBooking["booking-key"]);
                            booking.movieSessionId = ramSession.id;

                            for(auto position : sessionBooking["positions"])
                            {
                                auto seatId = nextSeatId + position.get<int>();
                                booking.seats.push_back(seatId);
                                data.seats[seatId].bookingId = booking.id;
                            }
                        }

                        auto& movie = moviesByKey.at(session["movie-key"]).get();
                        if(nextDay >= movie.idxDaysToSessions.size())
                            movie.idxDaysToSessions.resize(nextDay + 1);
                        movie.idxDaysToSessions[nextDay].push_back(ramSession.id);

                        auto& theatreMovieIdx = ramTheater.idxMoviesToDaysToSessions[movie.id];
                        if(nextDay >= theatreMovieIdx.size())
                            theatreMovieIdx.resize(nextDay + 1);
                        theatreMovieIdx[nextDay].push_back(ramSession.id);

                        nextSeatId += ramRoom.rows * ramRoom.columns;
                    }

                    ++nextDay;
                }
            }
        }
    }

    RAMDataStore::RAMDataStore(const std::filesystem::path& dataFilePath)
    {
        std::ifstream file(dataFilePath);
        auto jsonContent = nlohmann::json::parse(file);
        loadFromJsonContent(*this, jsonContent);
    }

    RAMDataStore::RAMDataStore(const std::string& dataContents)
    {
        auto jsonContent = nlohmann::json::parse(dataContents);
        loadFromJsonContent(*this, jsonContent);
    }
}