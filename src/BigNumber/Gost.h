#ifndef GOST_H
#define GOST_H

#include "vector"
#include "string"
#include "gmpxx.h"
#include "cmath"

using namespace std;

const int CON_2_16 = 65536;
const string CON_2_161 = "2923003274661805836407369665432566039311865085952";
const string CON_2_1024 = "179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137216";

const int minPrimes[] = {1, 2, 3, 5, 11, 17, 37, 67, 131, 257, 521, 1031, 2053, 4099, 8209, 16411, 32771};

string getPrime(int x0, int c, int t);

#endif //GOST_H
