#pragma once

#include "Policy.hpp"

namespace Owl {
    class AutoProxyGroup : public Policy {
    public:
        static constexpr const char *TYPE = "auto";

        AutoProxyGroup(const std::string &name, const std::vector<ProxyPtr> &proxies,
                       const std::chrono::seconds &period, std::string url);

        std::string GetPolicyType() const override;

        Outbound::BoundPtr GetOutbound(Endpoint endpoint) const override;

        void Start(net::any_io_executor executor) override;

    private:

        Awaitable<void> ConnectivityTest();

        std::chrono::seconds mPeriod;
        std::string mUrl;
    };
}


