cmake_minimum_required(VERSION 3.24)
project(ct-core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ct-core)

target_sources(ct-core
  PRIVATE
    src/TicketPlatformImpl.cpp
    src/TicketPlatformImplBuilder.cpp
    src/TimerImpl.cpp
    src/TimerImplBuilder.cpp
    src/ramdb/RAMDatabase.cpp
    src/ramdb/RAMDatabaseBuilder.cpp
    src/ramdb/RAMDataStore.cpp
    src/utils/ClockImpl.cpp
    src/utils/Time.cpp
)

target_include_directories(ct-core PUBLIC include)
target_include_directories(ct-core PUBLIC ../../third-party/nlohmann-json/include)

set(INSTALL_GTEST OFF)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.18.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(CTest)
include(GoogleTest)
enable_testing()

add_executable(ct-core-tests)

target_sources(ct-core-tests
  PRIVATE
    tests/unit/Test_RAMDatabase.cpp
    tests/unit/Test_RAMDataStore.cpp
    tests/integration/Test_TicketPlatformImpl.cpp
    tests/utils/src/DatabaseData.cpp
)

target_include_directories(ct-core-tests PRIVATE tests/utils/include)

target_link_libraries(ct-core-tests PRIVATE ct-core)
target_link_libraries(ct-core-tests PRIVATE GTest::gtest GTest::gtest_main)

gtest_discover_tests(ct-core-tests)

