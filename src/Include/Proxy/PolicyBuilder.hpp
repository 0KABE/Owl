#pragma once

#include "ProxyGroup.hpp"
#include "Util/Singleton.hpp"

namespace Owl {
    class PolicyBuilder {
    public:
        template<typename T>
        struct Register {
            explicit Register(const std::string &type) {
                PolicyStore &policyStore = PolicyStore::GetInstance();

                if (policyStore.mGenerators.find(type) != policyStore.mGenerators.end()) {
                    throw std::invalid_argument(type + " has been registered");
                }

                policyStore.mGenerators[type] = [](std::string name,
                                                   std::vector<ProxyGroup::ProxyPtr> proxies,
                                                   std::chrono::seconds period,
                                                   std::string url) {
                    return std::make_shared<T>(name, proxies, period, url);
                };
            }
        };

        PolicyBuilder &SetName(std::string name);

        PolicyBuilder &AddProxy(const ProxyGroup::ProxyPtr &proxyPtr);

        PolicyBuilder &SetPeriod(std::chrono::seconds period);

        PolicyBuilder &SetUrl(std::string url);

        ProxyGroup::ProxyGroupPtr Build(const std::string &type);

    private:
        struct PolicyStore : public Singleton<PolicyStore> {
            friend Singleton<PolicyStore>;
            using PolicyGenerator = std::function<ProxyGroup::ProxyGroupPtr(std::string name,
                                                                            std::vector<ProxyGroup::ProxyPtr> proxies,
                                                                            std::chrono::seconds period,
                                                                            std::string url)>;

            std::unordered_map<std::string, PolicyGenerator> mGenerators;

        private:
            PolicyStore() = default;
        };

        std::string mName;
        std::vector<ProxyGroup::ProxyPtr> mProxies;
        std::chrono::seconds mPeriod;
        std::string mUrl;
        ProxyGroup::ProxyGroupPtr mProxyGroupPtr;
    };
}


