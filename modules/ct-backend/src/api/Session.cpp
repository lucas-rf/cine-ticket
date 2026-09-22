#include <ct-backend/api/Session.h>
#include <string_view>

namespace ct::api
{
    constexpr const char* USERKEY_NAME = "userKey";

    Session::Session(CrowApp& crowApp):
        crowApp{crowApp},
        nextUserKey{0}
    { }

    int Session::GetUserKey(const crow::request& request)
    {
        auto& session = crowApp.get_context<CrowSession>(request);
        std::lock_guard<std::recursive_mutex> guard(session.mutex());
        int userKey = session.get(USERKEY_NAME, -1);
        if(userKey == -1)
        {
            userKey = nextUserKey.fetch_add(1);
            session.set(USERKEY_NAME, userKey);
        }
        return userKey;
    }

}
