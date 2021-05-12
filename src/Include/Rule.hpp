#pragma once

#include "ProxyNode.hpp"

namespace Owl {
    class Rule {
    public:
        using RulePtr = std::unique_ptr<Rule>;

        explicit Rule(ProxyPtr proxyPtr);

        virtual ~Rule() = default;

        virtual bool Match(const Endpoint &endpoint) = 0;

        [[nodiscard]] virtual std::string RuleType() const = 0;

        [[nodiscard]] virtual std::string Condition() const = 0;

        [[nodiscard]] const ProxyPtr &GetProxyPtr() const;

    protected:
        ProxyPtr mProxyPtr;
    };

    using RulePtr = Rule::RulePtr;
}


