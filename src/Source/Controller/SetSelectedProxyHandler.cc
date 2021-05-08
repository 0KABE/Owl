#include <rapidjson/document.h>
#include <spdlog/spdlog.h>
#include "Proxy/ProxyNodeManager.hpp"
#include "Controller.hpp"
#include "Controller/SetSelectedProxyHandler.hpp"

Owl::Awaitable<void> Owl::SetSelectedProxyHandler::Handle(const Owl::Request &request, Owl::Socket socket,
                                                          const Owl::Connection::Status &status) const {
    using namespace boost::beast;
    http::response<http::string_body> response(http::status::ok, 11);

    //Cross-Origin Resource Sharing
    response.set(http::field::access_control_allow_origin, "*");
    response.set(http::field::access_control_allow_headers, "*");
    if (request.method() == Method::options) {
        co_await http::async_write(socket, response, use_awaitable);
        co_return;
    }

    try {
        rapidjson::Document document;
        document.Parse(request.body().c_str(), request.body().length());
        std::string proxy = document["proxy"].GetString();
        std::string select = document["select"].GetString();

        ProxyNodeManager &proxyNodeManager = ProxyNodeManager::GetInstance();
        ProxyPtr proxyPtr = proxyNodeManager.GetProxyOrPolicy(select);
        proxyNodeManager.GetPolicies().at(proxy)->SetSelectedProxy(proxyPtr);
    } catch (std::exception &exception) {
        spdlog::error("Handling {} error: {}", TARGET, exception.what());
        response = http::response<http::string_body>(http::status::internal_server_error, 11);
        response.body() = exception.what();
    }
    co_await http::async_write(socket, response, use_awaitable);
}

bool Owl::SetSelectedProxyHandler::Match(const Owl::Request &request) const {
    spdlog::debug("target: '{}', TARGET: '{}' method: {}", request.target().to_string(), TARGET,
                  request.method_string().to_string());
    return request.target() == TARGET;// && request.method() == METHOD;
}

[[maybe_unused]] static auto reg
        = Owl::Controller::Register(std::make_unique<Owl::SetSelectedProxyHandler>());
