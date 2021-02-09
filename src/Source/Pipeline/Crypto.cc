#include "Pipeline/Crypto.hpp"

Owl::Buffer &Owl::Crypto::Wrap(Owl::Buffer &buffer) {
    return Encrypt(buffer);
}

Owl::Buffer &Owl::Crypto::UnWrap(Owl::Buffer &buffer) {
    return Decrypt(buffer);
}
