#include "Proxy/ProxyNodeManager.hpp"
#include <exception>

Owl::ProxyNode::ProxyPtr Owl::ProxyNodeManager::GetProxy(const std::string &name) {
    auto it = mBuiltInProxyNodes.find(name);
    if (it == mBuiltInProxyNodes.end())
        throw std::runtime_error(fmt::format("Can not find {} in built-in proxies", name));
    return it->second;
}

void Owl::ProxyNodeManager::AddProxy(const Owl::ProxyNode::ProxyPtr &proxyPtr) {
    mBuiltInProxyNodes[proxyPtr->GetName()] = proxyPtr;
}

