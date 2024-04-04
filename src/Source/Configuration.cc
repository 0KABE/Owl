#include <boost/locale.hpp>
#include <fstream>
#include "Configuration.hpp"
#include "Configuration/ConfigurationParser.hpp"
#include "Rule/RuleManager.hpp"
#include "Rule/RuleFactory.hpp"
#include "Proxy/ProxyNodeManager.hpp"
#include "Proxy/PolicyBuilder.hpp"
#include "Proxy/ExternalProxyNode.hpp"
#include "Pipeline/Crypto/CryptoFactory.hpp"
#include "Pipeline/SimpleObfsClient/SimpleObfsClientFactory.hpp"
#include <spdlog/spdlog.h>
#include <Proxy/StaticProxyGroup.hpp>

void Owl::Configuration::Load(const std::string &path, net::any_io_executor executor) {
    spdlog::info("Loading configuration: {}", path);

    ConfigurationParser parser(path);
    ConfInfo confInfo = parser.Parse();
    Register(confInfo, executor);
}

std::string Owl::Configuration::GetConfigContent(const std::string &path) {
    std::ifstream ifStream(path);
    const std::string content((std::istreambuf_iterator<char>(ifStream)), std::istreambuf_iterator<char>());
    return content;
}

Owl::Outbound::BoundPtr Owl::Configuration::Match(Owl::Endpoint endpoint) {
    ProxyPtr proxyPtr = RuleManager::GetInstance().Match(endpoint);
    spdlog::info("{} matches rule {}", endpoint.ToString(), proxyPtr->GetName());
    return proxyPtr->GetOutbound(std::move(endpoint));
}

void Owl::Configuration::Register(const ConfInfo &confInfo, net::any_io_executor executor) {
    RegisterProxy(confInfo);
    RegisterPolicy(confInfo, executor);
    RegisterRule(confInfo);
}

void Owl::Configuration::RegisterRule(const Owl::ConfInfo &confInfo) {
    ProxyNodeManager &proxyNodeManager = ProxyNodeManager::GetInstance();
    RuleManager &ruleManager = RuleManager::GetInstance();
    RuleFactory &ruleFactory = RuleFactory::GetInstance();

    // For multiple invocation case
    ruleManager.ClearRules();

    std::for_each(confInfo.rules.rbegin(), confInfo.rules.rend(), [&](const RuleInfo &ruleInfo) {
        std::string type = boost::locale::conv::utf_to_utf<char>(ruleInfo.type);
        std::string value = boost::locale::conv::utf_to_utf<char>(ruleInfo.value);
        std::string policy = boost::locale::conv::utf_to_utf<char>(ruleInfo.policy);
        ruleManager.AddRule(ruleFactory.Create(type, proxyNodeManager.GetProxyOrPolicy(policy), value));
    });
}

void Owl::Configuration::RegisterProxy(const Owl::ConfInfo &confInfo) {
    using boost::locale::conv::utf_to_utf;

    ProxyNodeManager &proxyNodeManager = ProxyNodeManager::GetInstance();

    // For multiple invocation case
    proxyNodeManager.ClearProxyNodes();

    std::for_each(confInfo.proxies.begin(), confInfo.proxies.end(), [&](const ProxyInfo &proxyInfo) {
        std::string name = utf_to_utf<char>(proxyInfo.name);
        std::string protocol = utf_to_utf<char>(proxyInfo.protocol);
        std::string server = utf_to_utf<char>(proxyInfo.server);
        std::string port = std::to_string(proxyInfo.port);
        ProxyPtr proxyPtr
                = std::make_shared<ExternalProxyNode>(name, server, port, BuildProxyPipelineGenerators(proxyInfo));
        proxyNodeManager.AddProxyNode(proxyPtr);
    });
}

void Owl::Configuration::RegisterPolicy(const ConfInfo &confInfo, net::any_io_executor executor) {
    using boost::locale::conv::utf_to_utf;
    ProxyNodeManager &proxyNodeManager = ProxyNodeManager::GetInstance();

    // For multiple invocation case
    proxyNodeManager.ClearPolicies();

    //Register all polices
    for (const PolicyInfo &policyInfo : confInfo.policies) {
        PolicyBuilder policyBuilder;
        policyBuilder.SetName(utf_to_utf<char>(policyInfo.name));

        //Exclude Static Policy
        if (utf_to_utf<char>(policyInfo.type) != StaticProxyGroup::TYPE) {
            if (policyInfo.properties.contains(U"interval")) {
                std::string interval = utf_to_utf<char>(policyInfo.properties.at(U"interval"));
                policyBuilder.SetPeriod(std::chrono::seconds(std::stoul(interval)));
            }
            if (policyInfo.properties.contains(U"url")) {
                std::string url = utf_to_utf<char>(policyInfo.properties.at(U"url"));
                policyBuilder.SetUrl(url);
            }
        }
        proxyNodeManager.AddPolicy(policyBuilder.Build(utf_to_utf<char>(policyInfo.type)));
    }

    //Update relative proxies
    for (const PolicyInfo &policyInfo : confInfo.policies) {
        PolicyPtr proxyGroupPtr = proxyNodeManager.GetPolicy(utf_to_utf<char>(policyInfo.name));

        std::vector<ProxyPtr> proxies;
        for (const std::u32string &proxyName : policyInfo.proxies) {
            ProxyPtr proxy = proxyNodeManager.GetProxyOrPolicy(utf_to_utf<char>(proxyName));
            proxies.push_back(proxy);
        }

        proxyGroupPtr->SetProxies(proxies);
        proxyGroupPtr->Start(executor);
    }
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

