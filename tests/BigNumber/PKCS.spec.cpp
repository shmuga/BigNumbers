//
// Created by Mark Orel on 6/3/16.
//

#include "gtest/gtest.h"
#include "BigNumber/PKCS.h"

using namespace std;

TEST(PKCS, CRYPT) {
    pair<PublicKey, PrivateKey> p = InitCrypto::init();
    string M = "I'll be back.";
    pair<string, string> C = Encrypt::start(M, p.first, p.second);
    EXPECT_EQ(M, Decrypt::start(C, p.first, p.second));
}//
// Created by Mark Orel on 6/16/16.
//

