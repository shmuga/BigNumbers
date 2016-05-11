//
// Created by axel on 5/7/16.
//

#include "gtest/gtest.h"
#include "BigNumber/BigNumber.h"
#include "BigNumber/Functions.h"
#include <string>
using namespace std;
#include "gmp.h"


void gen_big_number(mpz_t num) {
    mpz_init(num);
    unsigned long int i, seed;
    gmp_randstate_t r_state;
    seed = rand() % 10000;
    gmp_randinit_default (r_state);
    gmp_randseed_ui(r_state, seed);
    mpz_urandomb(num, r_state, 264);
}


TEST(FunctionsAddition, POSITIVE) {
    mpz_t a, b, res;
    mpz_init(res);
    gen_big_number(a);
    gen_big_number(b);
    mpz_add(res, a, b);
    char *aSt, *bSt, *resSt;
    mpz_get_str(aSt, 10, a);
    mpz_get_str(bSt, 10, b);
    mpz_get_str(resSt, 10, res);
    string aStr(aSt), bStr(bSt), resStr(resSt);
    EXPECT_EQ(resStr, sum(BigNumber(aStr), BigNumber(bStr)));
}

TEST(FunctionsAddition, NEGATIVE) {

}


