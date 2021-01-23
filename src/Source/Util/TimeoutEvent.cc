#include "Util/TimeoutEvent.hpp"

Owl::TimeoutEvent::TimeoutEvent(Owl::TimeoutEvent::Action action) : mActions({std::move(action)}) {}

Owl::TimeoutEvent::TimeoutEvent(std::vector<Action> actions) : mActions(std::move(actions)) {}

Owl::TimeoutEvent &Owl::TimeoutEvent::operator+=(Owl::TimeoutEvent::Action action) {
    mActions.push_back(std::move(action));
    return *this;
}

void Owl::TimeoutEvent::Run(const boost::asio::executor &executor, Owl::TimeoutEvent::Timeout timeout) {
    mTimer = Timer(executor);
    mTimer->expires_after(timeout);

    mTimer->async_wait([&](const boost::system::error_code &errorCode) {
        if (errorCode != net::error::operation_aborted)
            for (const Action &action : mActions) action(errorCode);
    });
}
