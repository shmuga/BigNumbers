//
// Created by Mark Orel on 6/3/16.
//

#include "gtest/gtest.h"
#include "BigNumber/md5.h"

using namespace std;

TEST(MD5, HASH) {
    cout <<  md5("hallo world") << endl;

}