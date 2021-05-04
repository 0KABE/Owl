#include "Controller.hpp"
#include <spdlog/spdlog.h>

Owl::Awaitable<void>
Owl::Controller::Handle(const Owl::Request &request, Owl::Socket socket, const Connection::Status &status) const {
    for (const HandlerPtr &handlerPtr : mHandlerCollection) {
        if (handlerPtr->Match(request)) {
            spdlog::info("Handling {} API request", request.target().to_string());
            co_await handlerPtr->Handle(request, std::move(socket), status);
            co_return;
        }
    }
    spdlog::info("No match any controller handler, http target: {}",
                 request.target().to_string());
}
