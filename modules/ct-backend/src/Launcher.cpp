#pragma once

#include <ct-backend/Application.h>
#include <ct-core/utils/Exception.h>
#include <argparse/argparse.hpp>
#include <cstdlib>
#include <filesystem>
#include <format>

constexpr const char* PORT_ENV_VAR = "CINE_TICKET_PORT";
constexpr const char* DB_FILE_ENV_VAR = "CINE_TICKET_DB_FILE";

constexpr const char* PORT_PARAM = "--port";
constexpr const char* MULTITHR_PARAM = "--multithreaded";
constexpr const char* DB_FILE_PARAM = "--database-file";

static int getPort(argparse::ArgumentParser& parser)
{
    auto checkRange = [](int port, const char* msg) -> int
    {
        if(port < 1024 || port > 65535)
            throw ct::CTException(std::vformat(msg, std::make_format_args(port)));
        return port;
    };

    if(parser.is_used(PORT_PARAM))
    {
        auto port = parser.get<int>(PORT_PARAM);
        return checkRange(port, "Invalid port value '{}' from the command line argument, "
            "please specify a value between 1024 and 65535.");
    }

    if(auto envPort = std::getenv(PORT_ENV_VAR); envPort)
    {
        try
        {
            int port = std::stoi(envPort);
            return checkRange(port, "Invalid port value '{}' from the environment variable, "
                "please specify a value between 1024 and 65535.");
        }
        catch(std::invalid_argument)
        {
            throw ct::CTException(std::format(
                "Invalid port value from the environment variable {}: '{}'",
                PORT_ENV_VAR, envPort));
        }
    }

    throw ct::CTException("Port not specified, application cannot start.");
}

std::filesystem::path getDbFilePath(argparse::ArgumentParser& parser)
{
    if(parser.present(DB_FILE_PARAM))
    {
        auto file = parser.get<std::string>(DB_FILE_PARAM);
        if(!std::filesystem::is_regular_file(file))
            throw ct::CTException(std::format(
                "Invalid database file from the command line argument {}: '{}'",
                DB_FILE_PARAM, file));
        return file;
    }

    if(auto envFile = std::getenv(DB_FILE_ENV_VAR); envFile)
    {
        if(!std::filesystem::is_regular_file(envFile))
            throw ct::CTException(std::format(
                "Invalid database file from the environment variable {}: '{}'",
                DB_FILE_ENV_VAR, envFile));
        return envFile;
    }

    throw ct::CTException("Database file not specified, application cannot start.");
}

int main(int argc, char* argv[])
{
    argparse::ArgumentParser parser("cine-ticket-backend", "1.0");

    parser.add_argument(PORT_PARAM)
        .scan<'i', int>()
        .help("The port in which incoming connections are to be accepted. If not specified, "
            "uses the value defined via the CINE_TICKET_PORT environment variable.");

    parser.add_argument(MULTITHR_PARAM)
        .help("Enable multithreaded mode")
        .flag();

    parser.add_argument(DB_FILE_PARAM)
        .help("Load database data from the specified file. If not specified, tries to "
            "load from the path defined by the CINE_TICKET_DB_FILE environment variable.");

    try
    {
        parser.parse_args(argc, argv);
    }
    catch(const std::exception& exc)
    {
        std::cerr << exc.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    try
    {
        ct::Application application(
            getPort(parser),
            parser.get<bool>(MULTITHR_PARAM),
            getDbFilePath(parser)
        );

        application.Run();
    }
    catch(const ct::CTException& ctExc)
    {
        std::cerr << ctExc.what() << std::endl;
        return 1;
    }
    catch(const std::exception& exc)
    {
        std::cerr << "Execution failed due to an exception: '";
        std::cerr << exc.what() << "'" << std::endl;
        return 1;
    }

    return 0;
}
