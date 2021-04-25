#pragma once

#include <vector>
#include <string>
#include <map>

namespace Owl {
    struct ProxyInfo {
        std::u32string name;
        std::u32string protocol;
        std::u32string server;
        int port;
        std::map<std::u32string, std::u32string> properties;
    };
    struct PolicyInfo {
        std::u32string name;
        std::u32string type;
        std::vector<std::u32string> proxies;
        std::map<std::u32string, std::u32string> properties;
    };
    struct RuleInfo {
        std::u32string type;
        std::u32string value;
        std::u32string policy;
    };
    struct ConfInfo {
        std::vector<ProxyInfo> proxies;
        std::vector<PolicyInfo> policies;
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


