#pragma once

#include "Pipeline.hpp"

namespace Owl {
    class Crypto : public Pipeline {
    public:
        Buffer &Wrap(Buffer &buffer) final;

        Buffer &UnWrap(Buffer &buffer) final;

        virtual Buffer &Encrypt(Buffer &input) = 0;

        virtual Buffer &Decrypt(Buffer &input) = 0;
    };
}


