#pragma once

#include "Util/Buffer.hpp"
#include "Util/Awaitable.hpp"
#include "Util/Endpoint.hpp"

namespace Owl {
    class Bound {
    public:
        using BufferSize = size_t;
        using TargetEndpoint = std::optional<Endpoint>;

        explicit Bound(Endpoint endpoint, BufferSize bufferSize);

        virtual ~Bound() = default;

        virtual Awaitable<void> Send(Buffer &buffer) = 0;

        virtual Awaitable <std::reference_wrapper<Buffer>> Receive() = 0;

        Endpoint &GetEndpoint();

        Buffer &GetReceiveBuffer();

        void Shutdown();

    protected:
        Endpoint mEndpoint;
        BufferSize mBufferSize;
        Buffer mBuffer;
        bool mUninitialized = true;
    };
}


