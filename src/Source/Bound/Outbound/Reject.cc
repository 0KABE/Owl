#include "Bound/Outbound/Reject.hpp"
#include <spdlog/spdlog.h>

Owl::Awaitable<void> Owl::Reject::Send(Owl::Buffer &buffer) {
    spdlog::info("Reject this connection");
    throw std::runtime_error("Reject this connection");
}

Owl::Awaitable<void> Owl::Reject::Receive() {
    spdlog::info("Reject this connection");
    throw std::runtime_error("Reject this connection");
}
