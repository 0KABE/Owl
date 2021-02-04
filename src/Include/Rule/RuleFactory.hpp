#pragma once

#include "Rule.hpp"
#include "Util/Singleton.hpp"

namespace Owl {
    class RuleFactory : public Singleton<RuleFactory> {
        friend Singleton<RuleFactory>;
    public:
        using GenerateFunc = std::function<Rule::RulePtr(Proxy::ProxyPtr proxyPtr, std::string rule)>;

        Rule::RulePtr Create(const std::string &type, Proxy::ProxyPtr proxyPtr, std::string rule);

        template<typename T>
        struct Register {
            explicit Register(const std::string &type) {
                RuleFactory &ruleFactory = GetInstance();
                if (ruleFactory.mRulesGenerators.find(type) != ruleFactory.mRulesGenerators.end())
                    throw std::invalid_argument(type + " has been registered");
                ruleFactory.mRulesGenerators[type] =
                        [](Proxy::ProxyPtr proxyPtr, std::string rule) { return T::Create(proxyPtr, rule); };
            }
        };

    private:
        RuleFactory() = default;

        std::unordered_map<std::string, GenerateFunc> mRulesGenerators;
    };
}


