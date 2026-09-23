#include <ct-core/impl/TicketPlatformImpl.h>
#include <ct-core/impl/ramdb/RAMDatabase.h>
#include <ct-core/utils/Exception.h>
#include <test-utils/DatabaseData.h>
#include <test-utils/ModelComparison.h>
#include <test-utils/MockClock.h>
#include <gtest/gtest.h>
#include <memory>

namespace ct::test
{
    class MockTimer: public Timer
    {
    public:
        virtual ID Set(TimePoint when, Callback&& callback) override
        {
            calls_Set.emplace_back(when, std::move(callback));
            return return_Set;
        };

        virtual bool Cancel(ID id) noexcept override
        {
            calls_Cancel.emplace_back(id);
            return return_Cancel;
        };

        void Clear()
        {
            calls_Set.clear();
            calls_Cancel.clear();
        }

        ID return_Set{0};
        bool return_Cancel{false};
        std::vector<std::pair<TimePoint, Callback>> calls_Set;
        std::vector<ID> calls_Cancel;
    };

    class MockEventListener: public EventListener
    {
    public:
        virtual void SeatSelected(const model::Seat& seat, int userKey) override
        {
            calls_SeatSelected.emplace_back(seat, userKey);
        }

        virtual void SeatDeselected(const model::Seat& seat, int userKey) override
        {
            calls_SeatDeselected.emplace_back(seat, userKey);
        }

        virtual void SeatsDeselected(const std::vector<model::Seat>& seats, int userKey) override
        {
            calls_SeatsDeselected.emplace_back(seats, userKey);
        }

        virtual void SeatsOrdered(const std::vector<model::Seat>& seats, int userKey) override
        {
            calls_SeatsOrdered.emplace_back(seats, userKey);
        }

        virtual void CartCreated(int cartId, int userKey) override
        {
            calls_CartCreated.emplace_back(cartId, userKey);
        }

        virtual void CartRecreated(int cartId, int userKey) override
        {
            calls_CartRecreated.emplace_back(cartId, userKey);
        }

        virtual void CartExpired(int cartId, int userKey) override
        {
            calls_CartExpired.emplace_back(cartId, userKey);
        }

        void Clear()
        {
            calls_SeatSelected.clear();
            calls_SeatDeselected.clear();
            calls_SeatsDeselected.clear();
            calls_SeatsOrdered.clear();
            calls_CartCreated.clear();
            calls_CartRecreated.clear();
            calls_CartExpired.clear();
        }

        std::vector<std::pair<model::Seat, int>> calls_SeatSelected;
        std::vector<std::pair<model::Seat, int>> calls_SeatDeselected;
        std::vector<std::pair<std::vector<model::Seat>, int>> calls_SeatsDeselected;
        std::vector<std::pair<std::vector<model::Seat>, int>> calls_SeatsOrdered;
        std::vector<std::pair<int, int>> calls_CartCreated;
        std::vector<std::pair<int, int>> calls_CartRecreated;
        std::vector<std::pair<int, int>> calls_CartExpired;
    };

    class MockApp
    {
    public:
        MockApp():
            clock{},
            db{std::string{utils::db_data::DB_DATA_SIMPLE_LONG}, clock},
            timer{},
            listener{},
            platform{db, timer, &listener, clock}
        { }

        MockClock clock;
        impl::RAMDatabase db;
        MockTimer timer;
        MockEventListener listener;
        impl::TicketPlatformImpl platform;
    };

    class TestTicketPlatformImpl: public testing::Test
    {
    protected:
        virtual void SetUp()
        {
            app = std::make_unique<MockApp>();
        }

        virtual void TearDown()
        {
            app.reset();
        }

        std::unique_ptr<MockApp> app;
    };

    TEST_F(TestTicketPlatformImpl, SelectSeat_SelectFreeSeatForEmptyCart)
    {
        app->platform.SetCartEvents(true);
        app->platform.SetMovieSessionEvents(1, true);

        auto result = app->platform.SelectSeat(10, 555);

        ASSERT_EQ(result, true);

        ASSERT_EQ(app->timer.calls_Set.size(), 1);
        ASSERT_EQ(app->timer.calls_Set[0].first, app->clock.NowValue + std::chrono::seconds{100});

        ASSERT_EQ(app->listener.calls_CartCreated.size(), 1);
        ASSERT_EQ(app->listener.calls_CartCreated[0], (std::make_pair<int, int>(0, 555)));

        ASSERT_EQ(app->listener.calls_SeatDeselected.size(), 0);
        ASSERT_EQ(app->listener.calls_SeatsDeselected.size(), 0);

        ASSERT_EQ(app->listener.calls_SeatSelected.size(), 1);
        ASSERT_EQ(app->listener.calls_SeatSelected[0].second, 555);
        ASSERT_EQ(app->listener.calls_SeatSelected[0].first, (model::Seat{
            .id = 10,
            .movieSessionId = 1,
            .row = 0,
            .column = 0,
            .state = model::Seat::SELECTED_BY_CURRENT_USER,
            .cartId = 0,
            .orderId = -1
        }));
    }

