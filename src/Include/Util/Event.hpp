#pragma once

#include "Net.hpp"
#include "Awaitable.hpp"

namespace Owl {
    class Event {
    public:
        explicit Event(const net::executor &executor);

        Awaitable<void> AsyncWait();

        void NotifyAll();

        void NotifyOnce();

        void ContinuouslyNotify();

    private:
        net::steady_timer timer;
    };
}


