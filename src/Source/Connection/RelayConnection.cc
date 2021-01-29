#include "Connection/RelayConnection.hpp"
#include "ConnectionManager.hpp"
#include "Proxy/ProxyNodeManager.hpp"
#include "Bound/Inbound/Socks5Server.hpp"
#include "BoundFactory.hpp"

void Owl::RelayConnection::Open() {
    mConnectionWeakPtr = shared_from_this();
    net::co_spawn(mInbound->GetEndpoint().GetSocket().get_executor(),
                  [=, self(shared_from_this())] { return Initialize(); },
                  net::detached);
}

Owl::RelayConnection::RelayConnection(Owl::Connection::Socket socket)
        : mInbound(BoundFactory::Create<Socks5Server>(std::move(socket))) {}

Owl::RelayConnection::~RelayConnection() {
    ConnectionManager::GetInstance().RemoveConnection(mConnectionWeakPtr);
}

Owl::Awaitable<void> Owl::RelayConnection::Initialize() {
    Bound::TargetEndpoint targetEndpoint = co_await mInbound->Initialize();
    mOutbound = ProxyNodeManager::GetInstance().GetProxy("DIRECT")->GetOutbound(std::move(*targetEndpoint));

    net::co_spawn(mInbound->GetEndpoint().GetSocket().get_executor(),
                  [=, self(shared_from_this())] { return RefreshTraffic(std::chrono::seconds(1)); },
                  net::detached);
    net::co_spawn(mInbound->GetEndpoint().GetSocket().get_executor(),
                  [=, self(shared_from_this())] { return Receive(); },
                  net::detached);
    net::co_spawn(mInbound->GetEndpoint().GetSocket().get_executor(),
                  [=, self(shared_from_this())] { return Send(); },
                  net::detached);
    mStatus = OPEN;
}

Owl::Awaitable<void> Owl::RelayConnection::Receive() {
    FinalAction finalAction([&] {
        spdlog::trace("Receive data completed, turning status to CLOSE");
        mStatus = CLOSE;
    });
    while (mStatus == OPEN) {
        co_await mInbound->Receive();
        co_await mOutbound->Send(mInbound->GetReceiveBuffer());
    }
}

Owl::Awaitable<void> Owl::RelayConnection::Send() {
    FinalAction finalAction([&] {
        spdlog::trace("Send data completed, turning status to CLOSE");
        mStatus = CLOSE;
    });
    while (mStatus == OPEN) {
        co_await mOutbound->Receive();
        co_await mInbound->Send(mOutbound->GetReceiveBuffer());
    }
}


