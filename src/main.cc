#include <Connection/RelayConnection.hpp>
#include <spdlog/spdlog.h>
#include "Util/Net.hpp"
#include "Server.hpp"

int main() {
    Owl::net::io_context ioContext;
    spdlog::set_level(spdlog::level::trace);
    Owl::Server<Owl::RelayConnection>::ServerPtr server
            = Owl::Server<Owl::RelayConnection>::Create(ioContext.get_executor(), 9999);
    ioContext.run();
    return 0;
}
