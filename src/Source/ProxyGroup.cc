#include "ProxyGroup.hpp"

#include <utility>

void Owl::ProxyGroup::SetSelectedProxy(ProxyPtr &selectedProxyPtr) {
    bool exist = std::any_of(mProxies.begin(), mProxies.end(),
                             [&](const ProxyPtr &proxyNode) { return proxyNode == selectedProxyPtr; });
    if (!exist) {
        throw std::invalid_argument(
                fmt::format("ProxyGroup does not contain {}", selectedProxyPtr->GetName()));
    }
    mSelectedProxy = selectedProxyPtr;
}

void Owl::ProxyGroup::SetProxies(const std::vector<ProxyPtr> &proxies) {
    mProxies = proxies;
    mSelectedProxy = mProxies.empty() ? nullptr : mProxies.front();
}

const Owl::ProxyNode::ProxyPtr &Owl::ProxyGroup::GetSelectedProxy() const { return mSelectedProxy; }

void Owl::ProxyGroup::Start(const net::executor &executor) {}

void Owl::ProxyGroup::Close() { mStatus = CLOSED; }

Owl::ProxyGroup::ProxyGroup(std::string name, std::vector<ProxyPtr> proxies)
        : ProxyNode(std::move(name)), mProxies(std::move(proxies)) {}
