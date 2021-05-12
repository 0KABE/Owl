#include "Rule/RuleManager.hpp"
#include "Rule/RuleFactory.hpp"
#include "Proxy/ProxyNodeManager.hpp"

Owl::RuleManager::RuleManager() {
    ProxyPtr directProxyPtr = ProxyNodeManager::GetInstance().GetBuiltInProxyNode("DIRECT");
    mRuleCollection.push_back(RuleFactory::GetInstance().Create("FINAL", directProxyPtr, ""));
}

void Owl::RuleManager::AddRule(Owl::RulePtr rulePtr) { mRuleCollection.push_front(std::move(rulePtr)); }

Owl::ProxyPtr Owl::RuleManager::Match(const Owl::Endpoint &endpoint) const {
    auto iterator = std::find_if(mRuleCollection.begin(), mRuleCollection.end(),
                                 [&](const RulePtr &rule) { return rule->Match(endpoint); });
    return iterator->get()->GetProxyPtr();
}

void Owl::RuleManager::ClearRules() {
    mRuleCollection.clear();
}

const Owl::RuleManager::RuleCollection &Owl::RuleManager::GetRuleCollection() const {
    return mRuleCollection;
}

