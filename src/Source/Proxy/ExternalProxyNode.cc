#include "Proxy/ExternalProxyNode.hpp"
#include "Pipeline/Pipelines.hpp"
#include "Bound/BoundFactory.hpp"

Owl::Outbound::BoundPtr Owl::ExternalProxyNode::GetOutbound(Owl::Endpoint endpoint) const {
    Pipelines::PipelinesPtr pipelinesPtr = std::make_unique<Pipelines>();
    std::for_each(mPipelineGenerators.begin(), mPipelineGenerators.end(),
                  [&](const PipelineGenerator &pipelineGenerator) {
                      pipelinesPtr->PushBackPipeline(pipelineGenerator());
                  });
    return BoundFactory::CreateProxy(std::move(pipelinesPtr),
                                     std::move(endpoint),
                                     {mProxyHostname, mProxyPort});
}

Owl::ExternalProxyNode::ExternalProxyNode(std::string name,
                                          std::string proxyHostname,
                                          std::string proxyPort,
                                          std::vector<PipelineGenerator> pipelineGenerators)
        : ProxyNode(std::move(name)),
          mProxyHostname(std::move(proxyHostname)),
          mProxyPort(std::move(proxyPort)),
          mPipelineGenerators(std::move(pipelineGenerators)) {}
