#pragma once

#include "Bound/Outbound.hpp"
#include "Pipeline.hpp"

namespace Owl {
    class ShadowsocksClient : public Outbound {
    public:
        struct Address {
            const uint8_t aType = 0x03;
            uint8_t length;
            uint8_t domain[0xff];
            uint16_t port;

            Address(const std::string &hostname, const std::string &port);

            friend Buffer &operator<<(Buffer &buffer, const Address &address);
        };

        ShadowsocksClient(Endpoint endpoint, Endpoint proxyEndpoint, BufferSize bufferSize,
                          Pipeline::PipelinePtr pipelinePtr);

        Awaitable<void> Send(Buffer &buffer) override;

        Awaitable<std::reference_wrapper<Buffer>> Receive() override;

    private:
        Awaitable<void> Initialize();

        Pipeline::PipelinePtr mPipelinePtr;
        Endpoint mProxyEndpoint;
    };
}


