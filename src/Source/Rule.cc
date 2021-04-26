#include "Rule.hpp"

Owl::Rule::Rule(Owl::ProxyPtr proxyPtr) : mProxyPtr(std::move(proxyPtr)) {}

const Owl::ProxyPtr &Owl::Rule::GetProxyPtr() const { return mProxyPtr; }
