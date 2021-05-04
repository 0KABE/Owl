#pragma once

#include "Handler.hpp"

namespace Owl {
    class TrafficHandler : public Handler {
    public:
        static constexpr const char *TARGET = "/traffic";

        Awaitable<void>
        Handle(const Owl::Request &request, Owl::Socket socket, const Connection::Status &status) const override;

        [[nodiscard]] bool Match(const Request &request) const override;

    private:
        [[nodiscard]] static std::string GetTraffic() ;
    };
}


