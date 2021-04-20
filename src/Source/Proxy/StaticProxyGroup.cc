#include "Proxy/StaticProxyGroup.hpp"

std::string Owl::StaticProxyGroup::GetPolicyType() const {
    return TYPE;
}

Owl::Outbound::BoundPtr Owl::StaticProxyGroup::GetOutbound(Owl::Endpoint endpoint) const {
    return mSelectedProxy->GetOutbound(std::move(endpoint));
}
