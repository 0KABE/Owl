#pragma once

#include <boost/asio.hpp>
#include "Connection/ConnectionManager.hpp"
#include "Util/Awaitable.hpp"
#include "Util/Concepts.hpp"
#include <spdlog/spdlog.h>

namespace Owl {
    template<typename ConnectionType> requires Derived<Connection, ConnectionType>
    class Server {
    public:
        using ServerPtr = std::shared_ptr<Server>;

        Server(net::any_io_executor executor, uint16_t port)
                : mAcceptor(executor, {net::ip::tcp::v4(), port}) {}

        ~Server() = default;

        static ServerPtr Create(net::any_io_executor executor, uint16_t port) {
            using namespace net;

            ServerPtr server = std::make_unique<Server<ConnectionType>>(executor, port);

            co_spawn(executor, [=] { return server->Run(); }, detached);

            return server;
        }

        void Close() { mOpened = false; }

        Awaitable<void> Run() {
            using Socket = net::ip::tcp::socket;
            spdlog::info("Starting server at {}:{}", mAcceptor.local_endpoint().address().to_string(),
                         mAcceptor.local_endpoint().port());

            while (mOpened) {
                Socket socket = co_await mAcceptor.async_accept(use_awaitable);
                ConnectionManager::GetInstance()
                        .Create<ConnectionType>(std::move(socket))
                        ->Open();
            }
        }

    private:
        net::ip::tcp::acceptor mAcceptor;
        bool mOpened = true;
    };
}


