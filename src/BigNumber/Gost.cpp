#include <iostream>
#include "Gost.h"


string getPrime(int x0, int c, int t0 = 1024) {

    vector<int> y;
    y.push_back(x0);
    vector<int> t;
    t.push_back(t0);

    for (int i = 0; t[i] >= 17; i++) {
        t.push_back(t[i] / 2);
    }
    int s = (int)t.size() - 1;

    int ps = minPrimes[t[s]];
    int m = s - 1;
    int rm = t[m+1] / 16;

    for (int i = 0; i < rm; i++) {
        y.push_back((19381 * y[0] + c) % CON_2_16);
    }

    int Ymt = 0;
    for (int i = 0; i < rm; i++) {
        Ymt += y[i];
    }
    mpz_class Ym(CON_2_161);
    Ym *= Ymt;
    cout << "\n\n" << Ym.get_str(10) << "\n\n";

    y[0] = y[rm];

    mpz_class con_2_1024(CON_2_1024);
    mpz_class N, N1(CON_2_1024), N2(CON_2_1024);
    cout << "\n\n" << N1.get_str(10) << "\n\n";
    N1 /= (m * ps);
    cout << N1.get_str(10) << "\n\n";
    cout << N2.get_str(10) << "\n\n";
    mpz_mul(N2.get_mpz_t(), N2.get_mpz_t(), Ym.get_mpz_t());
    cout << N2.get_str(10) << "\n\n";
    N2 /= (m * ps);
    cout << N2.get_str(10) << "\n\n";
    N2 /= pow(CON_2_16, rm);
    cout << N2.get_str(10) << "\n\n";
    mpz_add(N.get_mpz_t(), N1.get_mpz_t(), N2.get_mpz_t());
    cout << N.get_str(10) << "\n\n";

    if (mpz_odd_p(N.get_mpz_t())) {
        N += 1;
    }

    int k = 0;




    string rez = "";
    return rez;

}