#include "Rule.hpp"

Owl::Rule::Rule(Owl::ProxyNode::ProxyPtr proxyPtr) : mProxyPtr(std::move(proxyPtr)) {}

const Owl::ProxyNode::ProxyPtr &Owl::Rule::GetProxyPtr() const { return mProxyPtr; }
