#pragma once

#include "Stream.hpp"

namespace Owl {
    template<size_t keySize, size_t ivSize>
    class RC4MD5Stream : public Stream<keySize, ivSize> {
    public:
        using Stream = Stream<keySize, ivSize>;
        using typename Stream::Md5Result;
        using typename Stream::Key;
        using typename Stream::IV;

        explicit RC4MD5Stream(const std::string &password)
                : Stream(password),
                  mEvpCipher(EVP_rc4()),
                  mEvpCipherCtx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free) {}

    protected:
        size_t EncryptOnce(Buffer &in, Buffer &out) override {
            int len;
            if (!EVP_CipherUpdate(mEvpCipherCtx.get(),
                                  static_cast<uint8_t *>(out.prepare(in.size()).data()),
                                  &len,
                                  static_cast<const uint8_t *>(in.data().data()),
                                  in.size()))
                throw std::runtime_error("Encrypt data failed.");
            out.commit(len);
            in.consume(len);
            return len;
        }

        size_t DecryptOnce(Buffer &in, Buffer &out) override {
            int len;
            if (!EVP_CipherUpdate(mEvpCipherCtx.get(),
                                  static_cast<uint8_t *>(out.prepare(in.size()).data()),
                                  &len,
                                  static_cast<const uint8_t *>(in.data().data()),
                                  in.size()))
                throw std::runtime_error("Decrypt data failed.");
            out.commit(len);
            in.consume(len);
            return len;
        }

        void InitCipher() override {
            Md5Result md5Result;
            Md5(this->mKey, this->mIv, md5Result);
            EVP_EncryptInit(mEvpCipherCtx.get(), mEvpCipher, md5Result.data(), std::data(this->mIv));
        }

        void InitDecipher() override {
            Md5Result md5Result;
            Md5(this->mKey, this->mIv, md5Result);
            EVP_DecryptInit(mEvpCipherCtx.get(), mEvpCipher, md5Result.data(), std::data(this->mIv));
        }

    private:
        static void Md5(Key &key, const IV &iv, Md5Result &result) {
            MD5_CTX md5_ctx;
            MD5_Init(&md5_ctx);
            MD5_Update(&md5_ctx, key.data(), keySize);
            MD5_Update(&md5_ctx, iv.data(), ivSize);
            MD5_Final(result.data(), &md5_ctx);
        }

        const EVP_CIPHER *mEvpCipher;
        std::unique_ptr<EVP_CIPHER_CTX, void (*)(EVP_CIPHER_CTX *)> mEvpCipherCtx;
    };
}


