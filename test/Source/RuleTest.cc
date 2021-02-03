#include <gtest/gtest.h>
#include "RuleFactory.hpp"
#include "Util/Endpoint.hpp"

using namespace Owl;

TEST(Rule, DomainKeyword) {
    Endpoint endpoint1("www.google.com", "443");
    Endpoint endpoint2("www.baidu.com", "443");

    Rule::RulePtr rule = RuleFactory::GetInstance()
            .Create("DOMAIN-KEYWORD", nullptr, "google.com");

    ASSERT_TRUE(rule->Match(endpoint1));
    ASSERT_FALSE(rule->Match(endpoint2));
}

TEST(Rule, DomainSuffix) {
    Endpoint endpoint1("www.google.com", "443");
    Endpoint endpoint2("www.baidu.com", "443");

    Rule::RulePtr rule = RuleFactory::GetInstance()
            .Create("DOMAIN-SUFFIX", nullptr, "google.com");

    ASSERT_TRUE(rule->Match(endpoint1));
    ASSERT_FALSE(rule->Match(endpoint2));
}

TEST(Rule, IpCidr) {
    Endpoint endpoint1("www.google.com", "443");
    Endpoint endpoint2("www.baidu.com", "443");
    Endpoint endpoint3("192.168.1.255", "443");
    Endpoint endpoint4("192.168.2.255", "443");

    Rule::RulePtr rule = RuleFactory::GetInstance()
            .Create("IP-CIDR", nullptr, "192.168.1.0/24");

    ASSERT_FALSE(rule->Match(endpoint1));
    ASSERT_FALSE(rule->Match(endpoint2));
    ASSERT_TRUE(rule->Match(endpoint3));
    ASSERT_FALSE(rule->Match(endpoint4));
}

TEST(Rule, Final) {
    Endpoint endpoint1("www.google.com", "443");
    Endpoint endpoint2("www.baidu.com", "443");

    Rule::RulePtr rule = RuleFactory::GetInstance()
            .Create("FINAL", nullptr, "");

    ASSERT_TRUE(rule->Match(endpoint1));
    ASSERT_TRUE(rule->Match(endpoint2));
}
