#include <ct-core/impl/TimerImplBuilder.h>
#include <ct-core/impl/TimerImpl.h>

namespace ct::impl
{
    std::unique_ptr<Timer> TimerImplBuilder::CreateTimer() const
    {
        return std::make_unique<TimerImpl>();
    }

    TimerImplBuilder::TimerImplBuilder()
    { }

    const TimerImplBuilder TimerImplBuilder::Instance{};
}
