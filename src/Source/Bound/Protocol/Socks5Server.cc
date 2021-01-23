#include "Bound/Protocol/Socks5Server.hpp"
#include "Bound/Protocol/Socks5.hpp"

Owl::Awaitable<Owl::Bound::TargetEndpoint> Owl::Socks5Server::Initialize() {
    using namespace ProtocolDetail;

    bool authenticated = false;
    HandshakeRequest request{};
    HandshakeReply reply{VERSION, NO_ACCEPTABLE};

    co_await (mEndpoint.GetSocket() >> request);
    if (request.Ver == VERSION) {
        for (int i = 0; i < request.NumMethods; ++i) {
            switch (request.Methods[i]) {
                case NO_AUTHENTICATION:
                    reply = {VERSION, NO_AUTHENTICATION};
                    authenticated = true;
                    break;
            }
        }
    }

    co_await (mEndpoint.GetSocket() << reply);

    co_return authenticated ? co_await Handshake() : std::nullopt;
}

Owl::Awaitable<Owl::Bound::TargetEndpoint> Owl::Socks5Server::Handshake() {
    using namespace ProtocolDetail;

    TargetEndpoint endpoint;

    Socks5Request request{};
    co_await (mEndpoint.GetSocket() >> request);

    Socks5Reply reply{.Ver     = VERSION,
            .Rep     = COMMAND_NOT_SUPPORTED,
            .Rsv     = 0x00,
            .AType    = request.AType,
            .BndAddress = request.DstAddress,
            .Port    = request.Port};
    if (request.Ver == VERSION) {
        switch (request.Cmd) {
            case CONNECT:
                endpoint = Endpoint(request.DstAddress.Format(Socks5AddressType(request.AType)),
                                    std::to_string(request.Port));
                reply = {.Ver     = VERSION,
                        .Rep     = SUCCEEDED,
                        .Rsv     = 0x00,
                        .AType    = request.AType,
                        .BndAddress = request.DstAddress,
                        .Port    = request.Port};
                break;
        }
    }
    co_await (mEndpoint.GetSocket() << reply);

    mUninitialized = false;
    co_return endpoint;
}

Owl::Awaitable<void> Owl::Socks5Server::Send(Owl::Buffer &buffer) {
    co_await net::async_write(mEndpoint.GetSocket(), buffer, use_awaitable);
}

Owl::Awaitable<void> Owl::Socks5Server::Receive() {
    size_t n = co_await mEndpoint.GetSocket().async_read_some(mBuffer.prepare(mBufferSize), use_awaitable);
    mBuffer.commit(n);
}
