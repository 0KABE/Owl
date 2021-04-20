#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <spdlog/spdlog.h>
#include "Util/Event.hpp"
#include "Util/EnableSharedInstance.hpp"

Owl::Event::Event(const boost::asio::executor &executor) : mTimer(executor) {
    mTimer.expires_at(std::chrono::time_point<std::chrono::steady_clock>::max());
}

Owl::Awaitable<void> Owl::Event::AsyncWait() {
    try {
        co_await mTimer.async_wait(use_awaitable);
        spdlog::debug("AsyncWait completed");
    } catch (const std::exception &e) {
        spdlog::debug("Event::AsyncWait failed, message: {}", e.what());
    }
}

void Owl::Event::NotifyAll() { mTimer.cancel(); }

void Owl::Event::NotifyOnce() { mTimer.cancel_one(); }

void Owl::Event::ContinuouslyNotify() {
    mTimer.expires_at(std::chrono::steady_clock::now());
}
