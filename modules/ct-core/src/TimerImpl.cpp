#include <ct-core/impl/TimerImpl.h>

namespace ct::impl
{
    TimerImpl::TimerImpl():
        worker{&TimerImpl::threadEntry, this}
    { }

    Timer::ID TimerImpl::Set(TimePoint when, Callback&& callback)
    {
        std::lock_guard<std::mutex> guard{lock};
        auto id = nextId++;
        auto iter = queue.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(when),
            std::forward_as_tuple(id, std::move(callback))
        );
        idToItem.try_emplace(id, iter);
        if(iter == queue.begin())
            event.notify_all();
        return id;
    }

    bool TimerImpl::Cancel(ID id) noexcept
    {
        std::lock_guard<std::mutex> guard{lock};
        auto node = idToItem.extract(id);
        if(node.empty())
            return false;
        queue.erase(node.mapped());
        return true;
    }

    TimerImpl::~TimerImpl()
    {
        {
            std::lock_guard<std::mutex> guard{lock};
            active = false;
        }

        event.notify_all();
        worker.join();
    }

    void TimerImpl::threadEntry()
    {
        std::unique_lock<std::mutex> guard{lock};

        while(active)
        {
            if(queue.empty())
                event.wait(guard);
            else
                event.wait_until(guard, queue.begin()->first);

            if(!active)
                break;

            if(queue.empty() || queue.begin()->first > Clock::ref_clock::now())
                continue;

            auto item = queue.extract(queue.begin());
            idToItem.erase(item.mapped().first);

            guard.unlock();

            executeCallback(std::move(item.mapped().second));

            guard.lock();
        }
    }

    void TimerImpl::executeCallback(Callback callback) noexcept
    {
        try
        {
            callback();
        }
        catch(std::exception&)
        {
            // TODO: Log
        }
        catch(...)
        {
            // TODO: Log
        }
    }

}