#pragma once

#include <ct-core/TicketPlatform.h>
#include <ct-core/DBApi.h>
#include <ct-core/Timer.h>
#include <ct-core/EventListener.h>
#include <memory>

namespace ct
{
    class TicketPlatformBuilder
    {
    public:
        virtual std::unique_ptr<TicketPlatform> CreateTicketPlatform(DBApi& db, Timer& timer, EventListener& listener) const = 0;

        virtual ~TicketPlatformBuilder() = 0 { }
    };
}
