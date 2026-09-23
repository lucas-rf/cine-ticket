#pragma once

#include <ct-core/DBApi.h>
#include <memory>
#include <filesystem>
#include <string>

namespace ct
{
    class DBApiBuilder
    {
    public:
        virtual std::unique_ptr<DBApi> CreateDBApi(const std::filesystem::path& dataFilePath) const = 0;
        virtual std::unique_ptr<DBApi> CreateDBApi(const std::string& dataContents) const = 0;

        virtual ~DBApiBuilder() = default;
    };
}
