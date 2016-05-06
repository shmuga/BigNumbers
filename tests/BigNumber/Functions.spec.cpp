//
// Created by axel on 5/7/16.
//

#include "gtest/gtest.h"
#include "BigNumber/BigNumber.h"
#include "BigNumber/Functions.h"

TEST(FunctionsAddition, POSITIVE) {
    BigNumber a("2");
    BigNumber b("2");
    BigNumber c = addition(a, b);
    EXPECT_EQ(c.print(), "444");
}

TEST(FunctionsAddition, NEGATIVE) {
//    EXPECT_NE("123123", BigNumber("0").print());
}
