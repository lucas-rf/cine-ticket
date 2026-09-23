#include <ct-core/impl/utils/ClockImpl.h>

namespace ct::impl
{
    ClockImpl::ClockImpl()
    { }

    ClockImpl::time_point ClockImpl::now() const noexcept
    {
        return ClockImpl::ref_clock::now();
    }

    const ClockImpl ClockImpl::Instance{};
}
