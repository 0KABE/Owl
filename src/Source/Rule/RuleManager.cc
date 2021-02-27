#include "Rule/RuleManager.hpp"
#include "Rule/RuleFactory.hpp"
#include "Proxy/ProxyNodeManager.hpp"

Owl::RuleManager::RuleManager() {
    ProxyNode::ProxyPtr directProxyPtr = ProxyNodeManager::GetInstance().GetProxy("DIRECT");
    mRuleCollection.push_back(RuleFactory::GetInstance().Create("FINAL", directProxyPtr, ""));
}

void Owl::RuleManager::AddRule(Owl::Rule::RulePtr rulePtr) { mRuleCollection.push_front(std::move(rulePtr)); }

Owl::ProxyNode::ProxyPtr Owl::RuleManager::Match(const Owl::Endpoint &endpoint) const {
    auto iterator = std::find_if(mRuleCollection.begin(), mRuleCollection.end(),
                                 [&](const Rule::RulePtr &rule) { return rule->Match(endpoint); });
    return iterator->get()->GetProxyPtr();
}

