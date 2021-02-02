#pragma once

#include "Rule.hpp"

namespace Owl {
    class IpCidrRule : public Rule {
    public:
        using IpCidrRulePtr = std::unique_ptr<IpCidrRule>;

        IpCidrRule(Proxy::ProxyPtr proxyPtr, uint32_t ip, uint32_t mask);

        bool Match(const Endpoint &endpoint) override;

        static IpCidrRulePtr Create(const Proxy::ProxyPtr &proxyPtr, const std::string &rule);

    private:
        uint32_t mIp;
        uint32_t mMask;
    };
}


