#pragma once

#include <openssl/hmac.h>
#include <sodium.h>
#include <string>
#include "TypeTraits.hpp"
#include "Util/FinalAction.hpp"

namespace Owl {
    template<typename T>
    requires IsArrayOrVector<T>
    static void RandomFill(T &data) {
        randombytes_buf(std::data(data), std::size(data));
    }

    template<typename T>
    requires IsArrayOrVector<T>
    static void CipherToKey(const std::string &cipher, T &key) {
        const EVP_MD *md = EVP_md5();
        EVP_MD_CTX *ctx = EVP_MD_CTX_new();
        FinalAction finalAction([&] { EVP_MD_CTX_free(ctx); });
        uint8_t mdBuff[EVP_MAX_MD_SIZE];
        int addMd;
        uint32_t i, j;
        uint32_t mds = EVP_MD_size(md);

        for (j = 0, addMd = 0; j < std::size(key); ++addMd) {
            EVP_DigestInit(ctx, md);
            if (addMd)
                EVP_DigestUpdate(ctx, mdBuff, mds);

            EVP_DigestUpdate(ctx, std::data(cipher), std::size(cipher));
            EVP_DigestFinal(ctx, mdBuff, &mds);

            for (i = 0; i < mds && j < std::size(key); ++i, ++j)
                key[j] = mdBuff[i];
        }
    }
}
