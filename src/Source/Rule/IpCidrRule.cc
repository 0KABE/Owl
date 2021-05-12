#include "Rule/IpCidrRule.hpp"
#include "Rule/RuleFactory.hpp"


bool Owl::IpCidrRule::Match(const Owl::Endpoint &endpoint) {
    if (endpoint.GetHostnameType() != Endpoint::HOST_IP)
        return false;

    uint32_t checkIp =
            boost::asio::ip::address_v4::from_string(endpoint.GetHostname()).to_ulong();
    return (checkIp & mMask) == (mIp & mMask);
}

Owl::IpCidrRule::IpCidrRule(Owl::ProxyPtr proxyPtr, uint32_t ip, uint32_t mask)
        : Rule(std::move(proxyPtr)), mIp(ip), mMask(mask) {}

Owl::IpCidrRule::IpCidrRulePtr
Owl::IpCidrRule::Create(const Owl::ProxyPtr &proxyPtr, const std::string &rule) {
    std::string::size_type slashPos = rule.find('/');

    uint32_t ip = boost::asio::ip::address_v4::from_string(rule.substr(0, slashPos)).to_ulong();

    uint32_t netBits = std::stoi(rule.substr(slashPos + 1));
    uint32_t netMask = ~(~uint32_t(0) >> netBits);

    return std::make_unique<IpCidrRule>(proxyPtr, ip, netMask);
}

std::string Owl::IpCidrRule::RuleType() const {
    return "IP/CIDR";
}

std::string Owl::IpCidrRule::Condition() const {
    return fmt::format("{}/{}", boost::asio::ip::address_v4(mIp).to_string(), mMask);
}

static Owl::RuleFactory::Register<Owl::IpCidrRule> r("IP-CIDR");
