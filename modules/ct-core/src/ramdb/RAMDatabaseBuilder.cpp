#include <ct-core/impl/ramdb/RAMDatabaseBuilder.h>
#include <ct-core/impl/ramdb/RAMDatabase.h>

namespace ct::impl
{
    std::unique_ptr<DBApi> RAMDatabaseBuilder::CreateDBApi(const std::filesystem::path& dataFilePath) const
    {
        return std::make_unique<RAMDatabase>(dataFilePath);
    }

    std::unique_ptr<DBApi> RAMDatabaseBuilder::CreateDBApi(const std::string& dataContents) const
    {
        return std::make_unique<RAMDatabase>(dataContents);
    }

    RAMDatabaseBuilder::RAMDatabaseBuilder()
    { }

    const RAMDatabaseBuilder RAMDatabaseBuilder::Instance{};
}
