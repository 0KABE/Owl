#include <gtest/gtest.h>
#include "Configuration/ConfigurationParser.hpp"


TEST(ConfigurationParser, Proxy) {
    using namespace Owl;

    Owl::ConfigurationParser configurationParser("../../test/Source/ConfigurationParser-Proxy.txt");

    ConfInfo confInfo = configurationParser.Parse();

    ASSERT_EQ(confInfo.proxies[0].name, U"Proxy  A");
    ASSERT_EQ(confInfo.proxies[0].protocol, U"ss");
    ASSERT_EQ(confInfo.proxies[0].server, U"serverA.com");
    ASSERT_EQ(confInfo.proxies[0].port, 443);
    ASSERT_EQ(confInfo.proxies[0].properties[U"interval"], U"100");

    ASSERT_EQ(confInfo.proxies[1].name, U"ProxyB");
    ASSERT_EQ(confInfo.proxies[1].protocol, U"ss");
    ASSERT_EQ(confInfo.proxies[1].server, U"serverB.com");
    ASSERT_EQ(confInfo.proxies[1].port, 443);
    ASSERT_EQ(confInfo.proxies[1].properties[U"interval"], U"100");
}

TEST(ConfigurationParser, Rule) {
    using namespace Owl;

    Owl::ConfigurationParser configurationParser("../../test/Source/ConfigurationParser-Rule.txt");

    ConfInfo confInfo = configurationParser.Parse();

    ASSERT_EQ(confInfo.rules[0].type, U"DOMAIN-SUFFIX");
    ASSERT_EQ(confInfo.rules[0].value, U"company.com");
    ASSERT_EQ(confInfo.rules[0].policy, U"ProxyA");

    ASSERT_EQ(confInfo.rules[1].type, U"DOMAIN-KEYWORD");
    ASSERT_EQ(confInfo.rules[1].value, U"google");
    ASSERT_EQ(confInfo.rules[1].policy, U"DIRECT");

    ASSERT_EQ(confInfo.rules[2].type, U"GEOIP");
    ASSERT_EQ(confInfo.rules[2].value, U"US");
    ASSERT_EQ(confInfo.rules[2].policy, U"DIRECT");

    ASSERT_EQ(confInfo.rules[3].type, U"IP-CIDR");
    ASSERT_EQ(confInfo.rules[3].value, U"192.168.0.0/16");
    ASSERT_EQ(confInfo.rules[3].policy, U"DIRECT");

    ASSERT_EQ(confInfo.rules[4].type, U"FINAL");
    ASSERT_EQ(confInfo.rules[4].value, U"");
    ASSERT_EQ(confInfo.rules[4].policy, U"Proxy   B");
}
