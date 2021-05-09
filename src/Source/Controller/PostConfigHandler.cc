#include "Configuration.hpp"
#include "Controller/PostConfigHandler.hpp"
#include "Controller.hpp"
#include "CliOptions.hpp"

Owl::Awaitable<void> Owl::PostConfigHandler::Handle(const Owl::Request &request, Owl::Socket socket,
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

    //Save config
    std::ofstream(CLIOptions::GetInstance().config, std::ios_base::trunc) << request.body();

    //Reload config
    Configuration::Load(CLIOptions::GetInstance().config, socket.get_executor());

    co_await http::async_write(socket, response, use_awaitable);
}

bool Owl::PostConfigHandler::Match(const Owl::Request &request) const {
    return request.target() == TARGET
           && (request.method() == Method::post || request.method() == Method::options);
}

[[maybe_unused]] static auto reg
        = Owl::Controller::Register(std::make_unique<Owl::PostConfigHandler>());
