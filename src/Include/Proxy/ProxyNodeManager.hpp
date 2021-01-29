#pragma once

#include "Proxy.hpp"
#include "Bound/Outbound/Direct.hpp"
#include "BuiltInProxyNode.hpp"

namespace Owl {
    class ProxyNodeManager {
    public:
        static ProxyNodeManager &GetInstance();

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
        std::unordered_map<std::string, Proxy::ProxyPtr> mBuiltInProxyNodes;
    };
}


