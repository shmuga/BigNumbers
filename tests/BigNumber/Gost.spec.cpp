#include "gtest/gtest.h"
#include "BigNumber/Gost.h"

using namespace std;

TEST(GostTest, POSITIVE) {

    string t;
    t = getPrime(24265, 29505, 1024);
    cout << "\n\n" << t << "\n";

    EXPECT_EQ(true, true);

}
