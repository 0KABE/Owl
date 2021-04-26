#include "Proxy/ProxyNodeManager.hpp"
#include <exception>

Owl::ProxyNode::ProxyPtr Owl::ProxyNodeManager::GetBuiltInProxyNode(const std::string &name) {
    return mBuiltInProxyNodes.at(name);
}

Owl::ProxyNode::ProxyPtr Owl::ProxyNodeManager::GetProxyNode(const std::string &name) {
    auto it = mProxyNodes.find(name);
    if (it == mProxyNodes.end())
        throw std::runtime_error(fmt::format("Can not find {} in external proxies", name));
    return it->second;
}

void Owl::ProxyNodeManager::AddProxyNode(const Owl::ProxyNode::ProxyPtr &proxyPtr) {
    mProxyNodes[proxyPtr->GetName()] = proxyPtr;
}

Owl::ProxyGroup::ProxyGroupPtr Owl::ProxyNodeManager::GetPolicy(const std::string &name) {
    return mPolicies.at(name);
}

void Owl::ProxyNodeManager::AddPolicy(const Owl::ProxyGroup::ProxyGroupPtr &proxyGroupPtr) {
    mPolicies[proxyGroupPtr->GetName()] = proxyGroupPtr;
}

Owl::ProxyNode::ProxyPtr Owl::ProxyNodeManager::GetProxyOrPolicy(const std::string &name) {
    if (mBuiltInProxyNodes.contains(name)) return mBuiltInProxyNodes[name];
    else if (mProxyNodes.contains(name)) return mProxyNodes[name];
    else if (mPolicies.contains(name)) return mPolicies[name];

    throw std::invalid_argument(fmt::format("{} is not in BuiltProxyNode/ExternalProxyNodes/Policies", name));
}
