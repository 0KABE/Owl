#pragma once

#include "Bound/Raw.hpp"

namespace Owl {
    class Direct : public Raw {
    public:
        using Raw::Raw;

        Awaitable<TargetEndpoint> Initialize() override;

        Awaitable<void> Send(Buffer &buffer) override;

        Awaitable<void> Receive() override;
    };
}


