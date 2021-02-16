#pragma once

#include "Pipeline/SimpleObfsClient.hpp"
#include <random>
#include <string>

namespace Owl {
    class HttpObfuscator : public SimpleObfsClient {
    public:
        using SimpleObfsClient::SimpleObfsClient;

        Buffer &Obfuscate(Buffer &buffer) override;

        Buffer &DeObfuscate(Buffer &buffer) override;

    private:
        template<typename T>
        static void RandomBase64(T &buffer) {
            static const char b64Chars[] =
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
            static std::uniform_int_distribution<> u{0, (sizeof b64Chars) - 2};

            auto last = std::generate_n(std::begin(buffer), std::size(buffer) - 3,
                                        [&] { return b64Chars[u(mRandomEngine)]; });
            if (mRandomEngine() % 2) {
                *last++ = '=';
                *last++ = '=';
            } else {
                *last++ = b64Chars[u(mRandomEngine)];
                if (mRandomEngine() % 2) {
                    *last++ = '=';
                } else {
                    *last++ = b64Chars[u(mRandomEngine)];
                }
            }
            *last++ = '\0';
        }

        template<typename T>
        static Buffer &Prepend(Buffer &buffer, T prependData) {
            std::vector<uint8_t> data;

            // concatenate buffer and prependData
            data.reserve(prependData.size() + buffer.size());
            std::copy(std::begin(prependData), std::end(prependData), std::back_inserter(data));
            std::copy_n(static_cast<const uint8_t *>(std::data(buffer.data())), buffer.size(),
                        std::back_inserter(data));
            buffer.consume(buffer.size());

            // copy back to buffer
            std::copy(std::begin(data), std::end(data),
                      static_cast<uint8_t *>(buffer.prepare(data.size()).data()));
            buffer.commit(data.size());

            return buffer;
        }

        inline static std::default_random_engine mRandomEngine{std::random_device{}()};
        Buffer mDeObfuscateBuffer;
    };
}


