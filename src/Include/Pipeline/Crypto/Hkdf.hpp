#pragma once

#include <openssl/hmac.h>
#include <sodium.h>
#include <spdlog/spdlog.h>
#include "TypeTraits.hpp"
#include "Util/FinalAction.hpp"

namespace Owl {

    /**
     * @brief HKDF is a simple key derivation function (KDF)
     * based on a hash-based message authentication code (HMAC)
     *
     * https://tools.ietf.org/html/rfc5869
     */
    class HKDF {
    public:
        static constexpr const char *SUB_KEY_INFO = "ss-subkey";

        template<typename T, typename U>
        requires IsArrayOrVector<T> && IsArrayOrVector<U>
        HKDF(const T &salt, const U &inputKeyMaterial, const std::string &digestAlgorithm):
                mDigest(EVP_get_digestbyname(std::data(digestAlgorithm))),
                mPseudoRandomKey(Extract(salt, inputKeyMaterial, mDigest)) {}

        template<typename T>
        requires IsArrayOrVector<T>
        void Expand(T &result, const std::string &info = SUB_KEY_INFO) {
            Expand(mPseudoRandomKey, info, std::size(result), mDigest, result);
        }

        template<typename T, typename U>
        requires IsArrayOrVector<T> && IsArrayOrVector<U>
        static std::vector<uint8_t> Hmac(const T &key, const U &message, const EVP_MD *evpMd) {
            uint32_t len;
            std::vector<uint8_t> hash(EVP_MD_size(evpMd));
            HMAC(evpMd,
                 std::data(key), std::size(key),
                 std::data(message), std::size(message),
                 std::data(hash), &len);
            return hash;
        }

        template<typename T, typename U>
        requires IsArrayOrVector<T> && IsArrayOrVector<U>
        static void
        Expand(const T &pseudoRandomKey, const std::string &info, size_t length, const EVP_MD *md, U &result) {
            int hashLen = EVP_MD_size(md);
            if (length > (size_t) 0xff * hashLen) {
                std::string errorMessage = fmt::format(
                        "Cannot expand to more than 255 * {} bytes using the specified hash function", hashLen);
                spdlog::critical(errorMessage);
                throw std::length_error(errorMessage);
            }

            std::vector<uint8_t> outputBlock;
            int count = 0;
            auto it = std::begin(result);
            while (length) {
                std::vector<uint8_t> tmp;
                std::copy(std::begin(outputBlock), std::end(outputBlock),
                          std::back_inserter(tmp));
                std::copy(std::begin(info), std::end(info), std::back_inserter(tmp));
                tmp.push_back(++count);
                outputBlock = Hmac(pseudoRandomKey, tmp, md);
                std::copy_n(std::begin(outputBlock),
                            std::min(length, std::size(outputBlock)), it);
                it += std::min(length, std::size(outputBlock));
                length -= std::min(length, std::size(outputBlock));
            }
        }

        template<typename T, typename U>
        requires IsArrayOrVector<T> && IsArrayOrVector<U>
        static std::vector<uint8_t> Extract(const T &salt, const U &inputKeyMaterial, const EVP_MD *md) {
            EVP_MD_CTX *ctx = EVP_MD_CTX_new();
            FinalAction finalAction([&] { EVP_MD_CTX_free(ctx); }); //guarantee mCtx pointer always will be released
            EVP_DigestInit(ctx, md);
            std::vector<uint8_t> hash = Hmac(salt, inputKeyMaterial, md);
            return hash;
        }

    private:
        /**
         * mDigest must be constructed before mPseudoRandomKey
         * because mPseudoRandomKey constructing depends on the mDigest
         */
        const EVP_MD *mDigest;
        std::vector<unsigned char> mPseudoRandomKey;
    };
}


