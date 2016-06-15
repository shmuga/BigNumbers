#include "gtest/gtest.h"
#include "BigNumber/Gost.h"
#include "BigNumber/BBS.h"
using namespace std;

TEST(GostTest, POSITIVE) {

    srand (time(NULL));
    int a = stoi(bbs(16), nullptr, 2);
    srand (a);
    int b = stoi(bbs(16), nullptr, 2);

    pair<string, string> t = getPrime(a, b, 1024);

    mpz_t pr;
    mpz_init(pr);
    mpz_set_str(pr, t.first.c_str(), 10);
    int ifPrime = mpz_probab_prime_p(pr, 1000);
    cout << "Number=" << t.first << endl;
    cout << "ifPrime=" << ifPrime << endl;
    EXPECT_EQ(ifPrime != 0, true);

}
