#pragma once

#include "Util/Awaitable.hpp"


namespace Owl::ProtocolDetail {
    enum {
        VERSION = 0x05
    };

    enum AuthenticationMethod {
        NO_AUTHENTICATION = 0x00,
        GSSAPI = 0x01,
        USERNAME_PASSWORD = 0x02,
        NO_ACCEPTABLE = 0xff
    };

    struct HandshakeRequest {
        uint8_t Ver;
        uint8_t NumMethods;
        uint8_t Methods[0x100];

        friend Awaitable<void>
        operator>>(Owl::net::ip::tcp::socket &socket, HandshakeRequest &request);
    };

    struct HandshakeReply {
        uint8_t Ver;
        uint8_t Method;

        friend Awaitable<void>
        operator<<(Owl::net::ip::tcp::socket &socket, const HandshakeReply &reply);
    };

    enum Socks5RequestCmd {
        CONNECT = 0x01, BIND = 0x02, UDP = 0x03
    };
    enum Socks5AddressType {
        IP_V4 = 0x01, DOMAIN_NAME = 0x03, IP_V6 = 0x04
    };

    union AddressType {
        uint32_t IpV4;
        struct {
            uint8_t Length;
            uint8_t Address[0xff];
        } Domain;
        uint8_t IpV6[0x10];

        std::string Format(Socks5AddressType addressType);
    };

    struct Socks5Request {
        uint8_t Ver;
        uint8_t Cmd;
        uint8_t Rsv;
        uint8_t AType;
        AddressType DstAddress;
        uint16_t Port;

        friend Awaitable<void>
        operator>>(Owl::net::ip::tcp::socket &socket, Socks5Request &request);
    };

    enum Socks5ReplyRep {
        SUCCEEDED = 0x00,
        GENERAL_SOCKS_SERVER_FAILURE = 0x01,
        CONNECTION_NOT_ALLOWED_BY_RULE_SET = 0x02,
        NETWORK_UNREACHABLE = 0x03,
        HOST_UNREACHABLE = 0x04,
        CONNECTION_REFUSED = 0x05,
        TTL_EXPIRED = 0x06,
        COMMAND_NOT_SUPPORTED = 0x07,
        ADDRESS_TYPE_NOT_SUPPORTED = 0x08
    };

    struct Socks5Reply {
        uint8_t Ver;
        uint8_t Rep;
        uint8_t Rsv;
        uint8_t AType;
        AddressType BndAddress;
        uint16_t Port;

        friend Awaitable<void>
        operator<<(Owl::net::ip::tcp::socket &socket, const Socks5Reply &request);
    };
}


