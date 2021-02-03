#include "Rule/DomainSuffixRule.hpp"
#include "Rule/RuleFactory.hpp"

Owl::DomainSuffixRule::DomainSuffixRule(Owl::Proxy::ProxyPtr proxyPtr, std::string suffix)
        : Rule(std::move(proxyPtr)), mSuffix(std::move(suffix)) {}

bool Owl::DomainSuffixRule::Match(const Owl::Endpoint &endpoint) {
    return endpoint.GetHostnameType() == Endpoint::HOST_DOMAIN &&
           endpoint.GetHostname().ends_with(mSuffix);
}

Owl::DomainSuffixRule::DomainSuffixRulePtr
Owl::DomainSuffixRule::Create(const Owl::Proxy::ProxyPtr &proxyPtr, const std::string &rule) {
    return std::make_unique<DomainSuffixRule>(proxyPtr, rule);
}

static Owl::RuleFactory::Register<Owl::DomainSuffixRule> r("DOMAIN-SUFFIX");