#pragma once

#include "Util/Net.hpp"
#include "Util/Awaitable.hpp"
#include <boost/beast.hpp>
#include <Connection.hpp>

namespace Owl {
    using StringBody = boost::beast::http::string_body;
    using Request = boost::beast::http::request<StringBody>;
    using Socket = net::ip::tcp::socket;

    class Handler {
    public:
        [[nodiscard]] virtual Owl::Awaitable<void>
        Handle(const Owl::Request &request, Owl::Socket socket, const Owl::Connection::Status &status) const = 0;

        [[nodiscard]] virtual bool Match(const Request &request) const = 0;

        virtual ~Handler() = default;
    };

    using HandlerPtr = std::unique_ptr<Handler>;
}


