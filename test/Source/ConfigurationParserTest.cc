#include <gtest/gtest.h>
#include "Configuration/ConfigurationParser.hpp"


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