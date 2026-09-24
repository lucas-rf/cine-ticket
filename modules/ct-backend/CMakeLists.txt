cmake_minimum_required(VERSION 3.24)
project(ct-backend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ct-backend)

target_sources(ct-backend
  PRIVATE
    src/Application.cpp
    src/Launcher.cpp
    src/api/Session.cpp
    src/api/controllers/CommandController.cpp
    src/api/controllers/QueryController.cpp
    src/utils/json/CartJson.cpp
    src/utils/json/MovieJson.cpp
    src/utils/json/MovieSessionJson.cpp
    src/utils/json/BookingJson.cpp
    src/utils/json/RoomJson.cpp
    src/utils/json/RoomSessionJson.cpp
    src/utils/json/SeatJson.cpp
    src/utils/json/TheaterJson.cpp
)

target_include_directories(ct-backend PUBLIC include)
target_include_directories(ct-backend PUBLIC ../../third-party/argparse/include)
target_include_directories(ct-backend PUBLIC ../../third-party/asio/include)
target_include_directories(ct-backend PUBLIC ../../third-party/crow-cpp/include)
target_include_directories(ct-backend PUBLIC ../../third-party/nlohmann-json/include)

enable_testing()

add_subdirectory(../ct-core lib/ct-core)
target_link_libraries(ct-backend PRIVATE ct-core)

install(TARGETS ct-backend)
