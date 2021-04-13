#pragma once

#include <unordered_set>
#include "EnableSharedInstance.hpp"
#include "Net.hpp"
#include "Awaitable.hpp"

namespace Owl {
    class Event : public EnableSharedInstance<Event>,
                  public std::enable_shared_from_this<Event> {
    public:
        using EventPtr = std::shared_ptr<Event>;
        using ErrorCode = boost::system::error_code;
        using Action = std::function<void(ErrorCode)>;
        using ActionPtr = std::shared_ptr<Action>;
        static inline const ErrorCode &CANCEL_ERROR = net::error::operation_aborted;

        /**
         * @brief Enable to invoke callback action
         *
         * @return
         *
         * @note All callback in mActions will be invoked if this callback is hit by NotifyOnce()
         */
        EventPtr EnableCallback();

        Awaitable<void> AsyncWait();

        /**
         * @brief Add a callback
         *
         * @param action
         * @return
         */
        Event &operator+=(const ActionPtr &action);

        /**
         * @brief Remove a callback
         *
         * @param action
         * @return
         */
        Event &operator-=(const ActionPtr &action);

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

    protected:
        explicit Event(const net::executor &executor);

        net::steady_timer mTimer;
        std::unordered_set<ActionPtr> mActions;
        bool mCallbackEnabled = false;
    };

    using EventPtr = Event::EventPtr;
}


