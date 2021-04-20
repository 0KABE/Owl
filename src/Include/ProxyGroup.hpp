#pragma once

#include "ProxyNode.hpp"

namespace Owl {
    class ProxyGroup : public ProxyNode,
                       public std::enable_shared_from_this<ProxyGroup>,
                       public EnableSharedInstance<ProxyGroup> {
    public:
        using ProxyGroupPtr = std::shared_ptr<ProxyGroup>;
        using ProxyNode::ProxyPtr;
        enum Status {
            RUNNING, CLOSED
        };

        void SetSelectedProxy(ProxyPtr &selectedProxyPtr);

        const ProxyPtr &GetSelectedProxy() const;

        [[nodiscard]] virtual std::string GetPolicyType() const = 0;

        virtual void Start(net::executor &executor);

        void Close();

    protected:
        ProxyGroup(std::string name, std::vector<ProxyPtr> proxies);

        std::vector<ProxyPtr> mProxies;
        ProxyPtr mSelectedProxy;
        Status mStatus = RUNNING;
    };
}


