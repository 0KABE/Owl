#pragma once

#include "Util/Endpoint.hpp"
#include "Util/Net.hpp"
#include "Util/Singleton.hpp"
#include "Bound/Outbound.hpp"
#include "Configuration/ConfigurationParser.hpp"
#include "Proxy/ExternalProxyNode.hpp"
#include "Policy.hpp"

namespace Owl {
    class Configuration {
    public:
        inline static const std::u32string METHOD = U"encrypt-method";
        inline static const std::u32string PASSWORD = U"password";
        inline static const std::u32string OBFS = U"obfs";
        inline static const std::u32string OBFS_HOST = U"obfs-host";

        static void Load(const std::string &path, net::any_io_executor executor);

        static std::string GetConfigContent(const std::string &path);

        static Outbound::BoundPtr Match(Endpoint endpoint);

    private:
        static void Register(const ConfInfo &confInfo, net::any_io_executor executor);

        static void RegisterRule(const ConfInfo &confInfo);

        static void RegisterProxy(const ConfInfo &confInfo);

        static void RegisterPolicy(const ConfInfo &confInfo, net::any_io_executor executor);

        static ExternalProxyNode::PipelineGenerators BuildProxyPipelineGenerators(const ProxyInfo &proxyInfo);
    };
}


