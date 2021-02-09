#include <sodium.h>
#include "Pipeline/Crypto/SodiumAead.hpp"
#include "Pipeline/Crypto/CryptoFactory.hpp"

using chacha20_ietf_poly1305 = Owl::SodiumAEAD<32, 32, 12, 16,
        crypto_aead_chacha20poly1305_ietf_encrypt_detached,
        crypto_aead_chacha20poly1305_ietf_decrypt_detached>;
static const Owl::CryptoFactory::Register <chacha20_ietf_poly1305>
        register_chacha20_ietf_poly1305("chacha20-ietf-poly1305");
