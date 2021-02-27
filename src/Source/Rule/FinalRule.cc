#include "Rule/FinalRule.hpp"
#include "Rule/RuleFactory.hpp"

bool Owl::FinalRule::Match(const Owl::Endpoint &endpoint) { return true; }

Owl::FinalRule::FinalRulePtr Owl::FinalRule::Create(const Owl::ProxyNode::ProxyPtr &proxyPtr, const std::string &rule) {
    return std::make_unique<FinalRule>(proxyPtr);
}

static Owl::RuleFactory::Register<Owl::FinalRule> r("FINAL");
