#pragma once

#include "Bound/Inbound.hpp"

namespace Owl {
    class Socks5Server : public Inbound {
    public:
        using Inbound::Inbound;

        Awaitable<TargetEndpoint> Initialize() override;

        Awaitable<void> Send(Buffer &buffer) override;

        Awaitable<std::reference_wrapper<Buffer>> Receive() override;

    private:
        Owl::Awaitable<std::optional<Endpoint>> Handshake();
    };
}


