#pragma once

#include "Rule.hpp"

namespace Owl {
    class FinalRule : public Rule {
    public:
        using Rule::Rule;
        using FinalRulePtr = std::unique_ptr<FinalRule>;

        bool Match(const Endpoint &endpoint) override;

        static FinalRulePtr Create(const Proxy::ProxyPtr &proxyPtr, const std::string &rule);
    };
}


