#pragma once

#include "Util/Buffer.hpp"

namespace Owl {
    class Pipeline {
    public:
        using PipelinePtr = std::unique_ptr<Pipeline>;

        virtual  ~Pipeline() = default;

        virtual Buffer &Wrap(Buffer &buffer) = 0;

        virtual Buffer &UnWrap(Buffer &buffer) = 0;
    };
}


