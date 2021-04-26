#include "Bound/Outbound/ShadowsocksClient.hpp"

Owl::ShadowsocksClient::ShadowsocksClient(Owl::Endpoint endpoint,
                                          Owl::Endpoint proxyEndpoint,
                                          Owl::Bound::BufferSize bufferSize,
                                          Owl::PipelinePtr pipelinePtr)
        : Outbound(std::move(endpoint), bufferSize),
          mPipelinePtr(std::move(pipelinePtr)),
          mProxyEndpoint(std::move(proxyEndpoint)) {}

Owl::Awaitable<void> Owl::ShadowsocksClient::Send(Owl::Buffer &buffer) {
    co_await Initialize();
    Buffer &bufferForSending = mPipelinePtr->Wrap(buffer);
    spdlog::debug("Sending data to {}", mProxyEndpoint.ToString());
    co_await net::async_write(mProxyEndpoint.GetSocket(), bufferForSending, use_awaitable);
}

Owl::Awaitable<std::reference_wrapper<Owl::Buffer>> Owl::ShadowsocksClient::Receive() {
    co_await mProxyEndpoint.Connect();
    spdlog::debug("Receiving data from {}", mProxyEndpoint.ToString());
    size_t n = co_await mProxyEndpoint.GetSocket().async_read_some(mBuffer.prepare(mBufferSize), use_awaitable);
    mBuffer.commit(n);
    co_return mPipelinePtr->UnWrap(mBuffer);
}

Owl::Awaitable<void> Owl::ShadowsocksClient::Initialize() {
    co_await mProxyEndpoint.Connect();
    if (mUninitialized) {
        spdlog::debug("Initializing Shadowsocks Client");
        mUninitialized = false;

        Buffer buffer;
        buffer << Address(mEndpoint.GetHostname(), mEndpoint.GetPort());
        co_await Send(buffer);
        spdlog::debug("Succeed in initializing Shadowsocks Client");
    }
}

Owl::ShadowsocksClient::Address::Address(const std::string &hostname, const std::string &port)
        : length(std::size(hostname)), port(std::stoul(port)) {
    std::copy(std::begin(hostname), std::end(hostname), std::begin(domain));
}

Owl::Buffer &Owl::operator<<(Owl::Buffer &buffer, const Owl::ShadowsocksClient::Address &address) {
    const size_t length = sizeof(address.aType) + sizeof(address.length) + address.length + sizeof(address.port);
    auto mutableBuffer = buffer.prepare(length);
    auto *data = static_cast<uint8_t *> (mutableBuffer.data());

    // copy all data except port
    std::copy_n(&address.aType, length - 2, data);

    // copy port
    data += length - 2;
    *reinterpret_cast<uint16_t *>(data) = net::detail::socket_ops::host_to_network_short(address.port);

    buffer.commit(mutableBuffer.size());
    return buffer;
}
