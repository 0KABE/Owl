#pragma once

#include <Bound/Inbound.hpp>
#include <Bound/Outbound.hpp>
#include "Connection.hpp"

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

        Inbound::BoundPtr mInbound;
        Outbound::BoundPtr mOutbound;
    };
}


