#pragma once

#include "Pipeline.hpp"

namespace Owl {
    class SimpleObfsClient : public Pipeline {
    public:
        using SimpleObfsClientPtr = std::unique_ptr<SimpleObfsClient>;

        explicit SimpleObfsClient(std::string hostname);

        Buffer &Wrap(Buffer &buffer) final;

        Buffer &UnWrap(Buffer &buffer) final;

        virtual Buffer &Obfuscate(Buffer &buffer) = 0;

        virtual Buffer &DeObfuscate(Buffer &buffer) = 0;

    protected:
        std::string mHostname;
        bool mHandleOBFS = true;
        bool mHandleDeOBFS = true;
    };

    using SimpleObfsClientPtr = SimpleObfsClient::SimpleObfsClientPtr;
}


