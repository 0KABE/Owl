#pragma once

#include "Pipeline/Crypto.hpp"
#include "Util/Concepts.hpp"

namespace Owl {
    template<typename T> requires Derived<Crypto, T>
    class Cipher : public Crypto {
    public:
        explicit Cipher(const std::string &password) : mEncryptor(password), mDecryptor(password) {}

        Buffer &Encrypt(Buffer &input) override {
            return mEncryptor.Encrypt(input);
        }

        Buffer &Decrypt(Buffer &input) override {
            return mDecryptor.Encrypt(input);
        }

    private:
        T mEncryptor;
        T mDecryptor;
    };
}


