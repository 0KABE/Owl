#pragma once

#include "Util/Endpoint.hpp"
#include "Bound/Outbound.hpp"

namespace Owl {
    class Proxy {
    public:
        using ProxyPtr = std::shared_ptr<Proxy>;

        [[nodiscard]] virtual const std::string &GetName() const = 0;

        [[nodiscard]] virtual Outbound::BoundPtr GetOutbound(Endpoint endpoint) const = 0;

        virtual ~Proxy() = default;
    };
}


