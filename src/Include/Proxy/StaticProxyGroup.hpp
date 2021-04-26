#pragma once

#include "Policy.hpp"

namespace Owl {
    class StaticProxyGroup : public Policy {
    public:
        static constexpr const char *TYPE = "static";

        StaticProxyGroup(const std::string &name, const std::vector<ProxyPtr> &proxies,
                         const std::chrono::seconds &period, std::string url);

        [[nodiscard]] std::string GetPolicyType() const override;

        [[nodiscard]] Outbound::BoundPtr GetOutbound(Endpoint endpoint) const override;
    };
}
