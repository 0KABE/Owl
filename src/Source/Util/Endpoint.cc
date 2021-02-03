#include "Util/Endpoint.hpp"

Owl::Endpoint::Endpoint(Owl::Endpoint::Socket socket)
        : mOptionalSocket(std::move(socket)),
          mHostname(mOptionalSocket->remote_endpoint().address().to_string()),
          mPort(std::to_string(mOptionalSocket->remote_endpoint().port())),
          mHostnameType(ParseHostnameType(mHostname)) {}

Owl::Endpoint::Endpoint(std::string hostname, std::string port)
        : mHostname(std::move(hostname)),
          mPort(std::move(port)),
          mHostnameType(ParseHostnameType(mHostname)) {}

Owl::Awaitable<void> Owl::Endpoint::Connect(Owl::Endpoint::Timeout timeout) {
    const net::executor &executor = co_await net::this_coro::executor;
    switch (mState) {
        case READY_TO_CONNECT:
            try {
                mState = CONNECTING;
                mConnectCompleted = Event(executor);
                FinalAction finalAction([&] { mConnectCompleted->NotifyAll(); });

                ResolveResult resolveResult = co_await Resolve();
                co_await TryConnect(resolveResult, timeout);

                mState = CONNECTED;
            } catch (const std::exception &e) {
                mState = FAIL_TO_CONNECT;
                spdlog::warn("Fail in connecting to {}:{}, Exception={}", mHostname, mPort, e.what());
            }
            break;

        case CONNECTING:
            spdlog::trace("Waiting for connecting to {}:{}", mHostname, mPort);
            co_await mConnectCompleted->AsyncWait();
            spdlog::trace("Finish to waiting for connecting to {}:{}", mHostname, mPort);
            break;

        case CONNECTED:
            spdlog::trace("{}:{} has been connected", mHostname, mPort);
            break;

        case FAIL_TO_CONNECT:
            spdlog::trace("Connecting to {}:{} failed previously", mHostname, mPort);
            break;
    }
}

const Owl::Endpoint::ConnectionState &Owl::Endpoint::GetState() const {
    return mState;
}

Owl::Awaitable<Owl::Endpoint::ResolveResult> Owl::Endpoint::Resolve() {
    spdlog::info("Resolving {}:{}", mHostname, mPort);
    const net::executor &executor = co_await net::this_coro::executor;

    Resolver resolver(executor);
    ResolveResult resolveResult = co_await resolver.async_resolve({mHostname, mPort}, use_awaitable);

    spdlog::trace("Succeed in resolving {}:{}, ip={}", mHostname, mPort,
                  resolveResult.begin()->endpoint().address().to_string());
    co_return resolveResult;
}

Owl::Awaitable<void>
Owl::Endpoint::TryConnect(Owl::Endpoint::ResolveResult &resolveResult, Owl::Endpoint::Timeout timeout) {
    spdlog::info("Connecting to {}:{}", mHostname, mPort);

    const net::executor &executor = co_await net::this_coro::executor;
    mOptionalSocket = Socket(executor);

    TimeoutEvent timeoutEvent([&](const auto &ec) {
        if (ec != net::error::operation_aborted && mOptionalSocket.has_value()) {
            spdlog::warn("Connect to {}:{} timeout, timeout={}ms", mHostname, mPort, timeout.count());
            mOptionalSocket->close();
        }
    });
    timeoutEvent.Run(executor, timeout);

    co_await net::async_connect(*mOptionalSocket, resolveResult, use_awaitable);
    spdlog::trace("Succeed in connecting to {}:{}", mHostname, mPort);
}

Owl::Endpoint::HostnameType Owl::Endpoint::ParseHostnameType(const std::string &hostname) {
    boost::system::error_code errorCode;
    boost::asio::ip::address::from_string(hostname, errorCode);
    return errorCode ? HOST_DOMAIN : HOST_IP;
}

const std::string &Owl::Endpoint::GetHostname() const {
    return mHostname;
}

const std::string &Owl::Endpoint::GetPort() const {
    return mPort;
}

const Owl::Endpoint::HostnameType &Owl::Endpoint::GetHostnameType() const {
    return mHostnameType;
}

Owl::Endpoint::Socket &Owl::Endpoint::GetSocket() {
    return *mOptionalSocket;
}

std::string Owl::Endpoint::ToString() const {
    return fmt::format("{}:{}", mHostname, mPort);
}



