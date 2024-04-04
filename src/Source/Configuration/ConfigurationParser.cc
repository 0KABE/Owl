#include <Configuration/ConfigurationGrammar.hpp>
#include "Configuration/ConfigurationParser.hpp"
#include <fstream>
#include <spdlog/spdlog.h>
#include <boost/regex/pending/unicode_iterator.hpp>

Owl::ConfigurationParser::ConfigurationParser(std::string configPath) : mConfigPath(std::move(configPath)) {}

Owl::ConfInfo Owl::ConfigurationParser::Parse() {
    spdlog::info("Start in parse config: {}", mConfigPath);
    std::ifstream is(mConfigPath);
    return Parse(is);
}

Owl::ConfInfo Owl::ConfigurationParser::Parse(std::istream &istream) {
    using Iterator = boost::u8_to_u32_iterator<std::string::const_iterator>;
    const std::string data((std::istreambuf_iterator<char>(istream)), std::istreambuf_iterator<char>());
    Iterator begin = data.begin(), end = data.end();

    ConfInfo confInfo;
    ConfigurationGrammar<Iterator, qi::blank_type> grammar;

    bool r = qi::phrase_parse(begin, end, grammar, qi::blank, confInfo);

    if (r && begin == end) {
        spdlog::info("Parse config success");
    } else {
        auto it = begin;
        for (int i = 0; i < 30 && it != end; ++i, ++it);
        std::string context(begin, it);
        spdlog::error("Parse config failed, stopped at \"{}\"", context);
    }
    return confInfo;
}
