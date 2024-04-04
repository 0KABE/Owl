#pragma once

#include <boost/system/error_code.hpp>
#include <utility>
#include "Awaitable.hpp"

namespace Owl {
    class TimeoutEvent {
    public:
        using Timeout = std::chrono::milliseconds;
        using ErrorCode = boost::system::error_code;
        using Action = std::function<void(ErrorCode)>;
        using Timer = std::optional<net::steady_timer>;
        static inline const ErrorCode &CANCEL_ERROR = net::error::operation_aborted;

        TimeoutEvent() = default;

        explicit TimeoutEvent(Action action);

        explicit TimeoutEvent(std::vector<Action> actions);

        ~TimeoutEvent() = default;

        TimeoutEvent &operator+=(Action action);

        void Run(net::any_io_executor executor, Timeout timeout);

        void Cancel();

    private:
        Timer mTimer;
        std::vector<Action> mActions;
    };
}


