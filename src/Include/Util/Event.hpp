#pragma once

#include "Net.hpp"
#include "Awaitable.hpp"

namespace Owl {
    class Event {
    public:
        explicit Event(const net::executor &executor);

        Awaitable<void> AsyncWait();

        /**
         * @brief Notify for all objects awaiting on this event
         *
         * @note Objects will not receive notification if start to wait after call NotifyAll()
         */
        void NotifyAll();

        /**
         * @brief Notify for one object awaiting on this event
         *
         * @note Objects will not receive notification if start to wait after call NotifyAll()
         */
        void NotifyOnce();

        /**
         * @brief Continuously notify for any objects awaiting on this event
         *
         * @note Objects of which await on this event will receive notification even if ContinuouslyNotify() has been invoked before
         */
        void ContinuouslyNotify();

    private:
        net::steady_timer timer;
    };
}


