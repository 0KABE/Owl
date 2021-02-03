#include "Rule/DomainKeywordRule.hpp"
#include "Rule/RuleFactory.hpp"


bool Owl::DomainKeywordRule::Match(const Owl::Endpoint &endpoint) {
    return endpoint.GetHostnameType() == Endpoint::HOST_DOMAIN &&
           endpoint.GetHostname().find(mKeyword) != std::string::npos;
}

Owl::DomainKeywordRule::DomainKeywordRule(Owl::Proxy::ProxyPtr proxyPtr, std::string keyword)
        : Rule(std::move(proxyPtr)), mKeyword(std::move(keyword)) {}

Owl::DomainKeywordRule::DomainKeywordRulePtr
Owl::DomainKeywordRule::Create(const Owl::Proxy::ProxyPtr &proxyPtr, const std::string &rule) {
    return std::make_unique<DomainKeywordRule>(proxyPtr, rule);
}

static Owl::RuleFactory::Register<Owl::DomainKeywordRule> r("DOMAIN-KEYWORD");
