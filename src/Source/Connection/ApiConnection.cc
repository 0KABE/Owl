#include "Connection/ApiConnection.hpp"
#include "Connection/ConnectionManager.hpp"
#include "Controller.hpp"
#include "Util/FinalAction.hpp"

Owl::ApiConnection::ApiConnection(Owl::Connection::Socket socket)
        : mSocket(std::move(socket)) {}

Owl::ApiConnection::~ApiConnection() {
    ConnectionManager::GetInstance().RemoveConnection(mConnectionWeakPtr);
}

void Owl::ApiConnection::Open() {
    mStatus = OPEN;
    mConnectionWeakPtr = shared_from_this();
    net::co_spawn(mSocket.get_executor(),
                  [=, self = shared_from_this()] { return Handle(); },
                  net::detached);
}

Owl::Awaitable<void> Owl::ApiConnection::Handle() {
    using namespace boost::beast;

    FinalAction final([=] { Close(); });

    Request request;
    flat_buffer buffer;
    co_await http::async_read(mSocket, buffer, request, use_awaitable);
    spdlog::debug("{}", static_cast<std::string_view>(request.target()));
    co_await Controller::GetInstance().Handle(request, std::move(mSocket), mStatus);
}
