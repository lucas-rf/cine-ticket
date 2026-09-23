#pragma once

#include <ct-core/Timer.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <map>
#include <unordered_map>

namespace ct::impl
{
    class TimerImpl: public Timer
    {
    public:
        TimerImpl();

        virtual ID Set(TimePoint when, Callback&& callback) override;
        virtual bool Cancel(ID id) noexcept override;

        virtual ~TimerImpl();

    private:
        using QueueType = std::multimap<TimePoint, std::pair<ID, Callback>>;

        std::mutex lock;
        std::condition_variable event;
        QueueType queue;
        std::unordered_map<ID, QueueType::iterator> idToItem;
        ID nextId{0};
        bool active{true};
        std::thread worker;

        void threadEntry();
        void executeCallback(Callback callback) noexcept;
    };
}
