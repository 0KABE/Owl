#include "Rule/DomainSuffixRule.hpp"
#include "Rule/RuleFactory.hpp"

Owl::DomainSuffixRule::DomainSuffixRule(Owl::ProxyPtr proxyPtr, std::string suffix)
        : Rule(std::move(proxyPtr)), mSuffix(std::move(suffix)) {}

bool Owl::DomainSuffixRule::Match(const Owl::Endpoint &endpoint) {
    return endpoint.GetHostnameType() == Endpoint::HOST_DOMAIN &&
           endpoint.GetHostname().ends_with(mSuffix);
}

Owl::DomainSuffixRule::DomainSuffixRulePtr
Owl::DomainSuffixRule::Create(const Owl::ProxyPtr &proxyPtr, const std::string &rule) {
    return std::make_unique<DomainSuffixRule>(proxyPtr, rule);
}

std::string Owl::DomainSuffixRule::RuleType() const {
    return "DomainSuffix";
}

std::string Owl::DomainSuffixRule::Condition() const {
    return mSuffix;
}

static Owl::RuleFactory::Register<Owl::DomainSuffixRule> r("DOMAIN-SUFFIX");
