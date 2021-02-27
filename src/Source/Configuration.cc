#include <boost/locale.hpp>
#include "Configuration.hpp"
#include "Configuration/ConfigurationParser.hpp"
#include "Rule/RuleManager.hpp"
#include "Rule/RuleFactory.hpp"
#include "Proxy/ProxyNodeManager.hpp"
#include <spdlog/spdlog.h>

void Owl::Configuration::Load(const std::string &path) {
    spdlog::info("Loading configuration: {}", path);

    ConfigurationParser parser(path);
    ConfInfo confInfo = parser.Parse();

    RuleManager &ruleManager = RuleManager::GetInstance();
    RuleFactory &ruleFactory = RuleFactory::GetInstance();
    ProxyNodeManager &proxyNodeManager = ProxyNodeManager::GetInstance();

    std::for_each(confInfo.rules.rbegin(), confInfo.rules.rend(), [&](const RuleInfo &ruleInfo) {
        std::string type = boost::locale::conv::utf_to_utf<char>(ruleInfo.type);
        std::string value = boost::locale::conv::utf_to_utf<char>(ruleInfo.value);
        std::string policy = boost::locale::conv::utf_to_utf<char>(ruleInfo.policy);
        ruleManager.AddRule(ruleFactory.Create(type, proxyNodeManager.GetProxy(policy), value));
    });
}

Owl::Outbound::BoundPtr Owl::Configuration::Match(Owl::Endpoint endpoint) {
    ProxyNode::ProxyPtr proxyPtr = RuleManager::GetInstance().Match(endpoint);
    spdlog::info("{} matches rule {}", endpoint.ToString(), proxyPtr->GetName());
    return proxyPtr->GetOutbound(std::move(endpoint));
}
