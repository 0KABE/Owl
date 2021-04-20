#pragma once

#include <string>
#include "ProxyNode.hpp"

namespace Owl {
    class DelayTest {
    public:
        using Delay = std::optional<std::chrono::milliseconds>;

        DelayTest(std::string url, ProxyNode::ProxyPtr proxyPtr);

        Awaitable<Delay> TestDelay(TimeoutEvent::Timeout timeout = TimeoutEvent::Timeout(5000));

    private:
        std::string mUrl;
        ProxyNode::ProxyPtr mProxyPtr;
    };
}
