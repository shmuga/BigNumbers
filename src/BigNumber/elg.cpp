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
    mpz_sub_ui(num, num, 1);

    gmp_randstate_t r_state;

    srand(time(NULL) + rand());
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
        mpz_powm(tmp, g, r, p);
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
    mpz_init(k);
    mpz_init(a);
    mpz_init(b);
    mpz_init(p);
    mpz_init(g);
    mpz_init(y);
    mpz_init(x);
    mpz_set_str(k, generateX(pS).c_str(), 10);
    mpz_set_str(p, pS.c_str(), 10);
    mpz_set_str(g, gS.c_str(), 10);
    mpz_set_str(y, yS.c_str(), 10);
    mpz_set_str(x, xS.c_str(), 10);
    mpz_powm(a, g, k, p);
    mpz_powm(b, y, k, p);
    // FIX: Incorrect calculation b
    mpz_mul_ui(b, b, M);
    mpz_mod(b, b, p);

    string retA = mpz_get_str(NULL, 10, a);
    string retB = mpz_get_str(NULL, 10, b);
    return make_pair(retA, retB);
}

int ELGdecrypt(string bS, string aS, string xS, string pS) {
    mpz_t b, a, x, p, deg, res;
    mpz_init(b);
    mpz_init(res);
    mpz_init(deg);
    mpz_init(a);
    mpz_init(x);
    mpz_init(p);
    mpz_set_str(b, bS.c_str(), 10);
    mpz_set_str(a, aS.c_str(), 10);
    mpz_set_str(x, xS.c_str(), 10);
    mpz_set_str(p, pS.c_str(), 10);

    mpz_sub(deg, p, x);
    mpz_sub_ui(deg, deg, 1);
    mpz_powm(res, a, deg, p);
    mpz_mul(res, res, b);
    mpz_mod(res, res, p);
    return mpz_get_ui(res);
}

string generateK(string pS, string qS) {
    mpz_t p, q, r, res, tmp;
    mpz_init(res);
    mpz_init(tmp);
    mpz_init(p);
    mpz_init(q);
    mpz_init(r);

    mpz_set_str(p, pS.c_str(), 10);
    mpz_sub_ui(p, p, 1);
    mpz_set_str(q, qS.c_str(), 10);
    mpz_div(r, p, q);

    while(true) {
        string num = generateX(pS);
        mpz_set_str(res, num.c_str(), 10);
        mpz_gcd(tmp, res, p);
        if (mpz_cmp_ui(tmp, 1) == 0) break;
    }

    return mpz_get_str(NULL, 10, res);
}

pair<string, string> ELGsubscribe(int Ms, pair<string, string> pS, string gS, string xS) {
    string kS = generateK(pS.first, pS.second);
    string rS = generateY(pS.first, gS, kS);

    mpz_t p, g, x, k, r, M, s;
    mpz_init(s);
    mpz_init(p);
    mpz_init(g);
    mpz_init(x);
    mpz_init(k);
    mpz_init(r);
    mpz_init(M);

    mpz_set_str(p, pS.first.c_str(), 10);
    mpz_set_str(g, gS.c_str(), 10);
    mpz_set_str(x, xS.c_str(), 10);
    mpz_set_str(k, kS.c_str(), 10);
    mpz_set_str(r, rS.c_str(), 10);
    mpz_set_ui(M, Ms);


    mpz_sub_ui(p, p, 1);

    mpz_mul(x, x, r);
    mpz_sub(M, M, x);

    mpz_invert(k, k, p);
    mpz_mul(M, M, k);
    mpz_mod(s, M, p);

    string ret1 = mpz_get_str(NULL, 10, r);
    string ret2 = mpz_get_str(NULL, 10, s);

    return make_pair(ret1, ret2);
}

bool ELGcheck(int Ms, string pS, string gS, string yS, string rS, string sS) {
    mpz_t M, p, g, y, r, s;
    mpz_init(M);
    mpz_init(p);
    mpz_init(g);
    mpz_init(y);
    mpz_init(r);
    mpz_init(s);

    mpz_set_ui(M, Ms);
    mpz_set_str(p, pS.c_str(), 10);
    mpz_set_str(g, gS.c_str(), 10);
    mpz_set_str(y, yS.c_str(), 10);
    mpz_set_str(r, rS.c_str(), 10);
    mpz_set_str(s, sS.c_str(), 10);

    mpz_powm(y, y, r, p);
    mpz_powm(r, r, s, p);
    mpz_powm(g, g, M, p);
    mpz_mul(y, y, r);
    mpz_mod(y, y, p);
    string yy = mpz_get_str(NULL, 10, y);
    string gg = mpz_get_str(NULL, 10, g);
    return mpz_congruent_p(y, g, p);
}

