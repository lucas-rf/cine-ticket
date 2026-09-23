#include <ct-core/impl/ramdb/RAMDataStore.h>
#include <test-utils/DatabaseData.h>
#include <test-utils/RAMModelComparison.h>
#include <gtest/gtest.h>

namespace ct::test
{
    TEST(TestRAMDataStore, TestLoadConsistency)
    {
        using namespace ct::impl;

        RAMDataStore store(std::string{utils::db_data::DB_DATA_SIMPLE_LONG});

        ASSERT_EQ(store.settings.cartDuration, std::chrono::seconds(100));

        std::vector<RAMMovie> movies{
            {
                0,
                "movie-0-title",
                "movie-0-synopsis",
                "movie-0-classification",
                {"movie-0-genre-0", "movie-0-genre-1", "movie-0-genre-2"},
                "movie-0-coverImage",
                "movie-0-backgroundImage",
                100,
                {{0, 1, 8, 9, 16, 17, 24, 25}, {4, 5, 12, 13, 20, 21, 28, 29}}
            },
            {
                1,
                "movie-1-title",
                "movie-1-synopsis",
                "movie-1-classification",
                {"movie-1-genre-0", "movie-1-genre-1", "movie-1-genre-2"},
                "movie-1-coverImage",
                "movie-1-backgroundImage",
                101,
                {{2, 3, 10, 11, 18, 19, 26, 27}, {6, 7, 14, 15, 22, 23, 30, 31}}
            },
        };

        ASSERT_EQ(store.movies, movies);
    }
}
