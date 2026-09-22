#pragma once

#include <ct-core/TicketPlatformBuilder.h>

namespace ct::impl
{
    class TicketPlatformImplBuilder: public TicketPlatformBuilder
    {
    public:
        virtual std::unique_ptr<TicketPlatform> CreateTicketPlatform(DBApi& db, Timer& timer, EventListener* listener) const override;

        static const TicketPlatformImplBuilder Instance;

    private:
        TicketPlatformImplBuilder();
    };
}
