#include <ct-core/impl/ramdb/RAMDatabase.h>
#include <ct-core/utils/Exception.h>
#include <test-utils/DatabaseData.h>
#include <test-utils/ModelComparison.h>
#include <test-utils/MockClock.h>
#include <gtest/gtest.h>
#include <memory>

namespace ct::test
{
    class TestRAMDatabaseReadonly: public testing::Test
    {
    protected:
        static void SetUpTestCase()
        {
            db = std::make_unique<impl::RAMDatabase>(std::string{utils::db_data::DB_DATA_SIMPLE_LONG});
        }

        static void TearDownTestCase()
        {
            db.reset();
        }

        static std::unique_ptr<impl::RAMDatabase> db;
    };

    std::unique_ptr<impl::RAMDatabase> TestRAMDatabaseReadonly::db{};

    TEST_F(TestRAMDatabaseReadonly, MovieGet_FailIfIdLessThan0)
    {
        ASSERT_THROW(db->Movie_Get(-1), MovieNotFoundException);
    }

    TEST_F(TestRAMDatabaseReadonly, MovieGet_FailIfIdGreaterThanLast)
    {
        ASSERT_THROW(db->Movie_Get(2), MovieNotFoundException);
    }

    TEST_F(TestRAMDatabaseReadonly, MovieGet_DataIsCorrect)
    {
        model::Movie movie0
        {
            0,
            "movie-0-title",
            "movie-0-synopsis",
            "movie-0-classification",
            {"movie-0-genre-0", "movie-0-genre-1", "movie-0-genre-2"},
            "movie-0-coverImage",
            "movie-0-backgroundImage",
            100,
            {}
        };

        ASSERT_EQ(db->Movie_Get(0), movie0);

        model::Movie movie1
        {
            1,
            "movie-1-title",
            "movie-1-synopsis",
            "movie-1-classification",
            {"movie-1-genre-0", "movie-1-genre-1", "movie-1-genre-2"},
            "movie-1-coverImage",
            "movie-1-backgroundImage",
            101,
            {}
        };

        ASSERT_EQ(db->Movie_Get(1), movie1);
    }

    class TestRAMDatabaseReadWrite: public testing::Test
    {
    protected:
        virtual void SetUp()
        {
            clock = std::make_unique<MockClock>();
            db = std::make_unique<impl::RAMDatabase>(std::string{utils::db_data::DB_DATA_SIMPLE_LONG}, *clock);
        }

        virtual void TearDown()
        {
            db.reset();
            clock.reset();
        }

        std::unique_ptr<MockClock> clock;
        std::unique_ptr<impl::RAMDatabase> db;
    };

    TEST_F(TestRAMDatabaseReadWrite, CartGetOrCreate_CreateNew)
    {
        auto [cart, created] = db->Cart_GetOrCreate(555);
        
        ASSERT_EQ(created, true);

        model::Cart expectedCart{
            0,
            -1,
            555,
            clock->NowValue + std::chrono::seconds{100},
            0,
            {}
        };

        ASSERT_EQ(cart, expectedCart);
    }

    TEST_F(TestRAMDatabaseReadWrite, CartGetOrCreate_GetExisting)
    {
        auto [firstCart, firstCreated] = db->Cart_GetOrCreate(555);

        ASSERT_EQ(firstCreated, true);

        clock->NowValue += std::chrono::years{1};

        auto [secondCart, secondCreated] = db->Cart_GetOrCreate(555);

        ASSERT_EQ(secondCreated, false);
        ASSERT_EQ(secondCart, firstCart);
    }

    TEST_F(TestRAMDatabaseReadWrite, CartTryAddSeat_AddInvalidSeatIdLessThan0)
    {
        ASSERT_THROW(db->Cart_TryAddSeat(0, -1), SeatNotFoundException);
    }

    TEST_F(TestRAMDatabaseReadWrite, CartTryAddSeat_AddInvalidSeatIdGreaterThanLast)
    {
        ASSERT_THROW(db->Cart_TryAddSeat(0, 320), SeatNotFoundException);
    }

    TEST_F(TestRAMDatabaseReadWrite, CartTryAddSeat_AddValidSeatToNonExistingCart)
    {
        ASSERT_THROW(db->Cart_TryAddSeat(0, 0), CartNotFoundException);

        auto [cart, created] = db->Cart_GetOrCreate(555);

        ASSERT_THROW(db->Cart_TryAddSeat(1, 0), CartNotFoundException);
    }

    TEST_F(TestRAMDatabaseReadWrite, CartTryAddSeat_AddFreeSeatToEmptyCart)
    {
        auto [cart, created] = db->Cart_GetOrCreate(555);

        ASSERT_EQ(cart.expirationTime, clock->NowValue + std::chrono::seconds{100});
        
        clock->NowValue += std::chrono::seconds{100};
        auto resultingCart = db->Cart_TryAddSeat(cart.id, 10);
        
        ASSERT_EQ(resultingCart.has_value(), true);

        model::Cart expectedCart {
            0,
            1,
            555,
            clock->NowValue + std::chrono::seconds{100},
            1,
            {{
                10,
                1,
                0,
                0,
                model::Seat::SELECTED_BY_CURRENT_USER,
                0,
                -1
            }}
        };

        ASSERT_EQ(resultingCart.value(), expectedCart);
    }

    TEST_F(TestRAMDatabaseReadWrite, CartTryAddSeat_AddSeatAlreadySelectedByCurrentUser)
    {
        auto [cart, created] = db->Cart_GetOrCreate(555);
        auto firstResultingCart = db->Cart_TryAddSeat(cart.id, 10);

        ASSERT_EQ(firstResultingCart.has_value(), true);

        auto secondResultingCart = db->Cart_TryAddSeat(cart.id, 10);

        ASSERT_EQ(secondResultingCart.has_value(), false);
    }

    TEST_F(TestRAMDatabaseReadWrite, CartTryAddSeat_AddSeatAlreadySelectedByOtherUser)
    {
        auto [cartA, createdA] = db->Cart_GetOrCreate(444);
        auto firstResultingCart = db->Cart_TryAddSeat(cartA.id, 10);

        ASSERT_EQ(firstResultingCart.has_value(), true);

        auto [cartB, createdB] = db->Cart_GetOrCreate(555);
        auto secondResultingCart = db->Cart_TryAddSeat(cartB.id, 10);

        ASSERT_EQ(secondResultingCart.has_value(), false);

        auto seat = db->Seat_Get(10, 555);

        ASSERT_EQ(seat.cartId, cartA.id);

        auto updatedCartA = db->Cart_GetOrCreate(444);

        ASSERT_EQ(updatedCartA.first.seatCount, 1);
        ASSERT_EQ(updatedCartA.first.seats.size(), 1);
        ASSERT_EQ(updatedCartA.first.seats[0].id, 10);

        auto updatedCartB = db->Cart_GetOrCreate(555);

        ASSERT_EQ(updatedCartB.first.seatCount, 0);
        ASSERT_EQ(updatedCartB.first.seats.size(), 0);
    }

    TEST_F(TestRAMDatabaseReadWrite, CartTryAddSeat_AddSeatAlreadyBooked)
    {
        auto [cart, created] = db->Cart_GetOrCreate(555);
        auto resultingCart = db->Cart_TryAddSeat(cart.id, 3);
        ASSERT_EQ(resultingCart.has_value(), false);
    }


}
