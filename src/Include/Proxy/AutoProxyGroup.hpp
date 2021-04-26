#pragma once

#include "ProxyGroup.hpp"

namespace Owl {
    class AutoProxyGroup : public ProxyGroup {
    public:
        static constexpr const char *TYPE = "auto";

        AutoProxyGroup(const std::string &name, const std::vector<ProxyPtr> &proxies,
                       const std::chrono::seconds &period, std::string url);

        std::string GetPolicyType() const override;

        Outbound::BoundPtr GetOutbound(Endpoint endpoint) const override;

        void Start(const net::executor &executor) override;

    private:

        Awaitable<void> ConnectivityTest();

        std::chrono::seconds mPeriod;
        std::string mUrl;
    };
}


