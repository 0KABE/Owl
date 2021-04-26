#pragma once

#include <ProxyGroup.hpp>
#include "ProxyNode.hpp"
#include "Bound/Outbound/Direct.hpp"
#include "BuiltInProxyNode.hpp"
#include "Util/Singleton.hpp"

namespace Owl {
    class ProxyNodeManager : public Singleton<ProxyNodeManager> {
        friend Singleton<ProxyNodeManager>;
    public:
        ProxyNode::ProxyPtr GetBuiltInProxyNode(const std::string &name);

        ProxyNode::ProxyPtr GetProxyNode(const std::string &name);

        void AddProxyNode(const ProxyNode::ProxyPtr &proxyPtr);

        ProxyGroup::ProxyGroupPtr GetPolicy(const std::string &name);

        void AddPolicy(const ProxyGroup::ProxyGroupPtr &proxyGroupPtr);

        ProxyNode::ProxyPtr GetProxyOrPolicy(const std::string &name);

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

        std::unordered_map<std::string, ProxyNode::ProxyPtr> mBuiltInProxyNodes;
        std::unordered_map<std::string, ProxyNode::ProxyPtr> mProxyNodes;
        std::unordered_map<std::string, ProxyGroup::ProxyGroupPtr> mPolicies;
    };
}


