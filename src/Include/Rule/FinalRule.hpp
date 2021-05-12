#pragma once

#include "Rule.hpp"

namespace Owl {
    class FinalRule : public Rule {
    public:
        using Rule::Rule;
        using FinalRulePtr = std::unique_ptr<FinalRule>;

        std::string RuleType() const override;

        std::string Condition() const override;

        bool Match(const Endpoint &endpoint) override;

        static FinalRulePtr Create(const ProxyPtr &proxyPtr, const std::string &rule);
    };
}


