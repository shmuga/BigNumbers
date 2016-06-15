#ifndef GOST_H
#define GOST_H

#include "vector"
#include "string"
#include "gmpxx.h"
#include "cmath"
#include "utility"

using namespace std;

const int CON_2_16 = 65536;

const int minPrimes[] = {1, 2, 3, 5, 11, 17, 37, 67, 131, 257, 521, 1031, 2053, 4099, 8209, 16411, 32771};

pair<string, string> getPrime(int x0, int c, int t);

#endif //GOST_H
