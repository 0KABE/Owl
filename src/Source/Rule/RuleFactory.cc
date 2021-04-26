#include "Rule/RuleFactory.hpp"

Owl::RulePtr
Owl::RuleFactory::Create(const std::string &type, Owl::ProxyPtr proxyPtr, std::string rule) {
    return mRulesGenerators[type](std::move(proxyPtr), std::move(rule));
}
