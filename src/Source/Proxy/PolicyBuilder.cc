#include <utility>

#include "Proxy/PolicyBuilder.hpp"

Owl::PolicyBuilder &Owl::PolicyBuilder::SetPeriod(std::chrono::seconds period) {
    mPeriod = period;
    return *this;
}

Owl::PolicyBuilder &Owl::PolicyBuilder::AddProxy(const Owl::ProxyPtr &proxyPtr) {
    mProxies.push_back(proxyPtr);
    return *this;
}

Owl::PolicyBuilder &Owl::PolicyBuilder::SetName(std::string name) {
    mName = std::move(name);
    return *this;
}

Owl::PolicyBuilder &Owl::PolicyBuilder::SetUrl(std::string url) {
    mUrl = std::move(url);
    return *this;
}

Owl::PolicyPtr Owl::PolicyBuilder::Build(const std::string &type) {
    PolicyStore &policyStore = PolicyStore::GetInstance();
    mProxyGroupPtr = policyStore.mGenerators[type](mName, mProxies, mPeriod, mUrl);
    return mProxyGroupPtr;
}
