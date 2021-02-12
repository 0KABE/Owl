#pragma once

#include "Stream.hpp"

namespace Owl {
    using CipherFun = const EVP_CIPHER *();

    template<size_t keySize, size_t ivSize, CipherFun cipherFun>
    class OpensslStream : public Stream<keySize, ivSize> {
    public:
        using Stream = Stream<keySize, ivSize>;
        using typename Stream::IV;
        using typename Stream::Key;

        explicit OpensslStream(const std::string &password)
                : Stream(password),
                  mEvpCipher(cipherFun()),
                  mCipherCtx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free) {}

    protected:
        size_t EncryptOnce(Buffer &in, Buffer &out) override {
            int len;
            if (!EVP_CipherUpdate(mCipherCtx.get(),
                                  static_cast<uint8_t *>(out.prepare(in.size()).data()),
                                  &len,
                                  static_cast<const uint8_t *>(in.data().data()),
                                  in.size())) {
                throw std::runtime_error("Encrypt data failed.");
            }
            out.commit(len);
            in.consume(len);
            return len;
        }

        size_t DecryptOnce(Buffer &in, Buffer &out) override {
            int len;
            if (!EVP_CipherUpdate(mCipherCtx.get(),
                                  static_cast<uint8_t *>(out.prepare(in.size()).data()),
                                  &len,
                                  static_cast<const uint8_t *>(in.data().data()),
                                  in.size())) {
                throw std::runtime_error("Decrypt data failed.");
            }
            out.commit(len);
            in.consume(len);
            return len;
        }

        void InitCipher() override {
            EVP_EncryptInit(mCipherCtx.get(),
                            mEvpCipher,
                            std::data(this->mKey),
                            std::data(this->mIv));
        }

        void InitDecipher() override {
            EVP_DecryptInit(mCipherCtx.get(),
                            mEvpCipher,
                            std::data(this->mKey),
                            std::data(this->mIv));
        }

    private:
        const EVP_CIPHER *mEvpCipher;
        std::unique_ptr<EVP_CIPHER_CTX, void (*)(EVP_CIPHER_CTX *)> mCipherCtx;
    };
}


