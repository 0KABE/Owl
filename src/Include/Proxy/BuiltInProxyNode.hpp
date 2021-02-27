#pragma once

#include "Util/Concepts.hpp"
#include "ProxyNode.hpp"
#include "Bound/BoundFactory.hpp"

namespace Owl {
    template<typename T> requires Derived<Outbound, T>
    class BuiltInProxyNode : public ProxyNode {
    public:
        using ProxyPtr = std::shared_ptr<BuiltInProxyNode>;

        using ProxyNode::ProxyNode;

        [[nodiscard]] const std::string &GetName() const override { return mName; }

        [[nodiscard]] Outbound::BoundPtr GetOutbound(Endpoint endpoint) const override {
            return BoundFactory::Create<T>(std::move(endpoint));
        }

    };


}


