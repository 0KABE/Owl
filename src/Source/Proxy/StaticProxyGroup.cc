#include "Proxy/StaticProxyGroup.hpp"
#include "Proxy/PolicyBuilder.hpp"

Owl::StaticProxyGroup::StaticProxyGroup(const std::string &name,
                                        const std::vector<ProxyPtr> &proxies,
                                        const std::chrono::seconds &period,
                                        std::string url) : Policy(name, proxies) {}

std::string Owl::StaticProxyGroup::GetPolicyType() const {
    return TYPE;
}

Owl::Outbound::BoundPtr Owl::StaticProxyGroup::GetOutbound(Owl::Endpoint endpoint) const {
    return mSelectedProxy->GetOutbound(std::move(endpoint));
}

static Owl::PolicyBuilder::Register<Owl::StaticProxyGroup> r(Owl::StaticProxyGroup::TYPE);
