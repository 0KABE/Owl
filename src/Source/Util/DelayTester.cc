#include "Util/DelayTester.hpp"
#include "Util/HttpClient.hpp"

#include <utility>

Owl::DelayTester::DelayTester(std::string url, Owl::ProxyNode::ProxyPtr proxyPtr)
        : mUrl(std::move(url)), mProxyPtr(std::move(proxyPtr)) {}

Owl::Awaitable<Owl::DelayTester::Delay> Owl::DelayTester::TestDelay(TimeoutEvent::Timeout timeout) {
    const net::executor &executor = co_await net::this_coro::executor;
    HttpClient<> httpClient(executor, Url::Parse(mUrl), mProxyPtr);
    TimeoutEvent timeoutEvent([&](const TimeoutEvent::ErrorCode &errorCode) {
        httpClient.Stop();
    });
    timeoutEvent.Run(executor, timeout);

    try {
        std::chrono::steady_clock::time_point startPoint = std::chrono::steady_clock::now();
        HttpClient<>::Response response = co_await httpClient.Run();
        std::chrono::steady_clock::time_point endPoint = std::chrono::steady_clock::now();
        co_return std::chrono::duration_cast<std::chrono::milliseconds>(endPoint - startPoint).count();
    } catch (std::exception &exception) {
        spdlog::warn("Test Delay to {} failed, exception: {}", mUrl, exception.what());
        co_return Delay();
    }
}
