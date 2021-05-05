#pragma once

#include <Policy.hpp>
#include "ProxyNode.hpp"
#include "Bound/Outbound/Direct.hpp"
#include "BuiltInProxyNode.hpp"
#include "Util/Singleton.hpp"

namespace Owl {
    class ProxyNodeManager : public Singleton<ProxyNodeManager> {
        friend Singleton<ProxyNodeManager>;
    public:
        using Policies = std::unordered_map<std::string, PolicyPtr>;
        using Proxies = std::unordered_map<std::string, ProxyPtr>;

        ProxyPtr GetBuiltInProxyNode(const std::string &name);

        ProxyPtr GetProxyNode(const std::string &name);

        void AddProxyNode(const ProxyPtr &proxyPtr);

        PolicyPtr GetPolicy(const std::string &name);

        void AddPolicy(const PolicyPtr &proxyGroupPtr);

        ProxyPtr GetProxyOrPolicy(const std::string &name);

        [[nodiscard]] const std::unordered_map<std::string, PolicyPtr> &GetPolicies() const;

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

        Proxies mBuiltInProxyNodes;
        Proxies mProxyNodes;
        Policies mPolicies;
    };
}


