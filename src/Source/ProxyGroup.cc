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

const Owl::ProxyNode::ProxyPtr &Owl::ProxyGroup::GetSelectedProxy() const { return mSelectedProxy; }

void Owl::ProxyGroup::Start(boost::asio::executor &executor) {}

void Owl::ProxyGroup::Close() { mStatus = CLOSED; }

Owl::ProxyGroup::ProxyGroup(std::string name, std::vector<ProxyPtr> proxies)
        : ProxyNode(std::move(name)), mProxies(std::move(proxies)), mSelectedProxy(proxies.at(0)) {}
