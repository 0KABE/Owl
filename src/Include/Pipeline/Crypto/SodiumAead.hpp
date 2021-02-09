#pragma once

#include "Aead.hpp"

namespace Owl {
    using AeadEncryptMethod = int(unsigned char *c,
                                  unsigned char *mac,
                                  unsigned long long *maclen_p,
                                  const unsigned char *m,
                                  unsigned long long mlen,
                                  const unsigned char *ad,
                                  unsigned long long adlen,
                                  const unsigned char *nsec,
                                  const unsigned char *npub,
                                  const unsigned char *k);
    using AeadDecryptMethod = int(unsigned char *m,
                                  unsigned char *nsec,
                                  const unsigned char *c,
                                  unsigned long long clen,
                                  const unsigned char *mac,
                                  const unsigned char *ad,
                                  unsigned long long adlen,
                                  const unsigned char *npub,
                                  const unsigned char *k);

    template<size_t keySize, size_t saltSize, size_t nonceSize, size_t tagSize, AeadEncryptMethod encryptor, AeadDecryptMethod decryptor>
    class SodiumAEAD : public AEAD<keySize, saltSize, nonceSize, tagSize> {
    public:
        using AEAD = AEAD<keySize, saltSize, nonceSize, tagSize>;
        using AEAD::AEAD;

    protected:
        size_t EncryptOnce(const uint8_t *in, size_t size, uint8_t *out) override {
            unsigned long long mac_len;
            if (!~encryptor(out, out + size, &mac_len, in, size, NULL, 0, NULL,
                            std::data(this->mNonce), std::data(this->mSubKey)))
                throw std::runtime_error("Encrypt data failed.");
            this->NonceIncrement();
            return size;
        }

        size_t DecryptOnce(const uint8_t *in, size_t size, uint8_t *out) override {
            if (!~decryptor(out, NULL, in, size - tagSize, in + size - tagSize, NULL, 0,
                            std::data(this->mNonce), std::data(this->mSubKey)))
                throw std::runtime_error("Decrypt data failed.");
            this->NonceIncrement();
            return size - tagSize;
        }
    };
}


