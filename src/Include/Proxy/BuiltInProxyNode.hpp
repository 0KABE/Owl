#pragma once

#include "Util/Concepts.hpp"
#include "Proxy.hpp"
#include "BoundFactory.hpp"

namespace Owl {
    template<typename T> requires Derived<Outbound, T>
    class BuiltInProxyNode : public Proxy {
    public:
        using ProxyPtr = std::shared_ptr<BuiltInProxyNode>;

        explicit BuiltInProxyNode(std::string name) : mName(std::move(name)) {}

        [[nodiscard]] const std::string &GetName() const override { return mName; }

        [[nodiscard]] Outbound::BoundPtr GetOutbound(Endpoint endpoint) const override {
            return BoundFactory::Create<T>(std::move(endpoint));
        }

    private:
        std::string mName;
    };


}


