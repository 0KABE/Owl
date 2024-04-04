#include <gtest/gtest.h>
#include <regex>
#include "Util/HttpClient.hpp"
#include <spdlog/spdlog.h>
#include "Proxy/ProxyNodeManager.hpp"

using namespace Owl;

Awaitable<void> DirectHelper(std::string &data, const std::string &url, const ProxyNode::ProxyPtr &proxyNode) {
    boost::beast::http::empty_body::value_type body;

    auto executor = co_await net::this_coro::executor;

    try {
        HttpClient httpClient(executor, Url::Parse(url), proxyNode);
        httpClient.BuildRequest(body);
        const HttpClient<>::Response &response = co_await httpClient.Run();

        data = boost::beast::buffers_to_string(response.body().data());
    } catch (std::exception &exception) {
        spdlog::debug(exception.what());
    }
}

TEST(HttpClient, Direct) {
    net::io_context ioContext;
    std::string url = "http://info.cern.ch";
    std::string message;
    ProxyNode::ProxyPtr proxyNode = ProxyNodeManager::GetInstance().GetBuiltInProxyNode("DIRECT");

    net::co_spawn(ioContext.get_executor(), [&] { return DirectHelper(message, url, proxyNode); }, net::detached);
    ioContext.run();

    ASSERT_NE(message.find("<html>"), message.npos);
    ASSERT_NE(message.find("</html>"), message.npos);
}
