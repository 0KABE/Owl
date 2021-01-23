#pragma once

#include "Bound.hpp"

namespace Owl {
    class Inbound : public Bound {
    public:
        using BoundPtr = std::unique_ptr<Inbound>;
        using Bound::Bound;

        virtual Awaitable<TargetEndpoint> Initialize() = 0;
    };
}


