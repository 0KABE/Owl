#pragma once

#include <deque>
#include "Rule.hpp"
#include "Util/Singleton.hpp"

namespace Owl {
    class RuleManager : public Singleton<RuleManager> {
        friend Singleton<RuleManager>;

    public:
        using RuleCollection = std::deque<RulePtr>;

        void AddRule(RulePtr rulePtr);

        [[nodiscard]] ProxyPtr Match(const Endpoint &endpoint) const;

    private:
        RuleManager();

        RuleCollection mRuleCollection;
    };
}


