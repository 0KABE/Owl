#pragma once

#include <string>
#include "ProxyNode.hpp"

namespace Owl {
    class DelayTester {
    public:
        using Delay = std::optional<std::chrono::milliseconds>;

        DelayTester(std::string url, ProxyNode::ProxyPtr proxyPtr);

        Awaitable<Delay> TestDelay(TimeoutEvent::Timeout timeout = TimeoutEvent::Timeout(5000));

    private:
        std::string mUrl;
        ProxyNode::ProxyPtr mProxyPtr;
    };
}
