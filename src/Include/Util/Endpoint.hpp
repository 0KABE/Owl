#pragma once

#include <utility>
#include <spdlog/spdlog.h>
#include "Net.hpp"
#include "Awaitable.hpp"
#include "Event.hpp"
#include "FinalAction.hpp"
#include "TimeoutEvent.hpp"

namespace Owl {
    class Endpoint {
    public:
        enum ConnectionState {
            READY_TO_CONNECT, CONNECTING, CONNECTED, FAIL_TO_CONNECT
        };
        enum HostnameType {
            HOST_DOMAIN, HOST_IP
        };

        using ResolveResult = boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp>;
        using Socket = net::ip::tcp::socket;
        using OptionalSocket = std::optional<Socket>;
        using OptionalEvent = std::optional<Event>;
        using Timeout = std::chrono::milliseconds;
        using Resolver = net::ip::tcp::resolver;

        explicit Endpoint(Socket socket);

        Endpoint(std::string hostname, std::string port);

        Awaitable<void> Connect(Timeout timeout = Timeout(500));

        [[nodiscard]] const ConnectionState &GetState() const;

        [[nodiscard]] const std::string &GetHostname() const;

        [[nodiscard]] const std::string &GetPort() const;

        [[nodiscard]] Socket &GetSocket();

        [[nodiscard]] const HostnameType &GetHostnameType() const;

    private:
        static HostnameType ParseHostnameType(const std::string &hostname);

        Awaitable<ResolveResult> Resolve();

        Awaitable<void> TryConnect(ResolveResult &resolveResult, Timeout timeout);

        OptionalSocket mOptionalSocket;
        OptionalEvent mConnectCompleted;
        std::string mHostname;
        std::string mPort;
        ConnectionState mState = READY_TO_CONNECT;
        HostnameType mHostnameType;
    };
}


