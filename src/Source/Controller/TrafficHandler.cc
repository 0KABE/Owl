#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "Controller.hpp"
#include "Controller/TrafficHandler.hpp"
#include "Connection/ConnectionManager.hpp"
#include "Connection.hpp"

Owl::Awaitable<void>
Owl::TrafficHandler::Handle(const Owl::Request &request, Owl::Socket socket, const Connection::Status &status) const {
    using namespace boost::beast;

    websocket::stream<tcp_stream> ws(std::move(socket));
    co_await ws.async_accept(request, use_awaitable);

    net::steady_timer timer(ws.get_executor());
    while (status != Connection::CLOSE) {
        timer.expires_after(std::chrono::seconds(1));
        co_await timer.async_wait(use_awaitable);

        std::string trafficData = GetTraffic();
        net::const_buffer buffer(trafficData.data(), trafficData.size());
        co_await ws.async_write(buffer, use_awaitable);
    }
}

bool Owl::TrafficHandler::Match(const Owl::Request &request) const {
    return request.target() == TARGET;
}

std::string Owl::TrafficHandler::GetTraffic() {
    using namespace rapidjson;

    ConnectionManager &connectionManager = ConnectionManager::GetInstance();

    double upload = 0, download = 0;
    auto &connectionCollection = connectionManager.GetConnections();
    for (auto &connection : connectionCollection) {
        download += connection.lock()->GetDownloadTraffic();
        upload += connection.lock()->GetUploadTraffic();
    }

    Document document;
    document.SetObject();
    document.AddMember("upload", upload, document.GetAllocator());
    document.AddMember("download", download, document.GetAllocator());

    StringBuffer buffer;
    Writer <StringBuffer> writer(buffer);
    document.Accept(writer);
    return buffer.GetString();
}

static const Owl::Controller::Register
        handler(std::make_unique<Owl::TrafficHandler>());