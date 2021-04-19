#pragma once

#include <utility>
#include <spdlog/spdlog.h>
#include "Net.hpp"
#include "Awaitable.hpp"
#include "Event.hpp"
#include "FinalAction.hpp"
#include "TimeoutEvent.hpp"
#include "Delegate.hpp"

namespace Owl {
    class Endpoint {
    public:
        enum ConnectionState {
            READY,
            CONNECTING,
            CONNECTED,
            DISCONNECTED
        };
        enum HostnameType {
            HOST_DOMAIN, HOST_IP
        };

        using Socket = net::ip::tcp::socket;
        using Hostname = std::string;
        using Port = std::string;
        using Executor = const net::executor;
        using Milliseconds = std::chrono::milliseconds;
        using Resolver = net::ip::tcp::resolver;
        using ResolveResult = boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp>;


        explicit Endpoint(Socket socket);

        Endpoint(Executor &executor, Hostname hostname, Port port);


        Awaitable<void> Connect(Milliseconds timeout = Milliseconds(500));

        void Disconnect();

        [[nodiscard]] Socket &GetSocket();

        [[nodiscard]] const std::string &GetHostname() const;

        [[nodiscard]] const std::string &GetPort() const;

        [[nodiscard]] HostnameType GetHostnameType() const;

        [[nodiscard]] ConnectionState GetState() const;

        [[nodiscard]] std::string ToString() const;


    private:
        static HostnameType ParseHostnameType(const std::string &hostname);

        static const char *StatusToString(ConnectionState state);

        Owl::Awaitable<void> StartConnecting(Milliseconds timeout);

        Awaitable<ResolveResult> ResolveHostname();

        Awaitable<void> TryConnect(ResolveResult &resolveResult, Milliseconds timeout);

        Socket mSocket;
        EventPtr mConnectingEvent;
        std::string mHostname;
        std::string mPort;
        HostnameType mHostnameType;
        ConnectionState mState = READY;
        Delegate<> mDelegate;
    };
}
