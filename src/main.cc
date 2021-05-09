#include <Connection/RelayConnection.hpp>
#include "Connection/ApiConnection.hpp"
#include <spdlog/spdlog.h>
#include "Util/Net.hpp"
#include "Server.hpp"
#include "CliOptions.hpp"
#include "Configuration.hpp"

int main(int argc, char *argv[]) {
    CLI::App app;
    Owl::CLIOptions &cliOptions = Owl::CLIOptions::GetInstance();
    Owl::InitialCLI(app, cliOptions);
    CLI11_PARSE(app, argc, argv)

    spdlog::set_level(spdlog::level::from_str(cliOptions.logLevel));

    Owl::net::io_context ioContext;

    Owl::Configuration::Load(cliOptions.config, ioContext.get_executor());

    Owl::Server<Owl::RelayConnection>::ServerPtr server
            = Owl::Server<Owl::RelayConnection>::Create(ioContext.get_executor(), 9999);
    Owl::Server<Owl::ApiConnection>::ServerPtr apiServer
            = Owl::Server<Owl::ApiConnection>::Create(ioContext.get_executor(), 8080);

    ioContext.run();
    return 0;
}
