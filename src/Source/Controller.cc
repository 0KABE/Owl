#include "Controller.hpp"
#include <spdlog/spdlog.h>

Owl::Awaitable<void> Owl::Controller::Handle(const Owl::Request &request, Owl::Socket socket) const {
    for (const HandlerPtr &handlerPtr : mHandlerCollection) {
        if (handlerPtr->Match(request)) {
            co_await handlerPtr->Handle(request, std::move(socket));
            co_return;
        }
    }
    spdlog::info("No match any controller handler, http target: {}",
                 request.target().to_string());
}
