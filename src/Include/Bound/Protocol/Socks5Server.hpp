#pragma once

#include "Bound/Protocol.hpp"

namespace Owl {
    class Socks5Server : public Protocol {
    public:
        using Protocol::Protocol;

        Awaitable<TargetEndpoint> Initialize() override;

        Awaitable<void> Send(Buffer &buffer) override;

        Awaitable<void> Receive() override;

    private:
        Owl::Awaitable<std::optional<Endpoint>> Handshake();
    };
}


