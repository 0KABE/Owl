#pragma once

#include "Pipeline/Crypto/Utils.hpp"
#include "Pipeline/Crypto.hpp"
#include <openssl/md5.h>

namespace Owl {
    template<size_t keySize, size_t ivSize>
    class Stream : public Crypto {
    public:
        using Key = std::array<uint8_t, keySize>;
        using IV = std::array<uint8_t, ivSize>;
        using Md5Result = std::array<uint8_t, MD5_DIGEST_LENGTH>;

        explicit Stream(const std::string &password) {
            CipherToKey(password, mKey);
            RandomFill(mIv);
        }

        Buffer &Encrypt(Buffer &input) override {
            if (mSentIV) {
                mSentIV = false;
                auto ivBuffer = mOutput.prepare(ivSize);
                std::copy_n(std::begin(mIv), ivSize, static_cast<uint8_t *>(std::data(ivBuffer)));
                mOutput.commit(ivSize);
                InitCipher();
            }
            EncryptOnce(input, mOutput);
            return mOutput;
        }

        Buffer &Decrypt(Buffer &input) override {
            if (mReceiveIV) {
                mReceiveIV = false;
                std::copy_n(static_cast<const uint8_t *>(std::data(input.data())), ivSize, std::begin(mIv));
                input.consume(ivSize);
                InitDecipher();
            }
            DecryptOnce(input, mOutput);
            return mOutput;
        }

    protected:
        virtual size_t EncryptOnce(Buffer &in, Buffer &out) = 0;

        virtual size_t DecryptOnce(Buffer &in, Buffer &out) = 0;

        virtual void InitCipher() {}

        virtual void InitDecipher() {}

        Key mKey;
        IV mIv;
        Buffer mOutput;

    private:
        bool mSentIV = true;
        bool mReceiveIV = true;
    };
}


