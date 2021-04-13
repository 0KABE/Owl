#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <spdlog/spdlog.h>
#include "Util/Event.hpp"
#include "Util/EnableSharedInstance.hpp"

Owl::Event::Event(const boost::asio::executor &executor) : mTimer(executor) {
    mTimer.expires_at(std::chrono::time_point<std::chrono::steady_clock>::max());
}

Owl::EventPtr Owl::Event::EnableCallback() {
    if (!mCallbackEnabled) {
        mCallbackEnabled = true;
        mTimer.async_wait([=, self = shared_from_this()](ErrorCode errorCode) {
            SPDLOG_TRACE("Callback[{}] is invoked with error: {}, code: {}", fmt::ptr(this), errorCode.message(),
                         errorCode.value());
            for (const ActionPtr &action : mActions)
                (*action)(errorCode);
        });
    }
    return shared_from_this();
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

Owl::Event &Owl::Event::operator+=(const Owl::Event::ActionPtr &action) {
    mActions.insert(action);
    return *this;
}

Owl::Event &Owl::Event::operator-=(const Owl::Event::ActionPtr &action) {
    mActions.erase(action);
    return *this;
}
