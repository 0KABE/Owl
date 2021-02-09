#pragma once

#include <utility>
#include <sodium.h>

#include "Pipeline/Crypto.hpp"
#include "Utils.hpp"
#include "Hkdf.hpp"

namespace Owl {
    /**
     *
     * @tparam keySize
     * @tparam saltSize
     * @tparam nonceSize
     * @tparam tagSize
     *
     * @note [Reference Shadowsocks White paper Chapter 3 AEAD](https://shadowsocks.org/assets/whitepaper.pdf)
     */
    template<size_t keySize, size_t saltSize, size_t nonceSize, size_t tagSize>
    class AEAD : public Crypto {
    public:
        static const size_t SUBKEY_LENGTH = 32;
        static const constexpr char *HKDF_DIGEST_ALGORITHM = "sha1";

        using Key = std::array<uint8_t, keySize>;
        using SubKey = std::array<uint8_t, SUBKEY_LENGTH>;
        using Salt = std::array<uint8_t, saltSize>;
        using Nonce = std::array<uint8_t, nonceSize>;
        using Tag = std::array<uint8_t, tagSize>;

        union PayloadLength {
            uint8_t SingleBytes[2];
            uint16_t DoubleBytes;
        };

        explicit AEAD(std::string password) : mPassword(std::move(password)) {}

        Buffer &Encrypt(Buffer &input) override {
            SendSalt();
            SendPayloadLength(input);
            SendPayload(input);
            return mOutput;
        }

        Buffer &Decrypt(Buffer &input) override {
            if (ReceiveSalt(input)) {
                while (input.size()) {
                    if ((mReceivePayloadLength && !ReceivePayloadLength(input))
                        || !ReceivePayload(input))
                        break;
                }
            }
            return mOutput;
        }

    protected:
        /**
         *
         * @param in input pointer
         * @param size input data size
         * @param out pointer which should guarantee there are sufficient space to store output data
         * @return
         */
        virtual size_t EncryptOnce(const uint8_t *in, size_t size, uint8_t *out) = 0;

        /**
         *
         * @param in input pointer
         * @param size input data size
         * @param out pointer which should guarantee there are sufficient space to store output data
         * @return
         */
        virtual size_t DecryptOnce(const uint8_t *in, size_t size, uint8_t *out) = 0;

        void NonceIncrement() {
            sodium_increment(std::data(mNonce), std::size(mNonce));
        }

        bool mSaltSent = false;
        bool mReceiveSalt = true;
        bool mReceivePayloadLength = true;
        const std::string mPassword;
        PayloadLength mPayloadLength;
        Key mKey;
        SubKey mSubKey;
        Salt mSalt;
        Nonce mNonce;
        Buffer mOutput;

    private:
        void SendSalt() {
            if (!mSaltSent) {
                mSaltSent = true;
                RandomFill(mSalt);
                CipherToKey(mPassword, mKey);
                HKDF(mSalt, mKey, HKDF_DIGEST_ALGORITHM).Expand(mSubKey);
                auto saltBuff = mOutput.prepare(sizeof(mSalt));
                std::copy(std::begin(mSalt), std::end(mSalt),
                          static_cast<uint8_t *>(std::data(saltBuff)));;
                mOutput.commit(std::size(mSalt));
            }
        }

        void SendPayloadLength(Buffer &buffer) {
            using boost::asio::detail::socket_ops::host_to_network_short;
            mPayloadLength.DoubleBytes = host_to_network_short(buffer.size());
            auto payloadLengthBuffer = mOutput.prepare(sizeof(PayloadLength) + tagSize);
            EncryptOnce(mPayloadLength.SingleBytes, sizeof(PayloadLength),
                        static_cast<uint8_t *> (std::data(payloadLengthBuffer)));
            mOutput.commit(sizeof(PayloadLength) + tagSize);
        }

        void SendPayload(Buffer &buffer) {
            auto payloadBuffer = mOutput.prepare(std::size(buffer) + tagSize);
            EncryptOnce(static_cast<const uint8_t *> (std::data(buffer.data())), buffer.size(),
                        static_cast<uint8_t *>(payloadBuffer.data()));
            mOutput.commit(buffer.size() + tagSize);
            buffer.consume(buffer.size());
        }

        /**
         *
         * @param buffer
         * @return true for receiving salt success, false for receiving salt failure
         */
        bool ReceiveSalt(Buffer &buffer) {
            // only if not receive salt yet and input has whole salt data
            if (mReceiveSalt && buffer.size() >= std::size(mSalt)) {
                mReceiveSalt = false;
                std::copy_n(static_cast<const uint8_t *>(std::data(buffer.data())),
                            std::size(mSalt), std::begin(mSalt));
                buffer.consume(std::size(mSalt));
                CipherToKey(mPassword, mKey);
                HKDF(mSalt, mKey, HKDF_DIGEST_ALGORITHM).Expand(mSubKey);
            }
            return !mReceiveSalt;
        }

        /**
         *
         * @param buffer
         * @return true for receiving PayloadLength success, false for receiving PayloadLength failure
         */
        bool ReceivePayloadLength(Buffer &buffer) {
            using boost::asio::detail::socket_ops::network_to_host_short;
            // receive the PayloadLength only when the PayloadLength has been sent
            if (buffer.size() < sizeof(PayloadLength) + tagSize) return true;

            mReceivePayloadLength = false;
            DecryptOnce(static_cast<const uint8_t *> (std::data(buffer.data())),
                        sizeof(PayloadLength) + tagSize,
                        mPayloadLength.SingleBytes);
            buffer.consume(sizeof(PayloadLength) + tagSize);
            mPayloadLength.DoubleBytes = network_to_host_short(mPayloadLength.DoubleBytes);
            return false;
        }

        /**
         *
         * @param buffer
         * @return true for receiving Payload success, false for receiving Payload failure
         */
        bool ReceivePayload(Buffer &buffer) {
            // receive the Payload when the whole payload has been sent
            if (buffer.size() < mPayloadLength.DoubleBytes + tagSize) return true;

            auto outputBuffer = mOutput.prepare(mPayloadLength.DoubleBytes);
            DecryptOnce(static_cast<const uint8_t *>(std::data(buffer.data())),
                        mPayloadLength.DoubleBytes + tagSize,
                        static_cast<uint8_t *>(outputBuffer.data()));
            mOutput.commit(mPayloadLength.DoubleBytes);
            buffer.consume(mPayloadLength.DoubleBytes + tagSize);
            mReceivePayloadLength = true;
            return false;
        }
    };
}
