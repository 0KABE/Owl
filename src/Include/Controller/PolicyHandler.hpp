#pragma once

#include "Handler.hpp"

namespace Owl {
    class PolicyHandler : public Handler {
    public:
        static const constexpr char *TARGET = "/policies";

        Awaitable<void>
        Handle(const Request &request, Owl::Socket socket, const Connection::Status &status) const override;

        bool Match(const Request &request) const override;
    };
}


