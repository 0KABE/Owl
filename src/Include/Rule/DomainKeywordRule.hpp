#pragma once

#include "Rule.hpp"

namespace Owl {
    class DomainKeywordRule : public Rule {
    public:
        using DomainKeywordRulePtr = std::unique_ptr<DomainKeywordRule>;

        DomainKeywordRule(ProxyNode::ProxyPtr proxyPtr, std::string keyword);

        bool Match(const Endpoint &endpoint) override;

        static DomainKeywordRulePtr Create(const ProxyNode::ProxyPtr &proxyPtr, const std::string &rule);

    private:
        std::string mKeyword;
    };
}


