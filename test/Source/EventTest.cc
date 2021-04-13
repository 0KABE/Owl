#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include "Util/Event.hpp"
#include "Util/TimeoutEvent.hpp"

using namespace Owl;

enum Status {
    INITIALIZED,
    WAITING,
    NOTIFIED,
    FINISHED
};

Awaitable<void> WaitBeforeNotifyHelper(Event &event, Status &status) {
    const net::executor &executor = co_await net::this_coro::executor;
    net::steady_timer timer(executor);
    timer.expires_after(std::chrono::seconds(1));
    co_await timer.async_wait(net::use_awaitable);
    event.NotifyAll();
    status = status == WAITING ? NOTIFIED : status;
}

Awaitable<void> WaitBeforeNotify(Status &status) {
    const net::executor &executor = co_await net::this_coro::executor;
    EventPtr eventPtr = Event::NewInstance(executor);
    net::co_spawn(executor, [&] { return WaitBeforeNotifyHelper(*eventPtr, status); }, net::detached);
    status = status == INITIALIZED ? WAITING : status;
    co_await eventPtr->AsyncWait();
    status = status == NOTIFIED ? FINISHED : status;
}

TEST(Event, Wait_Before_Notify) {
    net::io_context ioContext;
    Status status = INITIALIZED;
    net::co_spawn(ioContext.get_executor(), [&] { return WaitBeforeNotify(status); }, net::detached);
    ioContext.run();
    ASSERT_EQ(status, FINISHED);
}

Awaitable<void> WaitAfterNotifyHelper(Event &event, Status &status) {
    const net::executor &executor = co_await net::this_coro::executor;
    spdlog::debug("NotifyHelper executing, expect for 50ms");

    net::steady_timer timer(executor);
    timer.expires_after(std::chrono::milliseconds(50));
    co_await timer.async_wait(net::use_awaitable);

    event.ContinuouslyNotify();
    spdlog::debug("NotifyHelper continuously notifying");
    status = status == INITIALIZED ? NOTIFIED : status;
}

Awaitable<void> WaitAfterNotify(TimeoutEvent &timeoutEvent, Status &status) {
    const net::executor &executor = co_await net::this_coro::executor;
    EventPtr eventPtr = Event::NewInstance(executor);
    net::co_spawn(executor, [&] { return WaitAfterNotifyHelper(*eventPtr, status); }, net::detached);

    spdlog::debug("Wait for 100 ms");
    net::steady_timer timer(executor);
    timer.expires_after(std::chrono::milliseconds(100));
    co_await timer.async_wait(net::use_awaitable);

    status = status == NOTIFIED ? WAITING : status;
    spdlog::debug("Waiting for notifying");
    co_await eventPtr->AsyncWait();
    spdlog::debug("Completed the wait");
    status = status == WAITING ? FINISHED : status;
    timeoutEvent.Cancel();
}

TEST(Event, Wait_After_Notify) {
    net::io_context ioContext;
    bool timeout = false;

    spdlog::debug("Set timeout as 200ms");
    TimeoutEvent timeoutEvent([&](const auto &ec) {
        if (ec != net::error::operation_aborted) {
            ioContext.stop();
            timeout = true;
        }
    });
    timeoutEvent.Run(ioContext.get_executor(), TimeoutEvent::Timeout(200));

    Status status = INITIALIZED;
    net::co_spawn(ioContext.get_executor(), [&] { return WaitAfterNotify(timeoutEvent, status); }, net::detached);
    ioContext.run();
    if (timeout) GTEST_FATAL_FAILURE_("Event::WaitAfterNotify timeout");
    ASSERT_EQ(status, FINISHED);
}

Awaitable<void> NotifyOnceHelper(int &count, Event &event) {
    co_await event.AsyncWait();
    ++count;
}

TEST(Event, Notify_Once) {
    net::io_context ioContext;
    EventPtr eventPtr = Event::NewInstance(ioContext.get_executor())->EnableCallback();
    int count = 0;

    for (int i = 0; i < 10; ++i) {
        net::co_spawn(ioContext.get_executor(), [&] { return NotifyOnceHelper(count, *eventPtr); }, net::detached);
    }

    TimeoutEvent notifyEvent([&](const TimeoutEvent::ErrorCode &errorCode) {
        if (errorCode != TimeoutEvent::CANCEL_ERROR) {
            eventPtr->NotifyOnce();
        }
    });
    notifyEvent.Run(ioContext.get_executor(), TimeoutEvent::Timeout(10));

    TimeoutEvent stopIoContextEvent([&](const TimeoutEvent::ErrorCode &ec) {
        if (ec != net::error::operation_aborted) {
            ioContext.stop();
        }
    });
    stopIoContextEvent.Run(ioContext.get_executor(), TimeoutEvent::Timeout(20));

    ioContext.run();

    ASSERT_EQ(count, 1);
}