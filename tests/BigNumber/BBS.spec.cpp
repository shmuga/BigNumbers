#include "gtest/gtest.h"
#include "BigNumber/BBS.h"

using namespace std;

TEST(BBSTest, POSITIVE) {

    string t;
    t = bbs(16);
    cout << "\n\n" << t << "\n";
    unsigned long num;
    num = stoi(t, nullptr, 2);
    cout << "\n\n" << num << "\n";
    EXPECT_EQ(true, true);

}
//
// Created by Mark Orel on 6/1/16.
//

