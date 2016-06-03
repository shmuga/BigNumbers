//
// Created by Mark Orel on 6/3/16.
//

#include "elg.h"
#include "Gost.h"
#include "BBS.h"

pair<string, string> generateP() {
    srand (time(NULL));
    int a = stoi(bbs(16), nullptr, 2);
    srand (a);
    int b = stoi(bbs(16), nullptr, 2);

    return getPrime(a, b, 1024);
}

string generateX(string p) {
    mpz_t num;
    mpz_init(num);
    mpz_set_str(num, p.c_str(), 10);

    gmp_randstate_t r_state;

    srand(time(NULL));
    int seed = rand();

    gmp_randinit_default (r_state);
    gmp_randseed_ui(r_state, seed);

    string res;
    mpz_urandomm(num, r_state, num);
    res = mpz_get_str(NULL, 10, num);
    return res;
}

string generateG(string pS, string qS) {
    int i = 1;

    bool found = false;
    mpz_t p, p1, q, r, g, tmp;
    mpz_init(p);
    mpz_init(tmp);
    mpz_init(p1);
    mpz_init(g);
    mpz_init(q);
    mpz_init(r);
    mpz_set_str(p, pS.c_str(), 10);
    mpz_sub_ui(p1, p, 1);

    mpz_set_str(q, qS.c_str(), 10);
    mpz_div(r, p1, q);

    while (!found) {
        i++;
        mpz_set_ui(g, i);
        mpz_powm(tmp, g, q, p);
        if (mpz_cmp_ui(tmp, 1) == 0) continue;
        mpz_powm_sec(tmp, g, r, p);
        if (mpz_cmp_ui(tmp, 1) == 0) continue;
        break;
    }
    return mpz_get_str(NULL, 10, g);
}

string generateY(string p, string g, string x) {
    mpz_t tmp, gg, xx, pp;
    mpz_init(tmp);
    mpz_init(gg);
    mpz_init(xx);
    mpz_init(pp);
    mpz_set_str(pp, p.c_str(), 10);
    mpz_set_str(gg, g.c_str(), 10);
    mpz_set_str(xx, x.c_str(), 10);

    mpz_powm(tmp, gg, xx, pp);
    return mpz_get_str(NULL, 10, tmp);
}


pair<string, string> ELGcrypt(int M, string pS, string gS, string yS, string xS) {
    mpz_t a, b;
    mpz_t p, g, y, x, k;
    mpz_set_str(k, generateX(pS).c_str(), 10);
    mpz_init(k);
    mpz_init(a);
    mpz_init(b);
    mpz_init(p);
    mpz_init(g);
    mpz_init(y);
    mpz_init(x);
    mpz_set_str(p, pS.c_str(), 10);
    mpz_set_str(g, gS.c_str(), 10);
    mpz_set_str(y, yS.c_str(), 10);
    mpz_set_str(x, xS.c_str(), 10);

    mpz_powm(a, g, x, p);
    mpz_powm(b, y, k, p);
    // FIX: Incorrect calculation b
    mpz_mul_ui(b, b, M);
    mpz_mod(b, b, p);

    string retA = mpz_get_str(NULL, 10, a);
    string retB = mpz_get_str(NULL, 10, b);

    return make_pair(retA, retB);
}