#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <unordered_set>

TEST(STD_Function, Pointer) {
    using FunctionPtr = std::shared_ptr<std::function<void()>>;

    FunctionPtr functionPtr1 = std::make_shared<std::function<void()>>([] { spdlog::debug("This is functionPtr1"); });
    FunctionPtr functionPtr2 = std::make_shared<std::function<void()>>([] { spdlog::debug("This is functionPtr2"); });

    std::set<FunctionPtr> set;

    set.insert(functionPtr1);
    ASSERT_NE(set.find(functionPtr1), set.end());
    ASSERT_EQ(set.find(functionPtr2), set.end());
    ASSERT_EQ(set.size(), 1);

    set.insert(functionPtr2);
    ASSERT_NE(set.find(functionPtr1), set.end());
    ASSERT_NE(set.find(functionPtr2), set.end());
    ASSERT_EQ(set.size(), 2);

    set.erase(functionPtr1);
    ASSERT_EQ(set.find(functionPtr1), set.end());
    ASSERT_NE(set.find(functionPtr2), set.end());
    ASSERT_EQ(set.size(), 1);

    set.erase(functionPtr2);
    ASSERT_EQ(set.find(functionPtr1), set.end());
    ASSERT_EQ(set.find(functionPtr2), set.end());
    ASSERT_EQ(set.size(), 0);
}