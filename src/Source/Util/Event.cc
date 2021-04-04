#include "Util/Event.hpp"

Owl::Event::Event(const boost::asio::executor &executor) : timer(executor) {
    timer.expires_at(std::chrono::time_point<std::chrono::steady_clock>::max());
}

Owl::Awaitable<void> Owl::Event::AsyncWait() {
    try {
        co_await timer.async_wait(use_awaitable);
    } catch (const std::exception &e) {}
}

void Owl::Event::NotifyAll() { timer.cancel(); }

void Owl::Event::NotifyOnce() { timer.cancel_one(); }

void Owl::Event::ContinuouslyNotify() {
    timer.expires_at(std::chrono::steady_clock::now());
}




