//
// Created by axel on 5/7/16.
//

#include "gtest/gtest.h"
#include "BigNumber/BigNumber.h"
#include "BigNumber/Functions.h"
#include <cstdlib>
#include <string>
using namespace std;
#include "gmp.h"
#include "chrono"


void gen_big_number(mpz_t num, int digits = 256) {
    mpz_init(num);
    unsigned int seed;
    gmp_randstate_t r_state;
    seed = rand() % 1000000;
    srand(seed);
    gmp_randinit_mt (r_state);
    gmp_randseed_ui(r_state, seed);
    mpz_urandomb(num, r_state, digits);
    gmp_randclear(r_state);
}


TEST(FunctionsAddition, POSITIVE) {
    mpz_t a, b, res;
    mpz_init(res);
    gen_big_number(a, 1000000);
    gen_big_number(b, 1000000);
    mpz_add(res, a, b);

    char * aSt = mpz_get_str(NULL,10,a);
    char * bSt = mpz_get_str(NULL,10,b);
    char * resSt = mpz_get_str(NULL,10,res);
    string aStr(aSt), bStr(bSt), resStr(resSt);
    string sumSt;
    sumSt = sum(BigNumber(aStr), BigNumber(bStr)).print();

    EXPECT_EQ(resStr, sumSt);
}

TEST(FunctionsAddition, NEGATIVE) {

}


