#include <ct-core/impl/TicketPlatformImplBuilder.h>
#include <ct-core/impl/TicketPlatformImpl.h>

namespace ct::impl
{
    std::unique_ptr<TicketPlatform> TicketPlatformImplBuilder::CreateTicketPlatform(DBApi& db, Timer& timer, EventListener* listener) const
    {
        return std::make_unique<TicketPlatformImpl>(db, timer, listener);
    }

    TicketPlatformImplBuilder::TicketPlatformImplBuilder()
    { }

    const TicketPlatformImplBuilder TicketPlatformImplBuilder::Instance{};
}
