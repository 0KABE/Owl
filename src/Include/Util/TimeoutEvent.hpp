#pragma once

#include <boost/system/error_code.hpp>
#include <utility>
#include "Awaitable.hpp"

namespace Owl {
    class TimeoutEvent {
    public:
        using Timeout = std::chrono::milliseconds;
        using Action = std::function<void(boost::system::error_code)>;
        using Timer = std::optional<net::steady_timer>;

        TimeoutEvent() = default;

        explicit TimeoutEvent(Action action);

        explicit TimeoutEvent(std::vector<Action> actions);

        ~TimeoutEvent() = default;

        TimeoutEvent &operator+=(Action action);

        void Run(const net::executor &executor, Timeout timeout);

    private:
        Timer mTimer;
        std::vector<Action> mActions;
    };
}


