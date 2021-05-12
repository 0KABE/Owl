#pragma once

#include "Rule.hpp"

namespace Owl {
    class DomainSuffixRule : public Rule {
    public:
        using DomainSuffixRulePtr = std::unique_ptr<DomainSuffixRule>;

        DomainSuffixRule(ProxyPtr proxyPtr, std::string suffix);

        std::string RuleType() const override;

        std::string Condition() const override;

        bool Match(const Endpoint &endpoint) override;

        static DomainSuffixRulePtr Create(const ProxyPtr &proxyPtr, const std::string &rule);

    private:
        std::string mSuffix;
    };
}


