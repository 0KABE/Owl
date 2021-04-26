#pragma once

#include "ProxyNode.hpp"

namespace Owl {
    //TODO rename to Policy
    class ProxyGroup : public ProxyNode,
                       public std::enable_shared_from_this<ProxyGroup>,
                       public EnableSharedInstance<ProxyGroup> {
    public:
        using ProxyGroupPtr = std::shared_ptr<ProxyGroup>;
        using ProxyNode::ProxyPtr;
        enum Status {
            RUNNING, CLOSED
        };

        void SetProxies(const std::vector<ProxyPtr> &proxies);

        void SetSelectedProxy(ProxyPtr &selectedProxyPtr);

        //TODO need to guarantee selectedProxy is not null
        const ProxyPtr &GetSelectedProxy() const;

        [[nodiscard]] virtual std::string GetPolicyType() const = 0;

        virtual void Start(const net::executor &executor);

        void Close();

    protected:
        ProxyGroup(std::string name, std::vector<ProxyPtr> proxies);

        std::vector<ProxyPtr> mProxies;
        ProxyPtr mSelectedProxy;
        Status mStatus = RUNNING;
    };
}


