#include "Policy.hpp"

#include <utility>

void Owl::Policy::SetSelectedProxy(ProxyPtr &selectedProxyPtr) {
    bool exist = std::any_of(mProxies.begin(), mProxies.end(),
                             [&](const ProxyPtr &proxyNode) { return proxyNode == selectedProxyPtr; });
    if (!exist) {
        throw std::invalid_argument(
                fmt::format("Policy does not contain {}", selectedProxyPtr->GetName()));
    }
    mSelectedProxy = selectedProxyPtr;
}

void Owl::Policy::SetProxies(const std::vector<ProxyPtr> &proxies) {
    mProxies = proxies;
    mSelectedProxy = mProxies.empty() ? nullptr : mProxies.front();
}

const Owl::ProxyPtr &Owl::Policy::GetSelectedProxy() const { return mSelectedProxy; }

void Owl::Policy::Start(const net::executor &executor) {}

void Owl::Policy::Close() { mStatus = CLOSED; }

Owl::Policy::Policy(std::string name, std::vector<ProxyPtr> proxies)
        : ProxyNode(std::move(name)), mProxies(std::move(proxies)) {}
