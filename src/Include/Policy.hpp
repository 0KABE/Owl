#pragma once

#include "ProxyNode.hpp"

namespace Owl {
    class Policy : public ProxyNode,
                   public std::enable_shared_from_this<Policy>,
                   public EnableSharedInstance<Policy> {
    public:
        using PolicyPtr = std::shared_ptr<Policy>;
        enum Status {
            RUNNING, CLOSED
        };

        void SetProxies(const std::vector<ProxyPtr> &proxies);

        void SetSelectedProxy(ProxyPtr &selectedProxyPtr);

        //TODO need to guarantee selectedProxy is not null
        const ProxyPtr &GetSelectedProxy() const;

        const std::vector<ProxyPtr> &GetProxies() const;

        [[nodiscard]] virtual std::string GetPolicyType() const = 0;

        virtual void Start(const net::executor &executor);

        void Close();

    protected:
        Policy(std::string name, std::vector<ProxyPtr> proxies);

        std::vector<ProxyPtr> mProxies;
        ProxyPtr mSelectedProxy;
        Status mStatus = RUNNING;
    };

    using PolicyPtr = Policy::PolicyPtr;
}


