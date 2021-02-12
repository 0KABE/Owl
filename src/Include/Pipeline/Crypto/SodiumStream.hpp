#pragma once

#include "Stream.hpp"

namespace Owl {
    using StreamFun = int(unsigned char *c,
                          const unsigned char *m,
                          unsigned long long mlen,
                          const unsigned char *n,
                          uint32_t ic,
                          const unsigned char *k);

    template<size_t keySize, size_t ivSize, StreamFun streamFun>
    class SodiumStream : public Stream<keySize, ivSize> {
    public:
        using Stream = Stream<keySize, ivSize>;
        using typename Stream::IV;
        using typename Stream::Key;

        explicit SodiumStream(const std::string &password) : Stream(password) {}

    protected:
        size_t EncryptOnce(Buffer &in, Buffer &out) override {
            try {
                return Update(in, out);
            } catch (std::runtime_error &error) {
                throw std::runtime_error("Encrypt data failed");
            }
        }

        size_t DecryptOnce(Buffer &in, Buffer &out) override {
            try {
                return Update(in, out);
            } catch (std::runtime_error &error) {
                throw std::runtime_error("Decrypt data failed");
            }
        }

    private:
        size_t Update(Buffer &input, Buffer &output) {
            size_t padding = counter % 64;
            size_t n = std::min(64 - counter % 64, input.size());
            uint8_t b[64] = {};

            // align
            std::copy_n(static_cast<const uint8_t *> (std::data(input.data())), n, b + padding);
            if (!~streamFun(b,
                            b,
                            sizeof(b),
                            std::data(this->mIv),
                            counter / 64,
                            std::data(this->mKey)))
                throw std::runtime_error("Update data failed");
            auto buff = output.prepare(n);
            std::copy_n(b + padding, n, static_cast<uint8_t *>(std::data(buff)));
            counter += n;
            output.commit(n);
            input.consume(n);

            // update remaining
            size_t remaining = input.size();
            buff = output.prepare(remaining);
            if (!~streamFun(static_cast<uint8_t *>(std::data(buff)),
                            static_cast<const uint8_t *>(std::data(input.data())),
                            remaining,
                            std::data(this->mIv),
                            counter / 64,
                            std::data(this->mKey)))
                throw std::runtime_error("Update data failed");
            counter += remaining;
            output.commit(remaining);
            input.consume(remaining);
            return n + remaining;
        }

        size_t counter = 0;
    };
}
