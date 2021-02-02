#include "Rule.hpp"

Owl::Rule::Rule(Owl::Proxy::ProxyPtr proxyPtr) : mProxyPtr(std::move(proxyPtr)) {}

const Owl::Proxy::ProxyPtr &Owl::Rule::GetProxyPtr() const { return mProxyPtr; }
