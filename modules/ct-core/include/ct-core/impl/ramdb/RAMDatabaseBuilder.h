#pragma once

#include <ct-core/DBApiBuilder.h>

namespace ct::impl
{
    class RAMDatabaseBuilder: public DBApiBuilder
    {
    public:
        virtual std::unique_ptr<DBApi> CreateDBApi(const std::filesystem::path& dataFilePath) const override;
        virtual std::unique_ptr<DBApi> CreateDBApi(const std::string& dataContents) const override;

        static const RAMDatabaseBuilder Instance;

    private:
        RAMDatabaseBuilder();
    };
}
