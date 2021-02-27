#pragma once

#include "Rule.hpp"

namespace Owl {
    class DomainSuffixRule : public Rule {
    public:
        using DomainSuffixRulePtr = std::unique_ptr<DomainSuffixRule>;

        DomainSuffixRule(ProxyNode::ProxyPtr proxyPtr, std::string suffix);

        bool Match(const Endpoint &endpoint) override;

        static DomainSuffixRulePtr Create(const ProxyNode::ProxyPtr &proxyPtr, const std::string &rule);

    private:
        std::string mSuffix;
    };
}


