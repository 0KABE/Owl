#pragma once

#include "Util/Net.hpp"
#include "Util/Awaitable.hpp"
#include <boost/beast.hpp>

namespace Owl {
    using StringBody = boost::beast::http::string_body;
    using Request = boost::beast::http::request<StringBody>;
    using Socket = net::ip::tcp::socket;

    class Handler {
    public:
        [[nodiscard]] virtual Awaitable<void> Handle(const Request &request, Socket socket) const = 0;

        [[nodiscard]] virtual bool Match(const Request &request) const = 0;

        virtual ~Handler() = default;
    };

    using HandlerPtr = std::unique_ptr<Handler>;
}


