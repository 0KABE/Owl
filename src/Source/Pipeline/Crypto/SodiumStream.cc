#include "Pipeline/Crypto/SodiumStream.hpp"
#include "Pipeline/Crypto/CryptoFactory.hpp"

using chacha20 = Owl::SodiumStream<32, 12, crypto_stream_chacha20_ietf_xor_ic>;
static const Owl::CryptoFactory::Register<chacha20>
        register_chacha20("chacha20-ietf");