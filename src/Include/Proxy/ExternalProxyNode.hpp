#pragma once

#include "ProxyNode.hpp"
#include "Pipeline.hpp"

namespace Owl {
    class ExternalProxyNode : public ProxyNode {
    public:
        using PipelineGenerator = std::function<Pipeline::PipelinePtr()>;
        using PipelineGenerators = std::vector<PipelineGenerator>;

        ExternalProxyNode(std::string name, std::string proxyHostname, std::string proxyPort,
                          std::vector<PipelineGenerator> pipelineGenerators);

        [[nodiscard]] Outbound::BoundPtr GetOutbound(Endpoint endpoint) const override;

    private:
        std::string mProxyHostname;
        std::string mProxyPort;
        PipelineGenerators mPipelineGenerators;
    };
}


