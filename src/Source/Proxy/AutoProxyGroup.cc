#include "Proxy/AutoProxyGroup.hpp"
#include "Proxy/PolicyBuilder.hpp"

#include <utility>
#include <Util/DelayTester.hpp>

Owl::AutoProxyGroup::AutoProxyGroup(const std::string &name, const std::vector<ProxyPtr> &proxies,
                                    const std::chrono::seconds &period, std::string url)
        : Policy(name, proxies),
          mPeriod(period),
          mUrl(std::move(url)) {}

std::string Owl::AutoProxyGroup::GetPolicyType() const {
    return TYPE;
}

Owl::Outbound::BoundPtr Owl::AutoProxyGroup::GetOutbound(Owl::Endpoint endpoint) const {
    return mSelectedProxy->GetOutbound(std::move(endpoint));
}

void Owl::AutoProxyGroup::Start(const net::executor &executor) {
    net::co_spawn(executor,
                  [=, self = shared_from_this()] { return ConnectivityTest(); },
                  net::detached);
}

Owl::Awaitable<void> Owl::AutoProxyGroup::ConnectivityTest() {
    const net::executor &executor = co_await net::this_coro::executor;
    net::steady_timer timer(executor);

    do {
        spdlog::info("Start in testing Policy: {}", mName);
        ProxyPtr selectedProxy;
        TimeoutEvent::Timeout minLatency = TimeoutEvent::Timeout::max();
        for (const ProxyPtr &proxyPtr : mProxies) {
            DelayTester delayTest(mUrl, proxyPtr);
            Delay delay = co_await delayTest.TestDelay(TimeoutEvent::Timeout(1000));
            spdlog::info("Test latency {}: {}ms", proxyPtr->GetName(), delay->count());
            if (delay && delay < minLatency) {
                minLatency = delay.value();
                selectedProxy = proxyPtr;
            }
        }

        spdlog::info("Proxy Group {} selected proxy group changed {} -> {}({}ms)",
                     GetName(),
                     GetSelectedProxy() ? GetSelectedProxy()->GetName() : "None",
                     selectedProxy->GetName(), minLatency.count());
        SetSelectedProxy(selectedProxy);

        timer.expires_after(mPeriod);
        co_await timer.async_wait(use_awaitable);
    } while (mStatus == RUNNING);
}

static Owl::PolicyBuilder::Register<Owl::AutoProxyGroup> r(Owl::AutoProxyGroup::TYPE);