#include "Util/Endpoint.hpp"
#include <gtest/gtest.h>

TEST(Endpoint, All_Success) {
    using namespace Owl;
    net::io_context ioContext;
    Endpoint endpoint("baidu.com", "80");

    for (int i = 0; i < 10; ++i) {
        net::co_spawn(ioContext.get_executor(),
                      [&] { return endpoint.Connect(std::chrono::milliseconds(100)); }, net::detached);
    }
    ioContext.run();
    ASSERT_EQ(endpoint.GetState(), Endpoint::ConnectionState::CONNECTED);
}

TEST(Endpoint, All_Failed) {
    using namespace Owl;
    net::io_context ioContext;
    Endpoint endpoint("aaa.com", "80");

    for (int i = 0; i < 10; ++i) {
        net::co_spawn(ioContext.get_executor(), [&] { return endpoint.Connect(std::chrono::milliseconds(100)); },
                      net::detached);
    }
    ioContext.run();
    ASSERT_EQ(endpoint.GetState(), Endpoint::ConnectionState::FAIL_TO_CONNECT);
}

TEST(Endpoint, Timeout) {
    using namespace Owl;
    net::io_context ioContext;
    Endpoint endpoint("baidu.com", "80");

    for (int i = 0; i < 10; ++i) {
        net::co_spawn(ioContext.get_executor(), [&] { return endpoint.Connect(std::chrono::milliseconds(0)); },
                      net::detached);
    }
    ioContext.run();
    ASSERT_EQ(endpoint.GetState(), Endpoint::ConnectionState::FAIL_TO_CONNECT);
}

TEST(Endpoint, One_By_One) {
    using namespace Owl;
    net::io_context ioContext;
    Endpoint endpoint("baidu.com", "80");

    net::co_spawn(ioContext.get_executor(), [&] { return endpoint.Connect(std::chrono::milliseconds(100)); },
                  net::detached);
    ioContext.run();
    ASSERT_EQ(endpoint.GetState(), Endpoint::ConnectionState::CONNECTED);
    ioContext.reset();

    net::co_spawn(ioContext.get_executor(), [&] { return endpoint.Connect(std::chrono::milliseconds(100)); },
                  net::detached);
    ioContext.run();
    ASSERT_EQ(endpoint.GetState(), Endpoint::ConnectionState::CONNECTED);
}
