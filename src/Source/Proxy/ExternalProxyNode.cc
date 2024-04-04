#include "Proxy/ExternalProxyNode.hpp"
#include "Pipeline/Pipelines.hpp"
#include "Bound/BoundFactory.hpp"

Owl::Outbound::BoundPtr Owl::ExternalProxyNode::GetOutbound(Owl::Endpoint endpoint) const {
    net::any_io_executor executor = endpoint.GetSocket().get_executor();
    Pipelines::PipelinesPtr pipelinesPtr = std::make_unique<Pipelines>();
    std::for_each(mPipelineGenerators.begin(), mPipelineGenerators.end(),
                  [&](const PipelineGenerator &pipelineGenerator) {
                      pipelinesPtr->PushBackPipeline(pipelineGenerator());
                  });
    return BoundFactory::CreateProxy(std::move(pipelinesPtr),
                                     std::move(endpoint),
                                     {executor, mProxyHostname, mProxyPort});
}

Owl::ExternalProxyNode::ExternalProxyNode(std::string name,
                                          std::string proxyHostname,
                                          std::string proxyPort,
                                          std::vector<PipelineGenerator> pipelineGenerators)
        : ProxyNode(std::move(name)),
          mProxyHostname(std::move(proxyHostname)),
          mProxyPort(std::move(proxyPort)),
          mPipelineGenerators(std::move(pipelineGenerators)) {}
