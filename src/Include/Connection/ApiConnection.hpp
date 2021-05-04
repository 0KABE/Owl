#pragma once

#include "Connection.hpp"

namespace Owl {
    class ApiConnection : public Connection {
    public:
        explicit ApiConnection(Socket socket);

        ~ApiConnection() override;

        void Open() override;

    private:
        Awaitable<void> Handle();

        Socket mSocket;
    };
}


