#include "Bound.hpp"

Owl::Bound::Bound(Endpoint endpoint, BufferSize bufferSize)
        : mEndpoint(std::move(endpoint)), mBufferSize(bufferSize) {}

Owl::Endpoint &Owl::Bound::GetEndpoint() { return mEndpoint; }

Owl::Buffer &Owl::Bound::GetReceiveBuffer() { return mBuffer; }

void Owl::Bound::Shutdown() {
    mEndpoint.Disconnect();
}
