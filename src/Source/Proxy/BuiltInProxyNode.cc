#include "Proxy/BuiltInProxyNode.hpp"
#include "Proxy/ProxyNodeManager.hpp"
#include "Bound/Outbound/Direct.hpp"

static auto DIRECT
        = Owl::ProxyNodeManager::BuiltInRegister<Owl::Direct>("DIRECT");
