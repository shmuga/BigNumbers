//
// Created by Mark Orel on 6/3/16.
//

#include "gtest/gtest.h"
#include "BigNumber/elg.h"

using namespace std;

TEST(ELGTest, ENCRYPT_DECRYPT) {
    string M = "Hello crypto world";
    string D;
    pair<string, string> p = generateP();

    string x = generateX(p.first);
    string g = generateG(p.first, p.second);
    string y = generateY(p.first, g, x);

    pair<string, string> C = ELGcrypt(M, p.first, g, y, x);
    D = ELGdecrypt(C.second, C.first, x, p.first);
    cout << M << endl;
    cout << D << endl;
    EXPECT_EQ(M, D);
}



TEST(ELGTest, SUBSCRIBE_CHECK) {
    string M = "Hello crypto world";
    pair<string, string> p = generateP();

    string x = generateX(p.first);
    string g = generateG(p.first, p.second);
    string y = generateY(p.first, g, x);

    pair<string, string> S = ELGsubscribe(M, p, g, x);

    string r = S.first;
    string s = S.second;

    EXPECT_TRUE(ELGcheck(M, p.first, g, y, r, s));
}

