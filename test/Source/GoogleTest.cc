#include <gtest/gtest.h>

TEST(GoogleTest, Bool) {
    ASSERT_EQ(true, true);
}

TEST(GoogleTest, Int) {
    ASSERT_EQ(0, 0);
}

TEST(GoogleTest, String) {
    ASSERT_EQ("This is a liveness test", "This is a liveness test");
}

TEST(GoogleTest, Double) {
    ASSERT_EQ(0.0, 0.0);
}