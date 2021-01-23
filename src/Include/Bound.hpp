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

        virtual Awaitable<void> Receive() = 0;

        Endpoint &GetEndpoint();

        Buffer &GetReceiveBuffer();

    protected:
        Endpoint mEndpoint;
        BufferSize mBufferSize;
        Buffer mBuffer;
        bool mUninitialized = true;
    };

    template<typename T>
    concept BoundDerived = std::is_base_of<Bound, T>::value && !std::is_same<Bound, T>::value;
}

