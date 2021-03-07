#include <boost/locale.hpp>
#include "Configuration.hpp"
#include "Configuration/ConfigurationParser.hpp"
#include "Rule/RuleManager.hpp"
#include "Rule/RuleFactory.hpp"
#include "Proxy/ProxyNodeManager.hpp"
#include "Proxy/ExternalProxyNode.hpp"
#include "Pipeline/Crypto/CryptoFactory.hpp"
#include "Pipeline/SimpleObfsClient/SimpleObfsClientFactory.hpp"
#include <spdlog/spdlog.h>

void Owl::Configuration::Load(const std::string &path) {
    spdlog::info("Loading configuration: {}", path);

    ConfigurationParser parser(path);
    ConfInfo confInfo = parser.Parse();

    RegisterProxy(confInfo);
    RegisterRule(confInfo);
}

Owl::Outbound::BoundPtr Owl::Configuration::Match(Owl::Endpoint endpoint) {
    ProxyNode::ProxyPtr proxyPtr = RuleManager::GetInstance().Match(endpoint);
    spdlog::info("{} matches rule {}", endpoint.ToString(), proxyPtr->GetName());
    return proxyPtr->GetOutbound(std::move(endpoint));
}

void Owl::Configuration::RegisterRule(const Owl::ConfInfo &confInfo) {
    ProxyNodeManager &proxyNodeManager = ProxyNodeManager::GetInstance();
    RuleManager &ruleManager = RuleManager::GetInstance();
    RuleFactory &ruleFactory = RuleFactory::GetInstance();

    std::for_each(confInfo.rules.rbegin(), confInfo.rules.rend(), [&](const RuleInfo &ruleInfo) {
        std::string type = boost::locale::conv::utf_to_utf<char>(ruleInfo.type);
        std::string value = boost::locale::conv::utf_to_utf<char>(ruleInfo.value);
        std::string policy = boost::locale::conv::utf_to_utf<char>(ruleInfo.policy);
        ruleManager.AddRule(ruleFactory.Create(type, proxyNodeManager.GetProxy(policy), value));
    });
}

void Owl::Configuration::RegisterProxy(const Owl::ConfInfo &confInfo) {
    using boost::locale::conv::utf_to_utf;

    ProxyNodeManager &proxyNodeManager = ProxyNodeManager::GetInstance();

    std::for_each(confInfo.proxies.begin(), confInfo.proxies.end(), [&](const ProxyInfo &proxyInfo) {
        std::string name = utf_to_utf<char>(proxyInfo.name);
        std::string protocol = utf_to_utf<char>(proxyInfo.protocol);
        std::string server = utf_to_utf<char>(proxyInfo.server);
        std::string port = std::to_string(proxyInfo.port);
        ExternalProxyNode::ProxyPtr proxyPtr
                = std::make_shared<ExternalProxyNode>(name, server, port, BuildProxyPipelineGenerators(proxyInfo));
        proxyNodeManager.AddProxy(proxyPtr);
    });
}

Owl::ExternalProxyNode::PipelineGenerators
Owl::Configuration::BuildProxyPipelineGenerators(const Owl::ProxyInfo &proxyInfo) {
    using boost::locale::conv::utf_to_utf;

    ExternalProxyNode::PipelineGenerators pipelineGenerators;

    if (proxyInfo.properties.contains(METHOD) && proxyInfo.properties.contains(PASSWORD)) {
        std::string method = utf_to_utf<char>(proxyInfo.properties.at(METHOD));
        std::string password = utf_to_utf<char>(proxyInfo.properties.at(PASSWORD));
        pipelineGenerators.push_back([=] {
            return CryptoFactory::GetInstance().Build(method, password);
        });
    } else {
        throw std::invalid_argument(
                fmt::format("Unacceptable proxy {}, no property {}",
                            utf_to_utf<char>(proxyInfo.name),
                            proxyInfo.properties.contains(METHOD)
                            ? utf_to_utf<char>(PASSWORD)
                            : utf_to_utf<char>(METHOD)));
    }

    if (proxyInfo.properties.contains(OBFS)) {
        std::string obfsType = utf_to_utf<char>(proxyInfo.properties.at(OBFS));
        std::string obfsHost = utf_to_utf<char>(proxyInfo.properties.at(OBFS_HOST));
        pipelineGenerators.push_back([=] {
            return SimpleObfsClientFactory::GetInstance().Build(obfsType, obfsHost);
        });
    }

    return pipelineGenerators;
}

