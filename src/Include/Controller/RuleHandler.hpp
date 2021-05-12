#pragma once

#include "Handler.hpp"

namespace Owl {
    class RuleHandler : public Handler {
    public:
        using Method = boost::beast::http::verb;
        static const constexpr char *TARGET = "/rules";

        Awaitable<void>
        Handle(const Request &request, Owl::Socket socket, const Connection::Status &status) const override;

        bool Match(const Request &request) const override;
    };
}


