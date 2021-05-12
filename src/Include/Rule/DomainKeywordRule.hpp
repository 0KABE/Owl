#pragma once

#include "Rule.hpp"

namespace Owl {
    class DomainKeywordRule : public Rule {
    public:
        using DomainKeywordRulePtr = std::unique_ptr<DomainKeywordRule>;

        DomainKeywordRule(ProxyPtr proxyPtr, std::string keyword);

        bool Match(const Endpoint &endpoint) override;

        std::string RuleType() const override;

        std::string Condition() const override;

        static DomainKeywordRulePtr Create(const ProxyPtr &proxyPtr, const std::string &rule);

    private:
        std::string mKeyword;
    };
}


