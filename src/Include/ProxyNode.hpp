#pragma once

#include "Util/Endpoint.hpp"
#include "Bound/Outbound.hpp"

namespace Owl {
    class ProxyNode {
    public:
        using ProxyPtr = std::shared_ptr<ProxyNode>;

        explicit ProxyNode(std::string name) : mName(std::move(name)) {}

        [[nodiscard]] virtual const std::string &GetName() const = 0;

        [[nodiscard]] virtual Outbound::BoundPtr GetOutbound(Endpoint endpoint) const = 0;

        virtual ~ProxyNode() = default;

    protected:
        std::string mName;
    };
}


