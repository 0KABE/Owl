#pragma once

#include "Util/Endpoint.hpp"
#include "Bound/Outbound.hpp"

namespace Owl {
    class ProxyNode {
    public:
        using ProxyPtr = std::shared_ptr<ProxyNode>;

        explicit ProxyNode(std::string name) : mName(std::move(name)) {}

        [[nodiscard]] const std::string &GetName() const;

        [[nodiscard]] virtual Outbound::BoundPtr GetOutbound(Endpoint endpoint) const = 0;

        virtual ~ProxyNode() = default;

    protected:
        std::string mName;
    };
}


