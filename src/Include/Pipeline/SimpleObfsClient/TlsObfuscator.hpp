#pragma once

#include "Pipeline/SimpleObfsClient.hpp"

namespace Owl {


    class TlsObfuscator : public SimpleObfsClient {
    public:
        using SimpleObfsClient::SimpleObfsClient;

        struct Frame {
            size_t len;
            Buffer buff;
        };

        enum DeObfsStatus {
            RECEIVE_HELLO, RECEIVE_FRAME_LEN, RECEIVE_FRAME
        };

        Buffer &Obfuscate(Buffer &buffer) override;

        Buffer &DeObfuscate(Buffer &buffer) override;

    private:
        struct Header {
            uint8_t dataHeader[3] = {0x17, 0x03, 0x03};
            union {
                uint8_t uInt8[2];
                uint16_t uInt16;
            } length = {};
        };

        static void ObfsAppData(Buffer &buffer);

        static void RandBytes(uint8_t *buf, size_t len);

        Frame frame;
        DeObfsStatus deObfsStatus = RECEIVE_HELLO;
    };
}


