#pragma once

#include "Pipeline.hpp"

namespace Owl {
    class Pipelines : public Pipeline {
    public:
        using PipelinePtrCollection = std::vector<PipelinePtr>;

        Buffer &Wrap(Buffer &buffer) override;

        Buffer &UnWrap(Buffer &buffer) override;

        void PushBackPipeline(PipelinePtr pipelinePtr);

    private:
        PipelinePtrCollection mPipelinePtrCollection;
    };
}


