#pragma once

#include "Proxy.hpp"
#include "Bound/Outbound/Direct.hpp"
#include "BuiltInProxyNode.hpp"
#include "Util/Singleton.hpp"

namespace Owl {
    class ProxyNodeManager : public Singleton<ProxyNodeManager> {
        friend Singleton<ProxyNodeManager>;
    public:
        Proxy::ProxyPtr GetProxy(const std::string &name);

        template<typename T>
        struct BuiltInRegister {
            explicit BuiltInRegister(const std::string &name) {
                ProxyNodeManager &proxyNodeFactory = GetInstance();
                auto proxy = std::make_shared<BuiltInProxyNode<T>>(name);
                proxyNodeFactory.mBuiltInProxyNodes[name] = proxy;
            }
        };

    private:
        ProxyNodeManager() = default;

        std::unordered_map<std::string, Proxy::ProxyPtr> mBuiltInProxyNodes;
    };
}


