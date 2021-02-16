#include "Pipeline/SimpleObfsClient.hpp"

#include <utility>

Owl::Buffer &Owl::SimpleObfsClient::Wrap(Owl::Buffer &buffer) {
    return Obfuscate(buffer);
}

Owl::Buffer &Owl::SimpleObfsClient::UnWrap(Owl::Buffer &buffer) {
    return DeObfuscate(buffer);
}

Owl::SimpleObfsClient::SimpleObfsClient(std::string hostname) : mHostname(std::move(hostname)) {}
