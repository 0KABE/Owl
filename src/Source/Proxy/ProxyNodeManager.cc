#include "Proxy/ProxyNodeManager.hpp"
#include <exception>

Owl::ProxyPtr Owl::ProxyNodeManager::GetBuiltInProxyNode(const std::string &name) {
    return mBuiltInProxyNodes.at(name);
}

Owl::ProxyPtr Owl::ProxyNodeManager::GetProxyNode(const std::string &name) {
    auto it = mProxyNodes.find(name);
    if (it == mProxyNodes.end())
        throw std::runtime_error(fmt::format("Can not find {} in external proxies", name));
    return it->second;
}

void Owl::ProxyNodeManager::AddProxyNode(const Owl::ProxyPtr &proxyPtr) {
    mProxyNodes[proxyPtr->GetName()] = proxyPtr;
}

Owl::PolicyPtr Owl::ProxyNodeManager::GetPolicy(const std::string &name) {
    return mPolicies.at(name);
}

void Owl::ProxyNodeManager::AddPolicy(const Owl::PolicyPtr &proxyGroupPtr) {
    mPolicies[proxyGroupPtr->GetName()] = proxyGroupPtr;
}

Owl::ProxyPtr Owl::ProxyNodeManager::GetProxyOrPolicy(const std::string &name) {
    if (mBuiltInProxyNodes.contains(name)) return mBuiltInProxyNodes[name];
    else if (mProxyNodes.contains(name)) return mProxyNodes[name];
    else if (mPolicies.contains(name)) return mPolicies[name];

    throw std::invalid_argument(fmt::format("{} is not in BuiltProxyNode/ExternalProxyNodes/Policies", name));
}

const std::unordered_map<std::string, Owl::PolicyPtr> &Owl::ProxyNodeManager::GetPolicies() const {
    return mPolicies;
}
