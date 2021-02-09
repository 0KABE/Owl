#pragma once

#include "Aead.hpp"

namespace Owl {
    using EvpCipher = const EVP_CIPHER *();

    template<size_t keySize, size_t saltSize, size_t nonceSize, size_t tagSize, EvpCipher cipher>
    class OpensslAEAD : public AEAD<keySize, saltSize, nonceSize, tagSize> {
    public:
        using AEAD = AEAD<keySize, saltSize, nonceSize, tagSize>;
        using typename AEAD::Nonce;
        using typename AEAD::SubKey;

        explicit OpensslAEAD(const std::string &password)
                : AEAD(password), mEvpCipher(cipher()), mCtx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free) {}

        size_t EncryptOnce(const uint8_t *in, size_t size, uint8_t *out) override {
            int len, final_len;
            InitCipher(mCtx.get(), this->mSubKey, this->mNonce, true);
            if (!EVP_EncryptUpdate(mCtx.get(), out, &len, in, size))
                throw std::runtime_error("EVP_EncryptUpdate failed.");
            if (!EVP_EncryptFinal(mCtx.get(), out + len, &final_len))
                throw std::runtime_error("EVP_EncryptFinal failed.");
            EVP_CIPHER_CTX_ctrl(mCtx.get(), EVP_CTRL_GCM_GET_TAG, tagSize, out + len);
            this->NonceIncrement();
            return len;
        }

        size_t DecryptOnce(const uint8_t *in, size_t size, uint8_t *out) override {
            int len, final_len;
            InitCipher(mCtx.get(), this->mSubKey, this->mNonce, false);
            EVP_CIPHER_CTX_ctrl(mCtx.get(), EVP_CTRL_GCM_SET_TAG, tagSize,
                                (uint8_t *) in + size - tagSize);
            if (!EVP_DecryptUpdate(mCtx.get(), out, &len, in, size - tagSize))
                throw std::runtime_error("EVP_DecryptUpdate failed.");
            if (!EVP_DecryptFinal(mCtx.get(), out + len, &final_len))
                throw std::runtime_error("EVP_DecryptFinal failed.");
            this->NonceIncrement();
            return len;
        }

    private:
        void InitCipher(EVP_CIPHER_CTX *ctx, SubKey &key, Nonce &nonce, bool encryption) {
            EVP_CipherInit(ctx, mEvpCipher, nullptr, nullptr, encryption);
            EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_SET_KEY_LENGTH, key.size(), nullptr);
            EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, nonce.size(), nullptr);
            EVP_CipherInit(ctx, nullptr, key.data(), nonce.data(), encryption);
        }

        const EVP_CIPHER *mEvpCipher;
        std::unique_ptr<EVP_CIPHER_CTX, void (*)(EVP_CIPHER_CTX *)> mCtx;
    };
}


