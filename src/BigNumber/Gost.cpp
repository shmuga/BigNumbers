#include <iostream>
#include "Gost.h"


string getPrime(int x0, int c, int t0 = 1024) {

    cout << "\n\n";

    vector<int> y;
//    #1
    y.push_back(x0);
    vector<int> t;
    t.push_back(t0);
    vector<mpz_class> p;

//    #2
    for (int i = 0; t[i] >= 17; i++) {
        t.push_back(t[i] / 2);
    }
    int s = (int)t.size() - 1;

//    #3
    mpz_class px(minPrimes[t[s] - 1]);
    p.push_back(px);

//    #4
    int m = s - 1;

    int rm = 0;
    mpz_class Ymt = 0;
    mpz_class N, N1, N2, Nk;
    mpz_class Ym = 0;
    int k = 0;
    int flag = 0;

    while (m >= 0) {
//        #5
        rm = t[m+1] / 16;

//        #6
        step6:
        y.erase(y.begin() + 1, y.end());
        for (int i = 0; i < rm; i++) {
            y.push_back((19381 * y[i] + c) % CON_2_16);
        }

//        #7
        Ym = 0;
        for (int i = 0; i < rm; i++) {
            Ymt = CON_2_16;
            mpz_pow_ui(Ymt.get_mpz_t(), Ymt.get_mpz_t(), i);
            Ymt *= y[i];
            mpz_add(Ym.get_mpz_t(), Ym.get_mpz_t(), Ymt.get_mpz_t());
        }

//        #8
        y[0] = y[rm];

//        #9
        N1 = 2;
        mpz_pow_ui(N1.get_mpz_t(), N1.get_mpz_t(), (unsigned long)(t[m] - 1));
        N2 = N1;
        mpz_cdiv_q(N1.get_mpz_t(), N1.get_mpz_t(), p[s - m - 1].get_mpz_t());

        mpz_mul(N2.get_mpz_t(), N2.get_mpz_t(), Ym.get_mpz_t());
        Nk = 2;
        mpz_pow_ui(Nk.get_mpz_t(), Nk.get_mpz_t(), (unsigned long)(16 * rm));
        mpz_mul(Nk.get_mpz_t(), Nk.get_mpz_t(), p[s - m - 1].get_mpz_t());
        mpz_fdiv_q(N2.get_mpz_t(), N2.get_mpz_t(), Nk.get_mpz_t());
        mpz_add(N.get_mpz_t(), N1.get_mpz_t(), N2.get_mpz_t());
        if (mpz_odd_p(N.get_mpz_t())) {
            N += 1;
        }

//        #10
        k = -2;

        flag = 0;
        while (flag < 2) {
            flag = 0;

            k += 2;

//            #11
            px = N;
            px += k;
            mpz_mul(px.get_mpz_t(), px.get_mpz_t(), p[s - m - 1].get_mpz_t());
            px += 1;

            if (p.size() - 1 < (s - m)) {
                p.push_back(px);
            } else {
                p[s - m] = px;
            }

//            #12
            px = 2;
            mpz_pow_ui(px.get_mpz_t(), px.get_mpz_t(), (unsigned long)(t[m]));

            if (mpz_cmp(p[s - m].get_mpz_t(), px.get_mpz_t()) > 0) {
                goto step6;
            }

//            #13
            px = 2;
            Nk = N;
            Nk += k;
            mpz_mul(Nk.get_mpz_t(), Nk.get_mpz_t(), p[s - m - 1].get_mpz_t());
            mpz_powm(px.get_mpz_t(), px.get_mpz_t(), Nk.get_mpz_t(), p[s - m].get_mpz_t());
            if (px.get_si() == 1) {
                flag++;
            }

            px = 2;
            Nk = N;
            Nk += k;
            mpz_powm(px.get_mpz_t(), px.get_mpz_t(), Nk.get_mpz_t(), p[s - m].get_mpz_t());

            if (px.get_si() != 1) {
                flag++;
            }

        }

        cout << p[s - m].get_str(10) << "\n";

        m -= 1;

    }

    string rez = p[s - m - 1].get_str(10);
    return rez;

}