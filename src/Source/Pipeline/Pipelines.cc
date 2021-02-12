#include "Pipeline/Pipelines.hpp"

Owl::Buffer &Owl::Pipelines::Wrap(Owl::Buffer &buffer) {
    std::reference_wrapper<Buffer> wrappedBuffer = buffer;
    for (auto item = mPipelinePtrCollection.rbegin(); item != mPipelinePtrCollection.rend(); ++item) {
        wrappedBuffer = (*item)->Wrap(wrappedBuffer);
    }
    return wrappedBuffer;
}

Owl::Buffer &Owl::Pipelines::UnWrap(Owl::Buffer &buffer) {
    std::reference_wrapper<Buffer> unwrappedBuffer = buffer;
    for (auto item = mPipelinePtrCollection.rbegin(); item != mPipelinePtrCollection.rend(); ++item) {
        unwrappedBuffer = (*item)->Wrap(unwrappedBuffer);
    }
    return unwrappedBuffer;
}

void Owl::Pipelines::PushBackPipeline(Owl::Pipeline::PipelinePtr pipelinePtr) {
    mPipelinePtrCollection.push_back(std::move(pipelinePtr));
}
