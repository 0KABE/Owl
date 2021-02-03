#include <Connection/RelayConnection.hpp>
#include <spdlog/spdlog.h>
#include "Util/Net.hpp"
#include "Server.hpp"
#include "CliOptions.hpp"
#include "Configuration.hpp"

int main(int argc, char *argv[]) {
    CLI::App app;
    Owl::CLIOptions cliOptions;
    Owl::InitialCLI(app, cliOptions);
    CLI11_PARSE(app, argc, argv)

    spdlog::set_level(spdlog::level::from_str(cliOptions.logLevel));

    //TODO implement --config CLI Option
    Owl::Configuration::Load("../../Default.conf");

    Owl::net::io_context ioContext;
    Owl::Server<Owl::RelayConnection>::ServerPtr server
            = Owl::Server<Owl::RelayConnection>::Create(ioContext.get_executor(), 9999);
    ioContext.run();
    return 0;
}
