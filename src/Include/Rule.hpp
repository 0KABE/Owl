#pragma once

#include "ProxyNode.hpp"

namespace Owl {
    class Rule {
    public:
        using RulePtr = std::unique_ptr<Rule>;

        explicit Rule(ProxyNode::ProxyPtr proxyPtr);

        virtual ~Rule() = default;

        virtual bool Match(const Endpoint &endpoint) = 0;

        [[nodiscard]] const ProxyNode::ProxyPtr &GetProxyPtr() const;

    protected:
        ProxyNode::ProxyPtr mProxyPtr;
    };
}


