#include "Proxy/ProxyNodeManager.hpp"
#include <exception>

Owl::Proxy::ProxyPtr Owl::ProxyNodeManager::GetProxy(const std::string &name) {
    auto it = mBuiltInProxyNodes.find(name);
    if (it == mBuiltInProxyNodes.end())
        throw std::runtime_error(fmt::format("Can not find {} in built-in proxies", name));
    return it->second;
}
