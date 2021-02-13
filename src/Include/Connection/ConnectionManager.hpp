#pragma once

#include <boost/asio.hpp>
#include <set>
#include <spdlog/spdlog.h>
#include "Connection.hpp"
#include "Util/Singleton.hpp"
#include "Util/Concepts.hpp"

namespace Owl {
    class ConnectionManager : public Singleton<ConnectionManager> {
        friend Singleton<ConnectionManager>;
    public:
        using ConnectionSharedPtr = std::shared_ptr<Connection>;
        using ConnectionWeakPtr = std::weak_ptr<Connection>;
        using ConnectionPtrs = std::set<ConnectionWeakPtr, std::owner_less<ConnectionWeakPtr>>;
        using Socket = net::ip::tcp::socket;

        template<typename T>
        requires Derived<Connection, T>
        ConnectionSharedPtr Create(Socket socket) {
            spdlog::trace("Creating a connection [{}:{}]<-->[{}:{}]",
                          socket.local_endpoint().address().to_string(),
                          socket.local_endpoint().port(),
                          socket.remote_endpoint().address().to_string(),
                          socket.remote_endpoint().port());
            ConnectionSharedPtr connection = std::make_shared<T>(std::move(socket));
            AddConnection(connection);
            return connection;
        }

        [[nodiscard]] const ConnectionPtrs &GetConnections() const { return mConnections; }

        void RemoveConnection(const ConnectionWeakPtr &connection) {
            mConnections.erase(connection);
            spdlog::trace("Connections in ConnectionManager: {}", mConnections.size());
        }

    private:
        void AddConnection(const ConnectionWeakPtr &connection) {
            mConnections.insert(connection);
            spdlog::trace("Connections in ConnectionManager: {}", mConnections.size());
        }

        ConnectionManager() = default;

        ConnectionPtrs mConnections;
    };
}


