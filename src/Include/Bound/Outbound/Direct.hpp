#pragma once

#include "Bound/Outbound.hpp"

namespace Owl {
    class Direct : public Outbound {
    public:
        using Outbound::Outbound;

        Awaitable<void> Initialize();

        Awaitable<void> Send(Buffer &buffer) override;

        Awaitable<void> Receive() override;
    };
}


