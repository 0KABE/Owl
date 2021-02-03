#pragma once

#include "Rule.hpp"

namespace Owl {
    class RuleFactory {
    public:
        using GenerateFunc = std::function<Rule::RulePtr(Proxy::ProxyPtr proxyPtr, std::string rule)>;

        static RuleFactory &GetInstance();

        Rule::RulePtr Create(const std::string& type, Proxy::ProxyPtr proxyPtr, std::string rule);

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


