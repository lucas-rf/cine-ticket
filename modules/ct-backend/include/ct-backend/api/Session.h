#pragma once

#include <ct-backend/api/CrowApp.h>
#include <atomic>

namespace ct::api
{
    class Session
    {
    public:
        Session(CrowApp& crowApp);
        int GetUserKey(const crow::request& request);

    private:
        CrowApp& crowApp;
        std::atomic<int> nextUserKey;
    };
}
