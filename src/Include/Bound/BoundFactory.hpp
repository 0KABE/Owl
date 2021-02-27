#pragma once

#include "Util/Concepts.hpp"
#include "Bound.hpp"
#include "Outbound/ShadowsocksClient.hpp"

namespace Owl {
    class BoundFactory {
    public:
        static constexpr size_t DEFAULT_BUFFER_SIZE = 0x400;

        template<typename T>
        requires Derived<Bound, T>
        static std::unique_ptr<T>
        Create(Endpoint::Socket socket, Bound::BufferSize bufferSize = DEFAULT_BUFFER_SIZE) {
            return std::make_unique<T>(Endpoint(std::move(socket)), bufferSize);
        }

        template<typename T>
        requires Derived<Bound, T>
        static std::unique_ptr<T>
        Create(Endpoint endpoint, Bound::BufferSize bufferSize = DEFAULT_BUFFER_SIZE) {
            return std::make_unique<T>(std::move(endpoint), bufferSize);
        }

        static std::unique_ptr<ShadowsocksClient>
        CreateProxy(Pipeline::PipelinePtr pipelinePtr, Endpoint endpoint, Endpoint proxyEndpoint,
                    Bound::BufferSize bufferSize = DEFAULT_BUFFER_SIZE) {
            return std::make_unique<ShadowsocksClient>(std::move(endpoint),
                                                       std::move(proxyEndpoint),
                                                       bufferSize,
                                                       std::move(pipelinePtr));
        }
    };
}
