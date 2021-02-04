#include "Rule/RuleFactory.hpp"

Owl::Rule::RulePtr Owl::RuleFactory::Create(const std::string &type, Owl::Proxy::ProxyPtr proxyPtr, std::string rule) {
    return mRulesGenerators[type](std::move(proxyPtr), std::move(rule));
}
