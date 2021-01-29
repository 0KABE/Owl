#include "Proxy/BuiltInProxyNode.hpp"
#include "Proxy/ProxyNodeManager.hpp"
#include "Bound/Outbound/Direct.hpp"
#include "Bound/Outbound/Reject.hpp"

static auto DIRECT
        = Owl::ProxyNodeManager::BuiltInRegister<Owl::Direct>("DIRECT");

static auto REJECT
        = Owl::ProxyNodeManager::BuiltInRegister<Owl::Reject>("REJECT");
