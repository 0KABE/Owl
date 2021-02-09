#pragma once

#include "Bound.hpp"

namespace Owl {
    class Outbound : public Bound {
    public:
        using BoundPtr = std::unique_ptr<Outbound>;
        using Bound::Bound;
    };
}


