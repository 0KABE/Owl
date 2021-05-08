#pragma once

#include "Handler.hpp"

namespace Owl {
    class SetSelectedProxyHandler : public Handler {
    public:
        using Method = boost::beast::http::verb;
        static const constexpr char *TARGET = "/setProxy";
        static const constexpr Method METHOD = Method::post;

        [[nodiscard]] Awaitable<void>
        Handle(const Request &request, Owl::Socket socket, const Connection::Status &status) const override;

        [[nodiscard]] bool Match(const Request &request) const override;
    };
}


