#pragma once

#include "Handler.hpp"

namespace Owl {
    class PostConfigHandler : public Handler {
    public:
        using Method = boost::beast::http::verb;
        static const constexpr char *TARGET = "/config";

        Awaitable<void>
        Handle(const Request &request, Owl::Socket socket, const Connection::Status &status) const override;

        bool Match(const Request &request) const override;
    };
}


