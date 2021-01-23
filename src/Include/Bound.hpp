#pragma once

#include "Util/Buffer.hpp"
#include "Util/Awaitable.hpp"
#include "Util/Endpoint.hpp"

namespace Owl {
    class Bound {
    public:
        using BoundPtr = std::unique_ptr<Bound>;
        using BufferSize = size_t;
        using TargetEndpoint = std::optional<Endpoint>;

        explicit Bound(Endpoint endpoint, BufferSize bufferSize);

        virtual ~Bound() = default;

        // TODO:Should be redesigned this method.
        //  Some of Derived class will not get TargetEndpoint
        virtual Awaitable<TargetEndpoint> Initialize() = 0;

        virtual Awaitable<void> Send(Buffer &buffer) = 0;

        virtual Awaitable<void> Receive() = 0;

        Endpoint &GetEndpoint();

        Buffer &GetReceiveBuffer();

    protected:
        Endpoint mEndpoint;
        BufferSize mBufferSize;
        Buffer mBuffer;
        bool mUninitialized = true;
    };
}


