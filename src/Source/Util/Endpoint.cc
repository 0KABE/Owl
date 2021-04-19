#include "Util/Endpoint.hpp"

#include <utility>

Owl::Endpoint::Endpoint(Socket socket)
        : mSocket(std::move(socket)),
          mConnectingEvent(Event::NewInstance(socket.get_executor())),
          mHostname(socket.remote_endpoint().address().to_string()),
          mPort(std::to_string(socket.remote_endpoint().port())),
          mHostnameType(ParseHostnameType(mHostname)) {}

Owl::Endpoint::Endpoint(Executor &executor, Hostname hostname, Port port)
        : mSocket(executor),
          mConnectingEvent(Event::NewInstance(executor)),
          mHostname(std::move(hostname)),
          mPort(std::move(port)),
          mHostnameType(ParseHostnameType(mHostname)) {}


Owl::Endpoint::HostnameType Owl::Endpoint::ParseHostnameType(const std::string &hostname) {
    boost::system::error_code errorCode;
    boost::asio::ip::address::from_string(hostname, errorCode);
    return errorCode ? HOST_DOMAIN : HOST_IP;
}

const char *Owl::Endpoint::StatusToString(ConnectionState state) {
    switch (state) {
        case READY:
            return "Ready";
        case CONNECTING :
            return "Connecting";
        case CONNECTED :
            return "Connected";
        case DISCONNECTED :
            return "Disconnected";
    }
}

Owl::Awaitable<void> Owl::Endpoint::Connect(Owl::Endpoint::Milliseconds timeout) {
    switch (mState) {
        case READY:
            try {
                mState = CONNECTING;
                FinalAction finalAction([&] { mConnectingEvent->NotifyAll(); });
                co_await StartConnecting(timeout);
                mState = CONNECTED;
            } catch (const std::exception &e) {
                mState = DISCONNECTED;
                spdlog::warn("Fail in connecting to {}:{}, Exception={}", mHostname, mPort, e.what());
            }
            break;

        case CONNECTING:
            spdlog::trace("Waiting for connecting to {}:{}", mHostname, mPort);
            co_await mConnectingEvent->AsyncWait();
            spdlog::trace("Finish to waiting for connecting to {}:{}", mHostname, mPort);
            break;

        default:
            spdlog::trace("Connection {}:{} has been {}", mHostname, mPort, StatusToString(mState));
            break;
    }
}

void Owl::Endpoint::Disconnect() {
    mDelegate();
    mState = DISCONNECTED;
}

Owl::Awaitable<void> Owl::Endpoint::StartConnecting(Milliseconds timeout) {
    ResolveResult resolveResult = co_await ResolveHostname();
    co_await TryConnect(resolveResult, timeout);
}

Owl::Awaitable<Owl::Endpoint::ResolveResult> Owl::Endpoint::ResolveHostname() {
    spdlog::info("Resolving {}:{}", mHostname, mPort);
    const net::executor &executor = co_await net::this_coro::executor;

    Resolver resolver(executor);

    //Delegate will be invoked by Disconnect()
    Delegate<>::SharedFunction delegateFunction = std::make_shared<Delegate<>::Function>([&] { resolver.cancel(); });
    mDelegate += delegateFunction;
    FinalAction finalAction([&] { mDelegate -= delegateFunction; });

    ResolveResult resolveResult = co_await resolver.async_resolve({mHostname, mPort}, use_awaitable);
    spdlog::trace("Succeed in resolving {}:{}, ip={}", mHostname, mPort,
                  resolveResult.begin()->endpoint().address().to_string());

    co_return resolveResult;
}

Owl::Awaitable<void>
Owl::Endpoint::TryConnect(Owl::Endpoint::ResolveResult &resolveResult, Owl::Endpoint::Milliseconds timeout) {
    spdlog::info("Connecting to {}:{}", mHostname, mPort);

    const net::executor &executor = co_await net::this_coro::executor;

    TimeoutEvent timeoutEvent([&](const auto &ec) {
        spdlog::warn("Connect to {}:{} timeout, timeout={}ms", mHostname, mPort, timeout.count());
        mSocket.close();
    });
    timeoutEvent.Run(executor, timeout);

    Delegate<>::SharedFunction delegateFunction = std::make_shared<Delegate<>::Function>([&] { mSocket.close(); });
    mDelegate += delegateFunction;
    FinalAction finalAction([&] { mDelegate -= delegateFunction; });

    co_await net::async_connect(mSocket, resolveResult, use_awaitable);
    spdlog::trace("Succeed in connecting to {}:{}", mHostname, mPort);
}

Owl::Endpoint::Socket &Owl::Endpoint::GetSocket() {
    return mSocket;
}

const std::string &Owl::Endpoint::GetHostname() const {
    return mHostname;
}

const std::string &Owl::Endpoint::GetPort() const {
    return mPort;
}

Owl::Endpoint::HostnameType Owl::Endpoint::GetHostnameType() const {
    return mHostnameType;
}

Owl::Endpoint::ConnectionState Owl::Endpoint::GetState() const {
    return mState;
}

std::string Owl::Endpoint::ToString() const {
    return fmt::format("{}:{}", mHostname, mPort);
}
