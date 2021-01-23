#include <gtest/gtest.h>
#include "Util/FinalAction.hpp"

TEST(FinalAction, Operator) {
    using namespace Owl;
    FinalAction finalAction;

    finalAction += [] { bool a = false; };
}