#include <gtest/gtest.h>
#include "Util/FinalAction.hpp"

TEST(FinalAction, Operator) {
    using namespace Owl;
    bool a = false;
    {
        FinalAction finalAction;
        finalAction += [&] { a = true; };
    }
    ASSERT_EQ(a, true);
}