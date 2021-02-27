#pragma once

#include "Bound/Outbound.hpp"

namespace Owl {
    class Reject : public Outbound {
    public:
        using Outbound::Outbound;

        Awaitable<void> Send(Buffer &buffer) override;

        Awaitable<std::reference_wrapper<Buffer>> Receive() override;
    };
}


