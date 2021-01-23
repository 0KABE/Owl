#include "Bound/Outbound/Direct.hpp"

Owl::Awaitable<Owl::Bound::TargetEndpoint> Owl::Direct::Initialize() {
    if (mUninitialized) {
        co_await mEndpoint.Connect();
        mUninitialized = false;
    }
    co_return std::nullopt;
}

Owl::Awaitable<void> Owl::Direct::Send(Owl::Buffer &buffer) {
    co_await Initialize();
    co_await net::async_write(mEndpoint.GetSocket(), buffer, use_awaitable);
}

Owl::Awaitable<void> Owl::Direct::Receive() {
    co_await Initialize();
    size_t n = co_await mEndpoint.GetSocket().async_read_some(mBuffer.prepare(mBufferSize), use_awaitable);
    this->mBuffer.commit(n);
}


