#pragma once

#include <crow/app.h>
#include <crow/middlewares/session.h>

namespace ct::api
{
    using CrowSession = crow::SessionMiddleware<crow::InMemoryStore>;
    using CrowApp = crow::App<crow::CookieParser, CrowSession>;
}