    TEST_F(TestTicketPlatformImpl, SelectSeat_SelectFreeSeatForNonEmptyCartSameSession)
    {
        auto resultFirst = app->platform.SelectSeat(10, 555);

        app->platform.SetCartEvents(true);
        app->platform.SetMovieSessionEvents(1, true);

        app->timer.Clear();

        auto resultSecond = app->platform.SelectSeat(11, 555);

        ASSERT_EQ(resultSecond, true);

        ASSERT_EQ(app->timer.calls_Set.size(), 0);

        ASSERT_EQ(app->listener.calls_CartCreated.size(), 0);
        ASSERT_EQ(app->listener.calls_CartRecreated.size(), 0);
        ASSERT_EQ(app->listener.calls_CartExpired.size(), 0);

        ASSERT_EQ(app->listener.calls_SeatDeselected.size(), 0);
        ASSERT_EQ(app->listener.calls_SeatsDeselected.size(), 0);

        ASSERT_EQ(app->listener.calls_SeatSelected.size(), 1);
        ASSERT_EQ(app->listener.calls_SeatSelected[0].second, 555);
        ASSERT_EQ(app->listener.calls_SeatSelected[0].first, (model::Seat{
            .id = 11,
            .movieSessionId = 1,
            .row = 0,
            .column = 1,
            .state = model::Seat::SELECTED_BY_CURRENT_USER,
            .cartId = 0,
            .orderId = -1
        }));
    }

    TEST_F(TestTicketPlatformImpl, SelectSeat_SelectFreeSeatForNonEmptyCartDifferentSession)
    {
        app->platform.SelectSeat(10, 555);
        app->platform.SelectSeat(11, 555);
        app->platform.SelectSeat(12, 555);

        app->platform.SetCartEvents(true);
        app->platform.SetMovieSessionEvents(1, true);
        app->platform.SetMovieSessionEvents(2, true);

        app->timer.Clear();
        app->clock.NowValue += std::chrono::seconds{100};

        auto result = app->platform.SelectSeat(20, 555);

        ASSERT_EQ(result, true);

        ASSERT_EQ(app->timer.calls_Set.size(), 1);
        ASSERT_EQ(app->timer.calls_Set[0].first, app->clock.NowValue + std::chrono::seconds{100});

        ASSERT_EQ(app->listener.calls_CartCreated.size(), 0);
        ASSERT_EQ(app->listener.calls_CartExpired.size(), 0);
        ASSERT_EQ(app->listener.calls_CartRecreated.size(), 1);
        ASSERT_EQ(app->listener.calls_CartRecreated[0], (std::make_pair<int, int>(0, 555)));

        ASSERT_EQ(app->listener.calls_SeatDeselected.size(), 0);
        ASSERT_EQ(app->listener.calls_SeatsDeselected.size(), 1);
        ASSERT_EQ(app->listener.calls_SeatsDeselected[0].second, 555);
        ASSERT_EQ(app->listener.calls_SeatsDeselected[0].first, (std::vector<model::Seat>{
            {
                .id = 10,
                .movieSessionId = 1,
                .row = 0,
                .column = 0,
                .state = model::Seat::FREE,
                .cartId = -1,
                .orderId = -1
            },
            {
                .id = 11,
                .movieSessionId = 1,
                .row = 0,
                .column = 1,
                .state = model::Seat::FREE,
                .cartId = -1,
                .orderId = -1
            },
            {
                .id = 12,
                .movieSessionId = 1,
                .row = 0,
                .column = 2,
                .state = model::Seat::FREE,
                .cartId = -1,
                .orderId = -1
            },
        }));

        ASSERT_EQ(app->listener.calls_SeatSelected.size(), 1);
        ASSERT_EQ(app->listener.calls_SeatSelected[0].second, 555);
        ASSERT_EQ(app->listener.calls_SeatSelected[0].first, (model::Seat{
            .id = 20,
            .movieSessionId = 2,
            .row = 0,
            .column = 0,
            .state = model::Seat::SELECTED_BY_CURRENT_USER,
            .cartId = 0,
            .orderId = -1
        }));
    }
}