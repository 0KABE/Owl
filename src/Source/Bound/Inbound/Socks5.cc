#include "Bound/Inbound/Socks5.hpp"

using Owl::net::use_awaitable;
using Owl::net::detail::socket_ops::host_to_network_long;
using Owl::net::detail::socket_ops::host_to_network_short;
using Owl::net::detail::socket_ops::network_to_host_long;
using Owl::net::detail::socket_ops::network_to_host_short;


std::string Owl::ProtocolDetail::AddressType::Format(Socks5AddressType addressType) {
    std::string fmt;
    switch (addressType) {
        case IP_V4:
            fmt = Owl::net::ip::make_address_v4(IpV4).to_string();
            break;

        case DOMAIN_NAME:
            fmt = std::string(reinterpret_cast<char *>(Domain.Address), Domain.Length);
            break;

        case IP_V6:
            std::array<uint8_t, 16> ip{};
            std::copy(std::begin(IpV6), std::end(IpV6), std::begin(ip));
            fmt = Owl::net::ip::make_address_v6(ip).to_string();
            break;
    }
    return fmt;
}

Owl::Awaitable<void> Owl::ProtocolDetail::operator>>(Owl::net::ip::tcp::socket &socket,
                                                     Owl::ProtocolDetail::HandshakeRequest &request) {
    co_await Owl::net::async_read(
            socket,
            Owl::net::buffer(static_cast<void *>(&request),
                             sizeof(request.Ver) + sizeof(request.NumMethods)),
            use_awaitable);
    co_await Owl::net::async_read(
            socket,
            Owl::net::buffer(static_cast<void *>(request.Methods),
                             request.NumMethods),
            use_awaitable);
}

Owl::Awaitable<void> Owl::ProtocolDetail::operator<<(Owl::net::ip::tcp::socket &socket,
                                                     const Owl::ProtocolDetail::HandshakeReply &reply) {
    co_await Owl::net::async_write(
            socket,
            Owl::net::buffer(static_cast<const void *>(&reply), sizeof(reply)),
            use_awaitable);
}

Owl::Awaitable<void> Owl::ProtocolDetail::operator>>(Owl::net::ip::tcp::socket &socket,
                                                     Owl::ProtocolDetail::Socks5Request &request) {
    using namespace Owl::ProtocolDetail;

    co_await Owl::net::async_read(
            socket,
            Owl::net::buffer(&request, sizeof(request.Ver) + sizeof(request.Cmd) +
                                       sizeof(request.Rsv) +
                                       sizeof(request.AType)),
            use_awaitable);

    switch (request.AType) {
        case IP_V4:
            co_await Owl::net::async_read(
                    socket,
                    Owl::net::buffer(&request.DstAddress.IpV4,
                                     sizeof(request.DstAddress.IpV4)),
                    use_awaitable);
            request.DstAddress.IpV4 = network_to_host_long(request.DstAddress.IpV4);
            break;

        case DOMAIN_NAME:
            co_await Owl::net::async_read(
                    socket,
                    Owl::net::buffer(&request.DstAddress.Domain.Length,
                                     sizeof(request.DstAddress.Domain.Length)),
                    use_awaitable);
            co_await Owl::net::async_read(
                    socket,
                    Owl::net::buffer(request.DstAddress.Domain.Address,
                                     request.DstAddress.Domain.Length),
                    use_awaitable);
            break;

        case IP_V6:
            co_await Owl::net::async_read(
                    socket,
                    Owl::net::buffer(&request.DstAddress.IpV6,
                                     sizeof(request.DstAddress.IpV6)),
                    use_awaitable);
            break;
    }

    co_await Owl::net::async_read(
            socket, Owl::net::buffer(&request.Port, sizeof(request.Port)),
            use_awaitable);
    request.Port = network_to_host_short(request.Port);
}

Owl::Awaitable<void> Owl::ProtocolDetail::operator<<(Owl::net::ip::tcp::socket &socket,
                                                     const Owl::ProtocolDetail::Socks5Reply &reply) {
    using namespace Owl::ProtocolDetail;

    std::vector<uint8_t> sent;
    sent.push_back(reply.Ver);
    sent.push_back(reply.Rep);
    sent.push_back(reply.Rsv);
    sent.push_back(reply.AType);
    switch (reply.AType) {
        case IP_V4: {
            uint32_t v4 = host_to_network_long(reply.BndAddress.IpV4);
            std::copy_n(reinterpret_cast<uint8_t *>(&v4), sizeof(v4),
                        std::back_inserter(sent));
            break;
        }

        case DOMAIN_NAME: {
            *std::back_inserter(sent) = reply.BndAddress.Domain.Length;
            std::copy_n(reply.BndAddress.Domain.Address, reply.BndAddress.Domain.Length,
                        std::back_inserter(sent));
            break;
        }

        case IP_V6: {
            std::copy(std::begin(reply.BndAddress.IpV6), std::begin(reply.BndAddress.IpV6),
                      std::back_inserter(sent));
            break;
        }
    }
    uint16_t port = host_to_network_short(reply.Port);
    std::copy_n(reinterpret_cast<const uint8_t *>(&port), sizeof(port),
                std::back_inserter(sent));

    co_await Owl::net::async_write(socket, Owl::net::buffer(sent),
                                   use_awaitable);
}
