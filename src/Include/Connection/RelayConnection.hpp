#pragma once

#include "Connection.hpp"
#include "Bound.hpp"

namespace Owl {
    class RelayConnection : public Connection {
    public:
        explicit RelayConnection(Socket socket);

        ~RelayConnection() override;

        void Open() override;

    private:
        Awaitable<void> Initialize();

        Awaitable<void> Receive();

        Awaitable<void> Send();

        Bound::BoundPtr mInbound;
        Bound::BoundPtr mOutbound;
    };
}


