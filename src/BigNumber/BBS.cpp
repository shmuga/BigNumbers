//
// Created by Mark Orel on 6/1/16.
//

#include "BBS.h"

string bbs(int N) {
    mpz_t A, B, r, n, tmp;
    mpz_init(tmp);
    mpz_init(n);
    mpz_init(r);
    mpz_init(A);
    mpz_init(B);

    mpz_set_str(A, "32372334488362947019304797379371153325410700999", 10);
    mpz_set_str(B, "541528607341564832259551", 10);
    mpz_mul(n, A, B);


    mpz_set_ui(r, rand() % 100000 + 1);
    string res;
    while (N) {
        mpz_powm_ui(r, r, 2, n);
        mpz_mod_ui(tmp, r, 2);
        res += to_string(mpz_get_ui(tmp));
        N--;
    }
    return res;
}