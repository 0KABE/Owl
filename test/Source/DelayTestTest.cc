#include <gtest/gtest.h>
#include "Util/DelayTest.hpp"
#include "Proxy/ProxyNodeManager.hpp"

using namespace Owl;

Awaitable<void> TestLatency(DelayTest::Delay &delay, std::string url, ProxyNode::ProxyPtr proxyPtr) {
    DelayTest delayTest(url, proxyPtr);
    delay = co_await delayTest.TestDelay();
}

TEST(DelayTest, Success) {
    net::io_context ioContext;
    std::string url = "http://info.cern.ch";

    DelayTest::Delay delay;
    net::co_spawn(ioContext.get_executor(),
                  [&] {
                      return TestLatency(delay, url, ProxyNodeManager::GetInstance().GetProxy("DIRECT"));
                  },
                  net::detached);
    ioContext.run();

    ASSERT_EQ(delay.has_value(), true);
    spdlog::info("Delay of connecting to {}: {}ms", url, delay.value().count());
}

TEST(DelayTest, Failure) {
    net::io_context ioContext;
    std::string url = "http://www.google.com";

    DelayTest::Delay delay;
    net::co_spawn(ioContext.get_executor(),
                  [&] {
                      return TestLatency(delay, url, ProxyNodeManager::GetInstance().GetProxy("DIRECT"));
                  },
                  net::detached);
    ioContext.run();

    ASSERT_EQ(delay.has_value(), false);
    spdlog::info("Delay of connecting to {}: {}ms", url, delay.value().count());
}
