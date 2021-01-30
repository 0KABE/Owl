#include <Configuration/ConfigurationGrammar.hpp>
#include "Configuration/ConfigurationParser.hpp"
#include <fstream>
#include <spdlog/spdlog.h>

Owl::ConfigurationParser::ConfigurationParser(std::string configPath) : mConfigPath(std::move(configPath)) {}

Owl::ConfInfo Owl::ConfigurationParser::Parse() {
    using Iterator = boost::u8_to_u32_iterator<std::string::const_iterator>;

    std::ifstream is(mConfigPath);
    const std::string data((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
    Iterator begin = data.begin(), end = data.end();

    ConfInfo confInfo;
    ConfigurationGrammar<Iterator, qi::blank_type> grammar;

    bool r = qi::phrase_parse(begin, end, grammar, qi::blank, confInfo);

    if (r && begin == end) {
        spdlog::info("Parse config {} success", mConfigPath);
    } else {
        auto it = begin;
        for (int i = 0; i < 30 && it != end; ++i, ++it);
        std::string context(begin, it);
        spdlog::error("Parse config {} failed, stopped at \"{}\"", mConfigPath, context);
    }
    return confInfo;
}
