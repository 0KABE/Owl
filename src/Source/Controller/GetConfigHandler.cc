#include <spdlog/spdlog.h>
#include "Controller/GetConfigHandler.hpp"
#include "Controller.hpp"
#include "Configuration.hpp"
#include "CliOptions.hpp"

Owl::Awaitable<void> Owl::GetConfigHandler::Handle(const Owl::Request &request, Owl::Socket socket,
                                                   const Owl::Connection::Status &status) const {
    using namespace boost::beast;
    using Response = http::response<http::string_body>;

    Response response;
    response.set(http::field::access_control_allow_origin, "*");
    response.body() = Configuration::GetConfigContent(CLIOptions::GetInstance().config);
    co_await http::async_write(socket, response, use_awaitable);
}

bool Owl::GetConfigHandler::Match(const Owl::Request &request) const {
    return request.target() == TARGET && request.method() == Method::get;
}

[[maybe_unused]] static auto reg
        = Owl::Controller::Register(std::make_unique<Owl::GetConfigHandler>());
