#pragma once

#include "ProxyGroup.hpp"

namespace Owl {
    class StaticProxyGroup : public ProxyGroup {
    public:
        static constexpr const char *TYPE = "static";

        [[nodiscard]] std::string GetPolicyType() const override;

        [[nodiscard]] Outbound::BoundPtr GetOutbound(Endpoint endpoint) const override;
    };
}
