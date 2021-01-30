#pragma once

#include <vector>
#include <string>
#include <map>

namespace Owl {
    struct RuleInfo {
        std::u32string type;
        std::u32string value;
        std::u32string policy;
    };
    struct ConfInfo {
        std::vector<RuleInfo> rules;
    };

    class ConfigurationParser {
    public:
        explicit ConfigurationParser(std::string configPath);

        ConfInfo Parse();

    private:
        std::string mConfigPath;
    };
}


