#pragma once

#include "Proxy.hpp"

namespace Owl {
    class Rule {
    public:
        using RulePtr = std::unique_ptr<Rule>;

        explicit Rule(Proxy::ProxyPtr proxyPtr);

        virtual ~Rule() = default;

        virtual bool Match(const Endpoint &endpoint) = 0;

        [[nodiscard]] const Proxy::ProxyPtr &GetProxyPtr() const;

    protected:
        Proxy::ProxyPtr mProxyPtr;
    };
}


