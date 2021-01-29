#pragma once

#include "Bound.hpp"

namespace Owl {
    class Outbound : public Bound {
    public:
        using BoundPtr = std::unique_ptr<Outbound>;
        using Bound::Bound;
    };

    template<typename T>
    concept OutboundDerived = std::is_base_of<Outbound, T>::value && !std::is_same<Outbound, T>::value;
}


