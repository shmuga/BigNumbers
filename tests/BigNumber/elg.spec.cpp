//
// Created by Mark Orel on 6/3/16.
//

#include "gtest/gtest.h"
#include "BigNumber/elg.h"

using namespace std;

TEST(ELGTest, POSITIVE) {
    int M = 2400;

    pair<string, string> p = generateP();

    string x = generateX(p.first);
    string g = generateG(p.first, p.second);
    string y = generateY(p.first, g, x);

    pair<string, string> C = ELGcrypt(M, p.first, g, y, x);

//    cout << p.first << endl;
//    cout << p.second << endl;
//    cout << x << endl;
//    cout << y << endl;
//    cout << g << endl;

    cout << C.first << endl;
    cout << C.second << endl;
}
//
// Created by Mark Orel on 6/1/16.
//

